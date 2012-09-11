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
#include "MPU6050_DMP6.cpp"

static void pwmpcb(PWMDriver *pwmp);






#define RC_IN_RANGE(x) (((x)>900 && (x)<2300))
volatile unsigned short RC_INPUT_CHANNELS[4], RC_INPUT_LAST_TCNT;
char PPM_FRAME_GOOD = 1;

/*
 *  _____       _                             _
 * |_   _|     | |                           | |
 *   | |  _ __ | |_ ___ _ __ _ __ _   _ _ __ | |_ ___
 *   | | | '_ \| __/ _ \ '__| '__| | | | '_ \| __/ __|
 *  _| |_| | | | ||  __/ |  | |  | |_| | |_) | |_\__ \
 * |_____|_| |_|\__\___|_|  |_|   \__,_| .__/ \__|___/
 *                                     | |
 *                                     |_|
 */
void rx_channel1_interrupt(EXTDriver *extp, expchannel_t channel) {
		(void)extp;
		(void)channel;

		chSysLockFromIsr();
		if (palReadPad(GPIOB, 13) == PAL_LOW) {
			unsigned short tmp = TIM4->CNT - RC_INPUT_LAST_TCNT;
			if (RC_IN_RANGE(tmp)) 
			{
				RC_INPUT_CHANNELS[0] = tmp;
				chprintf((BaseChannel *)&SD2, "Channel1: %u\r\n", RC_INPUT_CHANNELS[0]);
			}
		}
		RC_INPUT_LAST_TCNT = TIM4->CNT;
		chSysUnlockFromIsr();
}
void rx_channel2_interrupt(EXTDriver *extp, expchannel_t channel) {
		(void)extp;
		(void)channel;

		chSysLockFromIsr();
		if (palReadPad(GPIOB, 12) == PAL_LOW) {
			unsigned short tmp = TIM4->CNT - RC_INPUT_LAST_TCNT;
			if (RC_IN_RANGE(tmp)) 
			{
				RC_INPUT_CHANNELS[1] = tmp;
				chprintf((BaseChannel *)&SD2, "Channel2: %u\r\n", RC_INPUT_CHANNELS[1]);
			}
		}
		RC_INPUT_LAST_TCNT = TIM4->CNT;
		chSysUnlockFromIsr();
}
void rx_channel3_interrupt(EXTDriver *extp, expchannel_t channel) {
		(void)extp;
		(void)channel;

		chSysLockFromIsr();
		if (palReadPad(GPIOB, 1) == PAL_LOW) {
			unsigned short tmp = TIM4->CNT - RC_INPUT_LAST_TCNT;
			if (RC_IN_RANGE(tmp)) 
			{
				RC_INPUT_CHANNELS[2] = tmp;
				chprintf((BaseChannel *)&SD2, "Channel3: %u\r\n", RC_INPUT_CHANNELS[2]);
			}
		}
		RC_INPUT_LAST_TCNT = TIM4->CNT;
		chSysUnlockFromIsr();
}
void rx_channel4_interrupt(EXTDriver *extp, expchannel_t channel) {
		(void)extp;
		(void)channel;

		chSysLockFromIsr();
		if (palReadPad(GPIOB, 0) == PAL_LOW) {
			unsigned short tmp = TIM4->CNT - RC_INPUT_LAST_TCNT;
			if (RC_IN_RANGE(tmp)) 
			{
				RC_INPUT_CHANNELS[3] = tmp;
				chprintf((BaseChannel *)&SD2, "Channel4: %u\r\n", RC_INPUT_CHANNELS[3]);
			}
		}
		RC_INPUT_LAST_TCNT = TIM4->CNT;
		chSysUnlockFromIsr();
}
static const EXTConfig extcfg = {
	{
	    {EXT_CH_MODE_BOTH_EDGES | EXT_CH_MODE_AUTOSTART, rx_channel4_interrupt},
   	 	{EXT_CH_MODE_BOTH_EDGES | EXT_CH_MODE_AUTOSTART, rx_channel3_interrupt},
   	 	{EXT_CH_MODE_DISABLED, NULL},
    	{EXT_CH_MODE_DISABLED, NULL},
		{EXT_CH_MODE_DISABLED, NULL},
		{EXT_CH_MODE_DISABLED, NULL},
		{EXT_CH_MODE_DISABLED, NULL},
    	{EXT_CH_MODE_DISABLED, NULL},
    	{EXT_CH_MODE_DISABLED, NULL},
    	{EXT_CH_MODE_DISABLED, NULL},
    	{EXT_CH_MODE_DISABLED, NULL},
    	{EXT_CH_MODE_DISABLED, NULL},
		{EXT_CH_MODE_BOTH_EDGES | EXT_CH_MODE_AUTOSTART, rx_channel2_interrupt},
		{EXT_CH_MODE_BOTH_EDGES | EXT_CH_MODE_AUTOSTART, rx_channel1_interrupt},
		{EXT_CH_MODE_DISABLED, NULL},
		{EXT_CH_MODE_DISABLED, NULL},
	},
	EXT_MODE_EXTI(EXT_MODE_GPIOB, /* 0 */
	              EXT_MODE_GPIOB, /* 1 */
	              0, /* 2 */
	              0, /* 3 */
	              0, /* 4 */
	              0, /* 5 */
	              0, /* 6 */
	              0, /* 7 */
	              0, /* 8 */
	              0, /* 9 */
	              0, /* 10 */
	              0, /* 11 */
	              EXT_MODE_GPIOD,//EXT_MODE_GPIOB, /* 12 */
	              EXT_MODE_GPIOD,//EXT_MODE_GPIOB, /* 13 */
	              0,//EXT_MODE_GPIOB, /* 14 */
	              0)//EXT_MODE_GPIOB) /* 15 */
};





/*
 * PWM configuration structure.
 * Cyclic callback enabled, channels 1 and 4 enabled without callbacks,
 * the active state is a logic one.
 */
static PWMConfig pwmcfg_esc = {
  1000000,                                    /* 10kHz PWM clock frequency.   */
  20000,                                    /* PWM period 20ms (in ticks).    */
  NULL,
  {
    {PWM_OUTPUT_ACTIVE_HIGH, NULL},
    {PWM_OUTPUT_ACTIVE_HIGH, NULL},
    {PWM_OUTPUT_ACTIVE_HIGH, NULL},
    {PWM_OUTPUT_ACTIVE_HIGH, NULL}
  },
  /* HW dependent part.*/
  0
};



static const I2CConfig i2cfg1 = {
    OPMODE_I2C,
    100000,
    STD_DUTY_CYCLE,
};



void I2CInitialize(void)
{
  	i2cInit();
	i2cStart(&I2CD1, &i2cfg1);
	// Link PB6 and PB6 to I2C1 function
	palSetPadMode(GPIOB, 6,  PAL_MODE_ALTERNATE(4)); 
	palSetPadMode(GPIOB, 7,  PAL_MODE_ALTERNATE(4)); 

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
       if(messages == 0)chprintf((BaseChannel *)&SD2, "I2C1: Sensor is available on Address: 0x%x \r\n", x, messages);
	
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

// ================================================================
// ===                      INITIAL SETUP                       ===
// ================================================================

void setup_IMU() 
{
    mpu.initialize();
    devStatus = mpu.dmpInitialize();
    if (devStatus == 0) 
	{
    	mpu.setDMPEnabled(true);
        mpuIntStatus = mpu.getIntStatus();
        dmpReady = true;
		chprintf((BaseChannel *)&SD2, "DMP Activated\r\n");
        packetSize = mpu.dmpGetFIFOPacketSize();
		chprintf((BaseChannel *)&SD2, "FIFOPacketSize: %u\r\n",packetSize);
    } 
}


/*
 * Application entry point.
 */
int main(void) 
{

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
	palSetPadMode(GPIOD, 5, PAL_MODE_ALTERNATE(7));
	palSetPadMode(GPIOD, 6, PAL_MODE_ALTERNATE(7));
	
	/*
	* Initializes the PWM driver 5 for ESCs
	*/
	palSetPadMode(GPIOA, 0, PAL_MODE_ALTERNATE(2)); 
	palSetPadMode(GPIOA, 1, PAL_MODE_ALTERNATE(2));  
	palSetPadMode(GPIOA, 2, PAL_MODE_ALTERNATE(2));
	palSetPadMode(GPIOA, 3, PAL_MODE_ALTERNATE(2)); 
	pwmStart(&PWMD5, &pwmcfg_esc);

	setup_IMU();


	/*
	* Enable Timer 4
	*/
	
	TIM4->CR1 = 0x00000000;
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
	TIM4->SMCR = 0; // slave mode disabled
	TIM4->PSC = 84; // 84 mhz maximum apb1 bus speed
	TIM4->ARR = 0xffff;
	TIM4->SR = 0;
	TIM4->DIER = 0;
	TIM4->CR1 = 0x00000001;
	
	extStart(&EXTD1, &extcfg);  
	/*
	* Normal main() thread activity, in this demo it does nothing except
	* sleeping in a loop and check the button state, when the button is
	* pressed the test procedure is launched with output on the serial
	* driver 2.
	*/
	while (TRUE) 
	{
		int8_t x, y, z;
		enum {UP, DOWN};
  		static int dir = UP, step = 5, width = 700; /* starts at .7ms, ends at 2.0ms */
		//i2c_scanner1();

 		mpuIntStatus = mpu.getIntStatus();
		//chprintf((BaseChannel *)&SD2, "mpuIntStatus: 0x%x\r\n",mpuIntStatus);

    	// get current FIFO count
    	fifoCount = mpu.getFIFOCount();
		//chprintf((BaseChannel *)&SD2, "fifoCount: %u\r\n",fifoCount);

    	// check for overflow (this should never happen unless our code is too inefficient)
    	if ((mpuIntStatus & 0x10) || fifoCount == 1024) 
		{
        	// reset so we can continue cleanly
        	mpu.resetFIFO();
			//chprintf((BaseChannel *)&SD2, "RESET\r\n",fifoCount);	

    	// otherwise, check for DMP data ready interrupt (this should happen frequently)
    	} 
		else if (mpuIntStatus & 0x02) 
		{
	        // wait for correct available data length, should be a VERY short wait
	        while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();
	
	        // read a packet from FIFO
	        mpu.getFIFOBytes(fifoBuffer, packetSize);
	        
	        // track FIFO count here in case there is > 1 packet available
	        // (this lets us immediately read more without waiting for an interrupt)
	        fifoCount -= packetSize;
			mpu.dmpGetQuaternion(&q, fifoBuffer);
        	mpu.dmpGetEuler(euler, &q);
			//chprintf((BaseChannel *)&SD2, "Angle: %d %d %d \r\n",(int16_t)(euler[0]*57.2957795),(int16_t)(euler[1]*57.2957795),(int16_t)(euler[2]*57.2957795));
		}
	
    pwmEnableChannel(&PWMD5, 0, width);
    pwmEnableChannel(&PWMD5, 1, width);
    pwmEnableChannel(&PWMD5, 2, width);
    pwmEnableChannel(&PWMD5, 3, width);
    if(width == 900) dir = UP;
    else if (width == 2000) dir = DOWN;
    if (dir == UP) width += step;
    else if (dir == DOWN) width -= step;

   // x = (int8_t)lis302dlReadRegister(&SPID1, LIS302DL_OUTX);
   // y = (int8_t)lis302dlReadRegister(&SPID1, LIS302DL_OUTY);
   // z = (int8_t)lis302dlReadRegister(&SPID1, LIS302DL_OUTZ);
   // chprintf((BaseChannel *)&SD2, "%d, %d, %d\r\n", x, y, z);
    chThdSleepMilliseconds(10);
  }
}
