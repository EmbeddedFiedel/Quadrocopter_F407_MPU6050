#include "Datalogger.h"
#include "ch.h"
#include "hal.h"
#include "test.h"
#include "chprintf.h"
#include "ff.h"

	FATFS MMC_FS;		/* File system object */
	DIR dir;				/* Directory object */
	FILINFO fno;			/* File information object */
	MMCDriver MMCD1;




static bool_t Datalogger_ready_flag = FALSE;
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
	Datalogger_ready_flag = TRUE;
	return;
}

// MMC card removal event
static void RemoveHandler(eventid_t id)
{
  (void) id;
  Datalogger_ready_flag = FALSE;
}


/*
 * Working area for chEvtDispatchThread
 */
static WORKING_AREA(chEvtDispatchThreadWorkingArea, 256);
/*
 * chEvtDispatchThread
 */
static msg_t chEvtDispatchThread(void *arg) 
{ 
	static const evhandler_t evhndl[] = {InsertHandler, RemoveHandler};
  struct EventListener el0, el1;
  // initialize MMC driver
  mmcObjectInit(&MMCD1, &SPID1, &ls_spicfg, &hs_spicfg, mmc_is_protected, mmc_is_inserted);
  mmcStart(&MMCD1, NULL);
	chEvtRegister(&MMCD1.inserted_event, &el0, 0);
  chEvtRegister(&MMCD1.removed_event, &el1, 1);
	while (TRUE) 
	{
		chEvtDispatch(evhndl, chEvtWaitOne(ALL_EVENTS));
	}
}

void setup_Datalogger(void)
{
	
	// initialize MMC driver
  // setup pads to SPI1 function (connect these pads to your SD card accordingly)
  palSetPadMode(GPIOC, 4, PAL_MODE_OUTPUT_PUSHPULL | PAL_STM32_OSPEED_HIGHEST); // NSS
  palSetPadMode(GPIOA, 5, PAL_MODE_ALTERNATE(5) | PAL_STM32_OSPEED_HIGHEST); // SCK
  palSetPadMode(GPIOA, 6, PAL_MODE_ALTERNATE(5)); // MISO
  palSetPadMode(GPIOA, 7, PAL_MODE_ALTERNATE(5) | PAL_STM32_OSPEED_HIGHEST); // MOSI
  palSetPad(GPIOC, 4); // set NSS high
	
	//chThdSleepMilliseconds(7000);
	chThdCreateStatic(chEvtDispatchThreadWorkingArea, sizeof(chEvtDispatchThreadWorkingArea), NORMALPRIO, chEvtDispatchThread, NULL);
	return;
}

bool Datalogger_ready(void)
{
	return Datalogger_ready_flag;
}