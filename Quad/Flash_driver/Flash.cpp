#include "Flash.h"

uint16_t VirtAddVarTab[number_flash_val] = {0,1,2,3,4,5,6,7,8,9,10,11,12,14,16,18,20,22,24,26,28,30,32,34,36,38,40,42,44,46,48,50,52,54,56,58,60,62,64,66,68,70,72,74,76,78,80,82,84,86,88,90,92,94,96};

void setup_FLASH()
{
	uint16_t tmp_eeprom_data_structure_version;
	FLASH_Unlock();
	EE_Init();
	EE_ReadVariable(VirtAddVarTab[index_eeprom_data_structure_version],&tmp_eeprom_data_structure_version);	//eeprom_data_structure_version lesen
	if (tmp_eeprom_data_structure_version != eeprom_data_structure_version)
	{
		EE_Format();
		EE_WriteVariable(VirtAddVarTab[index_eeprom_data_structure_version],tmp_eeprom_data_structure_version);
	}
}
