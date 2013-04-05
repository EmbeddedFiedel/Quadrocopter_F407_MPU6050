#include "stm32f4xx_flash.h"
#include "eeprom.h"





/*	Array-Indexnummer der Fernsteuerungswerte	*/

#define	rc_roll_max			0
#define	rc_roll_min 		1
#define	rc_roll_null		2
#define	rc_nick_max			3
#define	rc_nick_min			4
#define	rc_nick_null		5
#define	rc_yaw_max			6
#define	rc_yaw_min			7
#define	rc_yaw_null			8
#define	rc_schub_max		9
#define	rc_schub_null		10
	
#define	calibration_ready_flag 11 //Bit 0 (LSB)

#define number_flash_val 14		//Gesamtanzahl der gespeicherten Werte
#define number_flash_cal 12	//anzahl der Werte für die Kalibration

extern uint16_t VirtAddVarTab[number_flash_val];

void setup_FLASH();

uint8_t FLASH_Write_Word(uint32_t Address, uint32_t Data);

uint8_t FLASH_Write_2Bytes(uint32_t Address, uint16_t Data);

uint8_t FLASH_Read_2Bytes(uint32_t Address, uint16_t *Data);

