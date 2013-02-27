#include "stm32f4xx_flash.h"



/*
**		!!!!!!		Achtung			!!!!!!
**				
**		Bei Änderung der Größe des Daten-Flashspeichers (Flash_size) muss
**		der ROM-Bereich in den Projekteinstellungen entsprechend 
**		verkleinert werden
*/
#define Flash_size	256

#define Flash_Offset 0x8100000- Flash_size

/*	Adressen der Fernsteuerungswerte als Offset von Flash_Offset	*/

#define	FLASH_nick_max		0x0A
#define	FLASH_nick_min		0x0C
#define	FLASH_nick_null		0x0E
#define	FLASH_yaw_max			0x10
#define	FLASH_yaw_min			0x12
#define	FLASH_yaw_null		0x14
#define	FLASH_schub_max		0x16
#define	FLASH_schub_null	0x18
	
#define	FLASH_calibration_ready_flag 0x1C //Bit 0 (LSB)

#define	FLASH_roll_max		0x20
#define	FLASH_roll_min 		0x22
#define	FLASH_roll_null		0x24



void setup_FLASH();

uint8_t FLASH_Write_Word(uint32_t Address, uint32_t Data);

uint8_t FLASH_Write_2Bytes(uint32_t Address, uint16_t Data);

uint8_t FLASH_Read_2Bytes(uint32_t Address, uint16_t *Data);

