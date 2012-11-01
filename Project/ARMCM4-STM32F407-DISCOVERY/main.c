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
#include "Fernsteuerung.h"
#include "Motoren.h"

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



void Regelung(void)
{
	/// Werte �bernehmen
	update_IMU();

	inSchub = getSchub();
	
	inNickSollLage = 0;
	inRollSollLage = getRoll();
	inYawSollLage  = 0;
	
	inYawIstLage = getEuler_yaw();
	inNickIstLage = getEuler_nick();
	inRollIstLage = getEuler_roll();
		 
	inRollIstV = getRate_roll();
	inNickIstV = getRate_nick();
	inYawIstV = getRate_yaw();

	/////////////////////////// Nick-Regler berechnen //////////////////////////////////////////
   //�u�erer Regler
   ea_Nick = (inNickSollLage) - (inNickIstLage+0.08*inNickIstV);  // Eingang in den �u�eren Regler
   if(inSchub > 0.1 && inSchub <=1 && inNickIstLage < 0.2 && inNickIstLage > -0.2)
   	ia_Nick = 0.01 * ea_Nick + ia_Nick;	//I-Anteil nur nahe der Nulllage ver�ndern  
   //Soll_v_Nick = ea_Nick*1 + ia_Nick*0.02; //Sollgeschwindigkeit des �u�eren Reglers
   //Soll_v_Nick = inNickSollLage*2.F;
   Soll_v_Nick = 0;
   //innerer Regler
   v_Nick_tp1 = 0.9*v_Nick_tp1 + 0.1*inNickIstV; // Tiefpass-gefilterter Gyrowert
   ei_Nick= Soll_v_Nick - v_Nick_tp1;	//Eingang in die innere Regelung
   pi_Nick = ei_Nick * 0.07;	//p-Anteil

   if(inSchub > 0.1 && inSchub <=1 /*&& inNickIstLage < 0.5 && inNickIstLage > -0.5*/)
   	ii_Nick = 0.08 * ei_Nick + ii_Nick;	//I-Anteil nur nahe der Nulllage ver�ndern 

   di_Nick = (ei_Nick - ei_Nick_alt)*10; //d-Anteil  
   if (di_Nick > 1.5)
	   di_Nick = 1.5;
   else if(di_Nick < -1.5)
	   di_Nick = -1.5;	  //Saturierung des D-Anteils

   aNick = (pi_Nick + (ii_Nick)*0.03 + di_Nick)*567; //Ausgang des inneren Reglers	 

	/////////////////////////// Roll-Regler berechnen ////////////////////////////////////////// 

   /*ea_Roll = (inRollSollLage) - (inRollIstLage+0.08*inRollIstV);  // Eingang in den �u�eren Regler
   if(inSchub > 0.1 && inSchub <=1 && inRollIstLage < 0.2 && inRollIstLage > -0.2)
   	ia_Roll = 0.01 * ea_Roll + ia_Roll;	//I-Anteil nur nahe der Nulllage ver�ndern  
   Soll_v_Roll = ea_Roll*2 + ia_Roll*0.02; //Sollgeschwindigkeit des �u�eren Reglers
   //Soll_v_Roll = inRollSollLage;

   //innerer Regler
   v_Roll_tp1 = 0.95*v_Roll_tp1 + 0.05*inRollIstV; // Tiefpass-gefilterter Gyrowert
   ei_Roll= Soll_v_Roll - v_Roll_tp1;	//Eingang in die innere Regelung
   pi_Roll = ei_Roll * 0.5;	//p-Anteil
   if(inSchub > 0.1 && inSchub <=1 && inRollIstLage < 0.2 && inRollIstLage > -0.2)
   	ii_Roll = 0.1 * ei_Roll + ii_Roll;	//I-Anteil nur nahe der Nulllage ver�ndern 

   di_Roll = (ei_Roll - ei_Roll_alt)*20; //d-Anteil  
   if (di_Roll > 1.5)
	   di_Roll = 1.5;
   else if(di_Roll < -1.5)
	   di_Roll = -1.5;	  //Saturierung des D-Anteils

   aRoll = (pi_Roll + (ii_Roll+37)*0.02 + di_Roll)*567; //Ausgang des inneren Reglers		 */

	ea_Roll = (inRollSollLage) - (inRollIstLage+0.08*inRollIstV);  // Eingang in den �u�eren Regler
   if(inSchub > 0.1 && inSchub <=1 && inRollIstLage < 0.2 && inRollIstLage > -0.2)
   	ia_Roll = 0.01 * ea_Roll + ia_Roll;	//I-Anteil nur nahe der Nulllage ver�ndern  
   Soll_v_Roll = ea_Roll*2 + ia_Roll*0.02; //Sollgeschwindigkeit des �u�eren Reglers
   Soll_v_Roll = inRollSollLage;

   //innerer Regler
   v_Roll_tp1 = 0.95*v_Roll_tp1 + 0.05*inRollIstV; // Tiefpass-gefilterter Gyrowert
   ei_Roll= Soll_v_Roll - v_Roll_tp1;	//Eingang in die innere Regelung
   pi_Roll = ei_Roll * 0.1;	//p-Anteil
   if(inSchub > 0.1 && inSchub <=1 /*&& inRollIstLage < 0.2 && inRollIstLage > -0.2*/)
   	ii_Roll = 0.01 * ei_Roll + ii_Roll;	//I-Anteil nur nahe der Nulllage ver�ndern 


   di_Roll = (ei_Roll - ei_Roll_alt)*10; //d-Anteil  
   if (di_Roll > 1.5)
	   di_Roll = 1.5;
   else if(di_Roll < -1.5)
	   di_Roll = -1.5;	  //Saturierung des D-Anteils

   aRoll = (pi_Roll + (ii_Roll)*0.005 + di_Roll)*567; //Ausgang des inneren Reglers

	/////////////////////////// Yaw-Regler berechnen ////////////////////////////////////////// 

   ea_Yaw = (inYawSollLage) - (inYawIstLage+0.08*inYawIstV);  // Eingang in den �u�eren Regler
   if(inSchub > 0.1 && inSchub <=1 && inYawIstLage < 0.2 && inYawIstLage > -0.2)
   	ia_Yaw = 0.01 * ea_Yaw + ia_Yaw;	//I-Anteil nur nahe der Nulllage ver�ndern  
   Soll_v_Yaw = ea_Yaw*0.5 + ia_Yaw*0.002; //Sollgeschwindigkeit des �u�eren Reglers
   //Soll_v_Yaw = inYawSollLage;

   //innerer Regler
   v_Yaw_tp1 = 0.99*v_Yaw_tp1 + 0.01*inYawIstV; // Tiefpass-gefilterter Gyrowert
   ei_Yaw= Soll_v_Yaw - v_Yaw_tp1;	//Eingang in die innere Regelung
   pi_Yaw = ei_Yaw * 0.4;	//p-Anteil
   if(inSchub > 0.1 && inSchub <=1 && inYawIstLage < 0.2 && inYawIstLage > -0.2)
   	ii_Yaw = 0.01 * ei_Yaw + ii_Yaw;	//I-Anteil nur nahe der Nulllage ver�ndern 

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
   
	/////////////////////////// Motorwerte saturieren und �bergeben //////////////////////// 
   if (outMotor1 > 6800.F) 	setMotor_1(6800.F);
   else if(outMotor1 < 0.F) setMotor_1(0.F);
	 else setMotor_1(outMotor1);

   if (outMotor2 > 6800.F)  setMotor_2(6800.F);
   else if(outMotor2 < 0.F) setMotor_2(0.F);
	 else setMotor_2(outMotor2);
	 
   if (outMotor3 > 6800.F)  setMotor_3(6800.F);
   else if(outMotor3 < 0.F) setMotor_3(0.F);
	 else setMotor_3(outMotor3);
	 
   if (outMotor4 > 6800.F)  setMotor_4(6800.F);
   else if(outMotor4 < 0.F) setMotor_4(0.F);	
	 else setMotor_4(outMotor4);	

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
	//chThdSleepMilliseconds(10); /* Fixed interval.*/
    chThdSleepUntil(time);
  }
}






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

void i2c_scanner1(void)
{
   uint8_t x = 0, txbuf[2],rxbuf[6];
   //chprintf((BaseChannel *)&SD2,"inside i2c1 scanner");
   for(x=0;x<128;x++)
   {
      txbuf[0] = 0x00;
      txbuf[1] = 0x00;
      if(i2cMasterTransmit(&I2CD1, x, txbuf, 2, rxbuf, 0) == RDY_OK)chprintf((BaseChannel *)&SD2, "I2C1: Sensor is available on Address: 0x%x \r\n", x);
      chThdSleepMilliseconds(1);
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
	
	setup_IMU();
	setup_Fernsteuerung();
	setup_Motoren();


	//Regelungsthread anlegen
	//Thread *tp = chThdCreateFromHeap(NULL, THD_WA_SIZE(128), NORMALPRIO+1, Regelungsthread, NULL);
  //	if (tp == NULL)
  //  	chSysHalt();    /* Memory exausted. */
	chThdCreateStatic(RegelungThreadWorkingArea, sizeof(RegelungThreadWorkingArea), NORMALPRIO, Regelungsthread, NULL);
 
	/*
	* Normal main() thread activity, in this demo it does nothing except
	* sleeping in a loop and check the button state, when the button is
	* pressed the test procedure is launched with output on the serial
	* driver 2.
	*/

	while (TRUE) 
	{
  		
		//i2c_scanner1();
	    chThdSleepMilliseconds(10);
  }
}
