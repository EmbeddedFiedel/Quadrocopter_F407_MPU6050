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
#include "ff.h"

	FATFS Fatfs;		/* File system object */
	FIL Fil;			/* File object */
	FRESULT rc;				/* Result code */
	DIR dir;				/* Directory object */
	FILINFO fno;			/* File information object */


FATFS MMC_FS;
MMCDriver MMCD1;
static bool_t fs_ready = FALSE;
static SPIConfig hs_spicfg = { NULL, GPIOC, 4, 0 };
static SPIConfig ls_spicfg = { NULL, GPIOC, 4, SPI_CR1_BR_2 | SPI_CR1_BR_1 };
/* Card insertion verification.*/
bool_t mmc_is_inserted(void)
{
//   return palReadPad(IOPORT3, GPIOC_MMCCP);
   return 1;
}

/* Card protection verification.*/
bool_t mmc_is_protected(void)
{
//   return !palReadPad(IOPORT3, GPIOC_MMCWP);
   return 0;
}
// MMC card insertion event
static void InsertHandler(eventid_t id)
{
  FRESULT err;
  (void) id;
	
  if(mmcConnect(&MMCD1))
  {
    chprintf((BaseChannel *) &SD2, "SD: Failed to connect to card\r\n");
    return;
  }
  else
  {
    chprintf((BaseChannel *) &SD2, "SD: Connected to card\r\n");
		
  }

  err = f_mount(0, &MMC_FS);
  if(err != FR_OK)
  {
    chprintf((BaseChannel *) &SD2, "SD: f_mount() failed %d\r\n", err);
    mmcDisconnect(&MMCD1);
    return;
  }
  else
  {
    chprintf((BaseChannel *) &SD2, "SD: File system mounted\r\n");
  }
  fs_ready = TRUE;

  // quick and dirty
  // directly decode all mp3's in the
  // cards root in a loop (assuming the card is never pulled)
  {
	rc = f_open(&Fil, "Quad_Handler.TXT", FA_WRITE | FA_CREATE_ALWAYS);
//	rc = f_printf(&Fil, "moin\r\n");	 
	rc = f_sync(&Fil);
  }
}

// MMC card removal event
static void RemoveHandler(eventid_t id)
{
  (void) id;
  fs_ready = FALSE;
}




/*
 * Application entry point.
 */
int main(void) 
{
	float nick, roll, yaw;
	
	
	static const evhandler_t evhndl[] = {InsertHandler, RemoveHandler};
  struct EventListener el0, el1;

	FRESULT err;
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
	
	setup_IMU();
	setup_Fernsteuerung();
	setup_Motoren();
	setup_Regelung();
 
   // initialize MMC driver
  // setup pads to SPI1 function (connect these pads to your SD card accordingly)
  palSetPadMode(GPIOC, 4, PAL_MODE_OUTPUT_PUSHPULL | PAL_STM32_OSPEED_HIGHEST); // NSS
  palSetPadMode(GPIOA, 5, PAL_MODE_ALTERNATE(5) | PAL_STM32_OSPEED_HIGHEST); // SCK
  palSetPadMode(GPIOA, 6, PAL_MODE_ALTERNATE(5)); // MISO
  palSetPadMode(GPIOA, 7, PAL_MODE_ALTERNATE(5) | PAL_STM32_OSPEED_HIGHEST); // MOSI
  palSetPad(GPIOC, 4); // set NSS high

  
	
  // initialize MMC driver
  mmcObjectInit(&MMCD1, &SPID1, &ls_spicfg, &hs_spicfg, mmc_is_protected, mmc_is_inserted);
  mmcStart(&MMCD1, NULL);
	chEvtRegister(&MMCD1.inserted_event, &el0, 0);
  chEvtRegister(&MMCD1.removed_event, &el1, 1);
	
chThdSleepMilliseconds(7000);
 if(mmcConnect(&MMCD1))
  {
    chprintf((BaseChannel *) &SD2, "SD: Failed to connect to card\r\n");
    return;
  }
  else
  {
    chprintf((BaseChannel *) &SD2, "SD: Connected to card\r\n");
		
  }

  err = f_mount(0, &MMC_FS);
  if(err != FR_OK)
  {
    chprintf((BaseChannel *) &SD2, "SD: f_mount() failed %d\r\n", err);
    mmcDisconnect(&MMCD1);
    return;
  }
  else
  {
    chprintf((BaseChannel *) &SD2, "SD: File system mounted\r\n");
  }
  fs_ready = TRUE;
	rc = f_open(&Fil, "Quad.TXT", FA_WRITE | FA_CREATE_ALWAYS);
	
	
	/*
	* Normal main() thread activity, in this demo it does nothing except
	* sleeping in a loop and check the button state, when the button is
	* pressed the test procedure is launched with output on the serial
	* driver 2.
	*/
	while (TRUE) 
	{
  		update_IMU();	 //Ersetzen durch Interrupt Handler!!!!!!
			nick = getEuler_nick();
			roll = getEuler_roll();
			yaw = getEuler_yaw();
			f_printf(&Fil, "%d;%d;%d\r\n",(int)(nick*100),(int)(roll*100),(int)(yaw*100));
			rc = f_sync(&Fil);
	    chThdSleepMilliseconds(10);
  }
}
