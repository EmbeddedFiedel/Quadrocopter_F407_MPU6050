/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

#include "ch.h"
#include "hal.h"
#include "test.h"
#include "lis302dl.h"
#include "chprintf.h"

static void pwmpcb(PWMDriver *pwmp);
static void adccb(ADCDriver *adcp, adcsample_t *buffer, size_t n);
static void spicb(SPIDriver *spip);

/* Total number of channels to be sampled by a single ADC operation.*/
#define ADC_GRP1_NUM_CHANNELS   2

/* Depth of the conversion buffer, channels are sampled four times each.*/
#define ADC_GRP1_BUF_DEPTH      4

/*
 * ADC samples buffer.
 */
static adcsample_t samples[ADC_GRP1_NUM_CHANNELS * ADC_GRP1_BUF_DEPTH];

/*
 * ADC conversion group.
 * Mode:        Linear buffer, 4 samples of 2 channels, SW triggered.
 * Channels:    IN11   (48 cycles sample time)
 *              Sensor (192 cycles sample time)
 */
static const ADCConversionGroup adcgrpcfg = {
  FALSE,
  ADC_GRP1_NUM_CHANNELS,
  adccb,
  NULL,
  /* HW dependent part.*/
  0,
  ADC_CR2_SWSTART,
  ADC_SMPR1_SMP_AN11(ADC_SAMPLE_56) | ADC_SMPR1_SMP_SENSOR(ADC_SAMPLE_144),
  0,
  ADC_SQR1_NUM_CH(ADC_GRP1_NUM_CHANNELS),
  0,
  ADC_SQR3_SQ2_N(ADC_CHANNEL_IN11) | ADC_SQR3_SQ1_N(ADC_CHANNEL_SENSOR)
};

/*
 * PWM configuration structure.
 * Cyclic callback enabled, channels 1 and 4 enabled without callbacks,
 * the active state is a logic one.
 */
static PWMConfig pwmcfg = {
  10000,                                    /* 10kHz PWM clock frequency.   */
  10000,                                    /* PWM period 1S (in ticks).    */
  pwmpcb,
  {
    {PWM_OUTPUT_ACTIVE_HIGH, NULL},
    {PWM_OUTPUT_DISABLED, NULL},
    {PWM_OUTPUT_DISABLED, NULL},
    {PWM_OUTPUT_ACTIVE_HIGH, NULL}
  },
  /* HW dependent part.*/
  0
};

/*
 * SPI1 configuration structure.
 * Speed 5.25MHz, CPHA=1, CPOL=1, 8bits frames, MSb transmitted first.
 * The slave select line is the pin GPIOE_CS_SPI on the port GPIOE.
 */
static const SPIConfig spi1cfg = {
  NULL,
  /* HW dependent part.*/
  GPIOE,
  GPIOE_CS_SPI,
  SPI_CR1_BR_0 | SPI_CR1_BR_1 | SPI_CR1_CPOL | SPI_CR1_CPHA
};

/*
 * SPI2 configuration structure.
 * Speed 21MHz, CPHA=0, CPOL=0, 16bits frames, MSb transmitted first.
 * The slave select line is the pin 12 on the port GPIOA.
 */
static const SPIConfig spi2cfg = {
  spicb,
  /* HW dependent part.*/
  GPIOB,
  12,
  SPI_CR1_DFF
};

static const I2CConfig i2cfg1 = {
    OPMODE_I2C,
    100000,
    STD_DUTY_CYCLE,
};

/*
 * PWM cyclic callback.
 * A new ADC conversion is started.
 */
static void pwmpcb(PWMDriver *pwmp) {

  (void)pwmp;

  /* Starts an asynchronous ADC conversion operation, the conversion
     will be executed in parallel to the current PWM cycle and will
     terminate before the next PWM cycle.*/
  chSysLockFromIsr();
  adcStartConversionI(&ADCD1, &adcgrpcfg, samples, ADC_GRP1_BUF_DEPTH);
  chSysUnlockFromIsr();
}

/*
 * ADC end conversion callback.
 * The PWM channels are reprogrammed using the latest ADC samples.
 * The latest samples are transmitted into a single SPI transaction.
 */
void adccb(ADCDriver *adcp, adcsample_t *buffer, size_t n) {

  (void) buffer; (void) n;
  /* Note, only in the ADC_COMPLETE state because the ADC driver fires an
     intermediate callback when the buffer is half full.*/
  if (adcp->state == ADC_COMPLETE) {
    adcsample_t avg_ch1, avg_ch2;

    /* Calculates the average values from the ADC samples.*/
    avg_ch1 = (samples[0] + samples[2] + samples[4] + samples[6]) / 4;
    avg_ch2 = (samples[1] + samples[3] + samples[5] + samples[7]) / 4;

    chSysLockFromIsr();

    /* Changes the channels pulse width, the change will be effective
       starting from the next cycle.*/
    pwmEnableChannelI(&PWMD4, 0, PWM_FRACTION_TO_WIDTH(&PWMD4, 4096, avg_ch1));
    pwmEnableChannelI(&PWMD4, 3, PWM_FRACTION_TO_WIDTH(&PWMD4, 4096, avg_ch2));

    /* SPI slave selection and transmission start.*/
    spiSelectI(&SPID2);
    spiStartSendI(&SPID2, ADC_GRP1_NUM_CHANNELS * ADC_GRP1_BUF_DEPTH, samples);

    chSysUnlockFromIsr();
  }
}

/*
 * SPI end transfer callback.
 */
static void spicb(SPIDriver *spip) {

  /* On transfer end just releases the slave select line.*/
  chSysLockFromIsr();
  spiUnselectI(spip);
  chSysUnlockFromIsr();
}

/*
 * This is a periodic thread that does absolutely nothing except flashing
 * a LED.
 */
static WORKING_AREA(waThread1, 128);
static msg_t Thread1(void *arg) {

  (void)arg;
  chRegSetThreadName("blinker");
  while (TRUE) {
    palSetPad(GPIOD, GPIOD_LED3);       /* Orange.  */
    chThdSleepMilliseconds(500);
    palClearPad(GPIOD, GPIOD_LED3);     /* Orange.  */
    chThdSleepMilliseconds(500);
  }
}
void I2CInitialize(void){
  i2cInit();

i2cStart(&I2CD1, &i2cfg1);

// Link PB8 and PB9 to I2C1 function
palSetPadMode(GPIOB, 6,  PAL_MODE_ALTERNATE(4));// 
palSetPadMode(GPIOB, 7,  PAL_MODE_ALTERNATE(4));// 
// startups. Pauses added just to be safe
chThdSleepMilliseconds(100);
}

void i2c_scanner1(void){
   uint8_t x = 0, txbuf[2],rxbuf[6];
   int32_t messages = 0;

   chprintf((BaseChannel *)&SD2,"inside i2c1 scanner");
   for(x=0;x<128;x++){

      txbuf[0] = 0x00;
      txbuf[1] = 0x00;

       messages = i2cMasterTransmit(&I2CD1, x, txbuf, 2, rxbuf, 0);
       if(messages == 0)chprintf((BaseChannel *)&SD2, "I2C1: Sensor is available on Address: %x \r\n", x, messages);
	
      chThdSleepMilliseconds(1);
      }
	 chThdSleepMilliseconds(500);
}
void readAcc(void)
{
 uint8_t rxbuf[6], addr[1];
  int32_t messages = 0;
  addr[0] = 0x3B;
	messages = i2cMasterTransmit(&I2CD1, 0x68, addr, 1, rxbuf, 6);
	if(messages == 0) chprintf((BaseChannel *)&SD2, "Acc MPU6000: %d %d %d \r\n", (int16_t)((rxbuf[0]<<8)+rxbuf[1]),(int16_t)((rxbuf[2]<<8)+rxbuf[3]),(int16_t)((rxbuf[4]<<8)+rxbuf[5]));
	 
}
/*
 * Application entry point.
 */
int main(void) {

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();
   I2CInitialize();
  /*
   * Activates the serial driver 2 using the driver default configuration.
   * PA2(TX) and PA3(RX) are routed to USART2.
   */
  sdStart(&SD2, NULL);
  palSetPadMode(GPIOA, 2, PAL_MODE_ALTERNATE(7));
  palSetPadMode(GPIOA, 3, PAL_MODE_ALTERNATE(7));

  /*
   * If the user button is pressed after the reset then the test suite is
   * executed immediately before activating the various device drivers in
   * order to not alter the benchmark scores.
   */
  if (palReadPad(GPIOA, GPIOA_BUTTON))
    TestThread(&SD2);

  /*
   * Initializes the SPI driver 2. The SPI2 signals are routed as follow:
   * PB12 - NSS.
   * PB13 - SCK.
   * PB14 - MISO.
   * PB15 - MOSI.
   */
  spiStart(&SPID2, &spi2cfg);
  palSetPad(GPIOB, 12);
  palSetPadMode(GPIOB, 12, PAL_MODE_OUTPUT_PUSHPULL |
                           PAL_STM32_OSPEED_HIGHEST);           /* NSS.     */
  palSetPadMode(GPIOB, 13, PAL_MODE_ALTERNATE(5) |
                           PAL_STM32_OSPEED_HIGHEST);           /* SCK.     */
  palSetPadMode(GPIOB, 14, PAL_MODE_ALTERNATE(5));              /* MISO.    */
  palSetPadMode(GPIOB, 15, PAL_MODE_ALTERNATE(5) |
                           PAL_STM32_OSPEED_HIGHEST);           /* MOSI.    */

  /*
   * Initializes the ADC driver 1 and enable the thermal sensor.
   * The pin PC0 on the port GPIOC is programmed as analog input.
   */
  adcStart(&ADCD1, NULL);
  adcSTM32EnableTSVREFE();
  palSetPadMode(GPIOC, 1, PAL_MODE_INPUT_ANALOG);

  /*
   * Initializes the PWM driver 4, routes the TIM4 outputs to the board LEDs.
   */
  pwmStart(&PWMD4, &pwmcfg);
  palSetPadMode(GPIOD, GPIOD_LED4, PAL_MODE_ALTERNATE(2));  /* Green.   */
  palSetPadMode(GPIOD, GPIOD_LED6, PAL_MODE_ALTERNATE(2));  /* Blue.    */

  /*
   * Creates the example thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

  /*
   * Initializes the SPI driver 1 in order to access the MEMS. The signals
   * are initialized in the board file.
   * Several LIS302DL registers are then initialized.
   */
  spiStart(&SPID1, &spi1cfg);
  lis302dlWriteRegister(&SPID1, LIS302DL_CTRL_REG1, 0x43);
  lis302dlWriteRegister(&SPID1, LIS302DL_CTRL_REG2, 0x00);
  lis302dlWriteRegister(&SPID1, LIS302DL_CTRL_REG3, 0x00);

  /*
   * Normal main() thread activity, in this demo it does nothing except
   * sleeping in a loop and check the button state, when the button is
   * pressed the test procedure is launched with output on the serial
   * driver 2.
   */
  while (TRUE) {
    int8_t x, y, z;
	i2c_scanner1();
    if (palReadPad(GPIOA, GPIOA_BUTTON))
      TestThread(&SD2);

    x = (int8_t)lis302dlReadRegister(&SPID1, LIS302DL_OUTX);
    y = (int8_t)lis302dlReadRegister(&SPID1, LIS302DL_OUTY);
    z = (int8_t)lis302dlReadRegister(&SPID1, LIS302DL_OUTZ);
    chprintf((BaseChannel *)&SD2, "%d, %d, %d\r\n", x, y, z);
    chThdSleepMilliseconds(500);
  }
}
