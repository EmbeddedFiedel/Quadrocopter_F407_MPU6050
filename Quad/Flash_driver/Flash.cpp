#include "Flash.h"


void setup_FLASH()
{
	FLASH_SetLatency(FLASH_Latency_5); // 168 MHZ und 3V
	FLASH_Unlock();
}


/*
**	Parameter:		
**						Address		Adresse zwischen 0 und Flash_size-4
**						Data			zu schreibende Daten
**	Rückgabewerte:
**								0 - erfolgreich
**								1 - Flash Fehler
**								2 - falsche Adresse
*/
uint8_t FLASH_Write_Word(uint32_t Address, uint32_t Data)		//Schreibe 32Bit-Wort
{
	if (Address >= 0 && Address <= Flash_size-4)
		if (FLASH_ProgramWord(Flash_Offset + Address, Data)==FLASH_COMPLETE)
			return 0;
		else return 1;
	else return 2;
}


/*
**	Parameter:		
**						Address		Adresse zwischen 0 und Flash_size-2
**						Data			zu schreibende Daten
**	Rückgabewerte:
**								0 - erfolgreich
**								1 - Flash Fehler
**								2 - falsche Adresse
*/
uint8_t FLASH_Write_2Bytes(uint32_t Address, uint16_t Data)	//Schreibe 16Bit
{
	if (Address >= 0 && Address <= Flash_size-2)
		if (FLASH_ProgramHalfWord(Flash_Offset + Address, Data) == FLASH_COMPLETE)
			return 0;
		else return 1;
	else return  2;
}

/*
**Parameter:
**					Address:	zu lesende Adresse zwischen 0 und Flash_size-2
**					*Data:		Zeiger auf eine Variable, in der die Daten gespeichert werden
**Rückgabe:	
**					0: Erfolg
**					1: falsche Adresse
*/
uint16_t *data_pointer;
uint8_t FLASH_Read_2Bytes(uint32_t Address, uint16_t *Data)
{
	
	if (Address >= 0 && Address <= Flash_size-2)
	{
		data_pointer = (uint16_t*) (Address + (uint32_t)Flash_Offset) ;
		*Data = *data_pointer;
		return 0;
	}
	else return 1;
}