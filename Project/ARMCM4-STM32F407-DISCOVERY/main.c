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
#include "Lage.h"

static void pwmpcb(PWMDriver *pwmp);


 float inNickIstLage;
float inNickIstV;
float inNickSollLage;
float inRollIstLage;
float inRollIstV;
float inRollSollLage;
float inSchub;
float inYawIstLage;
float inYawIstV;
float inYawSollLage;
float outMotor1;
float outMotor2;
float outMotor3;
float outMotor4;	 
float Schub_Offset = 0;


float v_Nick_tp1 = 0;   	
float v_Nick_tp1_alt = 0;
float di_Nick = 0;
float pi_Nick = 0;
float ii_Nick = -10;
float ia_Nick = 0;
float pa_Nick = 0;
float ei_Nick = 0;
float ei_Nick_alt = 0;
float ea_Nick = 0;
float Soll_v_Nick = 0;
float aNick;

float v_Roll_tp1 = 0;   	
float v_Roll_tp1_alt = 0;
float di_Roll = 0;
float pi_Roll = 0;
float ii_Roll = 0;
float ia_Roll = 0;
float pa_Roll = 0;
float ei_Roll = 0;
float ei_Roll_alt = 0;
float ea_Roll = 0;
float Soll_v_Roll = 0;
float aRoll; 

float v_Yaw_tp1 = 0;   	
float v_Yaw_tp1_alt = 0;
float di_Yaw = 0;
float pi_Yaw = 0;
float ii_Yaw = 0;
float ia_Yaw = 0;
float pa_Yaw = 0;
float ei_Yaw = 0;
float ei_Yaw_alt = 0;
float ea_Yaw = 0;
float Soll_v_Yaw = 0;
float aYaw;
volatile unsigned short tmp111;


enum {UP, DOWN};
static int dir = UP,step = 5;

#define RC_IN_RANGE(x) (((x)>900 && (x)<2300))
volatile unsigned short RC_INPUT_CHANNELS[4], RC_INPUT_LAST_TCNT,tmp=0;
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
			if (RC_IN_RANGE(tmp)) RC_INPUT_CHANNELS[0] = tmp;
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

void Regelung(void)
{
	/// Werte übernehmen
	update_IMU();

	inSchub = float(RC_INPUT_CHANNELS[2] - 1100)/1000;
	
	inNickSollLage = 0;
	inRollSollLage = (float(RC_INPUT_CHANNELS[1]) - 1500)/1000;
	inYawSollLage  = 0;
	
	inYawIstLage = getEuler_yaw();
	inNickIstLage = getEuler_nick();
	inRollIstLage = getEuler_roll();
		 
	inRollIstV = getRate_roll();
	inNickIstV = getRate_nick();
	inYawIstV = getRate_yaw();

	/////////////////////////// Nick-Regler berechnen //////////////////////////////////////////
   //äußerer Regler
   ea_Nick = (inNickSollLage) - (inNickIstLage+0.08*inNickIstV);  // Eingang in den äußeren Regler
   if(inSchub > 0.1 && inSchub <=1 && inNickIstLage < 0.2 && inNickIstLage > -0.2)
   	ia_Nick = 0.01 * ea_Nick + ia_Nick;	//I-Anteil nur nahe der Nulllage verändern  
   //Soll_v_Nick = ea_Nick*1 + ia_Nick*0.02; //Sollgeschwindigkeit des äußeren Reglers
   //Soll_v_Nick = inNickSollLage*2.F;
   Soll_v_Nick = 0;
   //innerer Regler
   v_Nick_tp1 = 0.9*v_Nick_tp1 + 0.1*inNickIstV; // Tiefpass-gefilterter Gyrowert
   ei_Nick= Soll_v_Nick - v_Nick_tp1;	//Eingang in die innere Regelung
   pi_Nick = ei_Nick * 0.07;	//p-Anteil

   if(inSchub > 0.1 && inSchub <=1 /*&& inNickIstLage < 0.5 && inNickIstLage > -0.5*/)
   	ii_Nick = 0.08 * ei_Nick + ii_Nick;	//I-Anteil nur nahe der Nulllage verändern 

   di_Nick = (ei_Nick - ei_Nick_alt)*10; //d-Anteil  
   if (di_Nick > 1.5)
	   di_Nick = 1.5;
   else if(di_Nick < -1.5)
	   di_Nick = -1.5;	  //Saturierung des D-Anteils

   aNick = (pi_Nick + (ii_Nick)*0.03 + di_Nick)*567; //Ausgang des inneren Reglers	 

	/////////////////////////// Roll-Regler berechnen ////////////////////////////////////////// 

   /*ea_Roll = (inRollSollLage) - (inRollIstLage+0.08*inRollIstV);  // Eingang in den äußeren Regler
   if(inSchub > 0.1 && inSchub <=1 && inRollIstLage < 0.2 && inRollIstLage > -0.2)
   	ia_Roll = 0.01 * ea_Roll + ia_Roll;	//I-Anteil nur nahe der Nulllage verändern  
   Soll_v_Roll = ea_Roll*2 + ia_Roll*0.02; //Sollgeschwindigkeit des äußeren Reglers
   //Soll_v_Roll = inRollSollLage;

   //innerer Regler
   v_Roll_tp1 = 0.95*v_Roll_tp1 + 0.05*inRollIstV; // Tiefpass-gefilterter Gyrowert
   ei_Roll= Soll_v_Roll - v_Roll_tp1;	//Eingang in die innere Regelung
   pi_Roll = ei_Roll * 0.5;	//p-Anteil
   if(inSchub > 0.1 && inSchub <=1 && inRollIstLage < 0.2 && inRollIstLage > -0.2)
   	ii_Roll = 0.1 * ei_Roll + ii_Roll;	//I-Anteil nur nahe der Nulllage verändern 

   di_Roll = (ei_Roll - ei_Roll_alt)*20; //d-Anteil  
   if (di_Roll > 1.5)
	   di_Roll = 1.5;
   else if(di_Roll < -1.5)
	   di_Roll = -1.5;	  //Saturierung des D-Anteils

   aRoll = (pi_Roll + (ii_Roll+37)*0.02 + di_Roll)*567; //Ausgang des inneren Reglers		 */

	ea_Roll = (inRollSollLage) - (inRollIstLage+0.08*inRollIstV);  // Eingang in den äußeren Regler
   if(inSchub > 0.1 && inSchub <=1 && inRollIstLage < 0.2 && inRollIstLage > -0.2)
   	ia_Roll = 0.01 * ea_Roll + ia_Roll;	//I-Anteil nur nahe der Nulllage verändern  
   Soll_v_Roll = ea_Roll*2 + ia_Roll*0.02; //Sollgeschwindigkeit des äußeren Reglers
   Soll_v_Roll = inRollSollLage;

   //innerer Regler
   v_Roll_tp1 = 0.95*v_Roll_tp1 + 0.05*inRollIstV; // Tiefpass-gefilterter Gyrowert
   ei_Roll= Soll_v_Roll - v_Roll_tp1;	//Eingang in die innere Regelung
   pi_Roll = ei_Roll * 0.1;	//p-Anteil
   if(inSchub > 0.1 && inSchub <=1 /*&& inRollIstLage < 0.2 && inRollIstLage > -0.2*/)
   	ii_Roll = 0.01 * ei_Roll + ii_Roll;	//I-Anteil nur nahe der Nulllage verändern 


   di_Roll = (ei_Roll - ei_Roll_alt)*10; //d-Anteil  
   if (di_Roll > 1.5)
	   di_Roll = 1.5;
   else if(di_Roll < -1.5)
	   di_Roll = -1.5;	  //Saturierung des D-Anteils

   aRoll = (pi_Roll + (ii_Roll)*0.005 + di_Roll)*567; //Ausgang des inneren Reglers

	/////////////////////////// Yaw-Regler berechnen ////////////////////////////////////////// 

   ea_Yaw = (inYawSollLage) - (inYawIstLage+0.08*inYawIstV);  // Eingang in den äußeren Regler
   if(inSchub > 0.1 && inSchub <=1 && inYawIstLage < 0.2 && inYawIstLage > -0.2)
   	ia_Yaw = 0.01 * ea_Yaw + ia_Yaw;	//I-Anteil nur nahe der Nulllage verändern  
   Soll_v_Yaw = ea_Yaw*0.5 + ia_Yaw*0.002; //Sollgeschwindigkeit des äußeren Reglers
   //Soll_v_Yaw = inYawSollLage;

   //innerer Regler
   v_Yaw_tp1 = 0.99*v_Yaw_tp1 + 0.01*inYawIstV; // Tiefpass-gefilterter Gyrowert
   ei_Yaw= Soll_v_Yaw - v_Yaw_tp1;	//Eingang in die innere Regelung
   pi_Yaw = ei_Yaw * 0.4;	//p-Anteil
   if(inSchub > 0.1 && inSchub <=1 && inYawIstLage < 0.2 && inYawIstLage > -0.2)
   	ii_Yaw = 0.01 * ei_Yaw + ii_Yaw;	//I-Anteil nur nahe der Nulllage verändern 

   di_Yaw = (ei_Yaw - ei_Yaw_alt)*20; //d-Anteil  
   if (di_Yaw > 1.5)
	   di_Yaw = 1.5;
   else if(di_Yaw < -1.5)
	   di_Yaw = -1.5;	  //Saturierung des D-Anteils

   aYaw = (pi_Yaw + (ii_Yaw)*0.1 + di_Yaw)*567; //Ausgang des inneren Reglers
   
   if (aYaw > 2000.F)
	   aYaw = 2000.F;
   else if(aYaw < -2000.F)
	   aYaw = -2000.F;	  //Saturierung des Yaw-Anteils
  	
	/////////////////////////// Motorwerte setzen //////////////////////////////////////////  
   if(inSchub > 0.1 && inSchub <=1)
   {
     Schub_Offset = 6000 * inSchub;
	 aNick = 0.F;	
	 aYaw = 0.F;

     outMotor3 = Schub_Offset - aRoll + aYaw;
     //outMotor1 = Schub_Offset + aNick - aYaw;
     //outMotor4 = Schub_Offset - aNick - aYaw; 
  	 outMotor1 = 0.F;
 	 outMotor4 = 0.F;
     outMotor2 = Schub_Offset + aRoll + aYaw;
   }
   else 
   {					
  	 outMotor1 = 0.F;
  	 outMotor2 = 0.F;
 	 outMotor3 = 0.F;
 	 outMotor4 = 0.F;
   }						   
	/////////////////////////// Motorwerte saturieren ////////////////////////////////////////// 

   if (outMotor1 > 6800.F)
	   outMotor1 = 6800.F;
   else if(outMotor1 < 0.F)
	   outMotor1 = 0.F;

   if (outMotor2 > 6800.F)
	   outMotor2 = 6800.F;
   else if(outMotor2 < 0.F)
	   outMotor2 = 0.F;

   if (outMotor3 > 6800.F)
	   outMotor3 = 6800.F;
   else if(outMotor3 < 0.F)
	   outMotor3 = 0.F;

   if (outMotor4 > 6800.F)
	   outMotor4 = 6800.F;
   else if(outMotor4 < 0.F)
	   outMotor4 = 0.F;	
	   
/*	if(outMotor2 == 900) dir = UP;
    else if (outMotor2 == 3000) dir = DOWN;
    if (dir == UP) outMotor2 += step;
    else if (dir == DOWN) outMotor2 -= step;
    if (dir == UP) outMotor3 += step;
    else if (dir == DOWN) outMotor3 -= step;*/			  

	/////////////////////////// Alte Werte merken ////////////////////////////////////////// 

   v_Nick_tp1_alt = v_Nick_tp1;
   ei_Nick_alt = ei_Nick;

   v_Roll_tp1_alt = v_Roll_tp1;
   ei_Roll_alt = ei_Roll;

   v_Yaw_tp1_alt = v_Yaw_tp1;
   ei_Yaw_alt = ei_Yaw;

}
/*
 * Working area for Regelung
 */
static WORKING_AREA(RegelungThreadWorkingArea, 128);
//int dennis=0; 
/*
 * Regelungsthread
 */
static msg_t Regelungsthread(void *arg) {
 
  systime_t time = chTimeNow();     // Tnow
  while (TRUE) {
    time += MS2ST(5);            // Next deadline
    
	
	Regelung();
	
	 //
	//chThdSleepMilliseconds(10); /* Fixed interval.*/
    chThdSleepUntil(time);
  }
}



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

   //chprintf((BaseChannel *)&SD2,"inside i2c1 scanner");
   for(x=0;x<128;x++){

      txbuf[0] = 0x00;
      txbuf[1] = 0x00;

       messages = i2cMasterTransmit(&I2CD1, x, txbuf, 2, rxbuf, 0);
       //if(messages == 0)chprintf((BaseChannel *)&SD2, "I2C1: Sensor is available on Address: 0x%x \r\n", x, messages);
	
      chThdSleepMilliseconds(1);
      }
	 chThdSleepMilliseconds(500);
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

	outMotor2 = 900;
	outMotor3 = 1100;
	
	//Regelungsthread anlegen
	//Thread *tp = chThdCreateFromHeap(NULL, THD_WA_SIZE(128), NORMALPRIO+1, Regelungsthread, NULL);
  //	if (tp == NULL)
  //  	chSysHalt();    /* Memory exausted. */
	chThdCreateStatic(RegelungThreadWorkingArea, sizeof(RegelungThreadWorkingArea), NORMALPRIO, Regelungsthread, NULL);
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
  		static int width1 = 700, width2 = 700,width3 = 700,width4 = 700; /* starts at .7ms, ends at 2.0ms */
		//i2c_scanner1();
 	
    	pwmEnableChannel(&PWMD5, 0, width1);
    	pwmEnableChannel(&PWMD5, 1, width2);
    	pwmEnableChannel(&PWMD5, 2, width3);
    	pwmEnableChannel(&PWMD5, 3, width4);

		width1 = outMotor1/6800*1000+1000;
		width2 = outMotor2/6800*1000+1000;
		width3 = outMotor3/6800*1000+1000;
		width4 = outMotor4/6800*1000+1000;

	    chThdSleepMilliseconds(10);
  }
}
