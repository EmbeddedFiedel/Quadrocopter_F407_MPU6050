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
#include "Regelung.h"
#include "Datalogger.h"
#include "ExternalInterrupt.h"
#include "stm32f4xx_flash.h"

float dummy1,dummy2,dummy3,dummy4;


//   Variablen für Flashtest
uint8_t schreiben=2;
uint32_t flash_data=0xF00FF00F;
unsigned int *flash_pointer;
FLASH_Status status_erase,status_prog=(FLASH_Status)0xFFFF;
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
	
	/*
	* Activates the serial driver 2 using the driver default configuration.
	* PA2(TX) and PA3(RX) are routed to USART2.
	*/
	sdStart(&SD2, NULL);
	palSetPadMode(GPIOD, 5, PAL_MODE_ALTERNATE(7));
	palSetPadMode(GPIOD, 6, PAL_MODE_ALTERNATE(7));
	
	//setup_IMU();
	setup_ExternalInterrupt();
	setup_Fernsteuerung();

	//setup_Motoren();
	//setup_Regelung();
	setup_Datalogger(); 
    palSetPad(GPIOD, GPIOD_LED3);       /* Orange.  */

	


	/*
	* Normal main() thread activity, in this demo it does nothing except
	* sleeping in a loop and check the button state, when the button is
	* pressed the test procedure is launched with output on the serial
	* driver 2.
	*/
	
	FLASH_SetLatency(FLASH_Latency_5); // 168 MHZ und 3V
	FLASH_Unlock();
	flash_pointer=(unsigned int*)0x80FFF00;
	//FLASH_EraseSector(FLASH_Sector_0,VoltageRange_3);
	//FLASH_ProgramWord(0x80000000,0xAFFEAFFE);
	
	while (TRUE) 
	{
		if (1==schreiben)
		{
			//status_erase=FLASH_EraseSector(FLASH_Sector_11,VoltageRange_3);
			status_prog=FLASH_ProgramWord(0x80FFF00,0xFFFFFFFF);
			schreiben=2;
		}
		if (0==schreiben)
		{
			flash_data= *flash_pointer;
			schreiben=2;
		} 
		
		dummy1=get_euler_roll_soll();
		dummy2=get_euler_nick_soll();
		dummy3=get_euler_yaw_soll();
		dummy4=get_schub_soll();
		//update_IMU();
		//datalog();
		//chThdSleepMilliseconds(10);
  }
}
