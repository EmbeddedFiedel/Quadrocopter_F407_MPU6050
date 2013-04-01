#include "stm32f4xx_flash.h"
#include "eeprom.h"


#define eeprom_data_structure_version 2
#define index_eeprom_data_structure_version 0

/*	Array-Indexnummer der Fernsteuerungswerte	*/


////////////////////Fernsteuerungs Kalibrierung/////////////////////
#define offset_flash_cal 1

#define index_rc_roll_max			offset_flash_cal+0
#define index_rc_roll_min 		offset_flash_cal+1
#define index_rc_roll_null		offset_flash_cal+2
#define index_rc_nick_max			offset_flash_cal+3
#define index_rc_nick_min			offset_flash_cal+4
#define index_rc_nick_null		offset_flash_cal+5
#define index_rc_yaw_max			offset_flash_cal+6
#define index_rc_yaw_min			offset_flash_cal+7
#define index_rc_yaw_null			offset_flash_cal+8
#define index_rc_schub_max		offset_flash_cal+9
#define index_rc_schub_null		offset_flash_cal+10
#define index_calibration_ready_flag offset_flash_cal+11 //Bit 0 (LSB)

#define number_flash_cal 12	//anzahl der Werte für die Kalibration

////////////////////Regler Parametrierung/////////////////////
#define offset_flash_parameter 13

#define index_N_pid_pa 				offset_flash_parameter+0
#define index_N_pid_da 				offset_flash_parameter+1
#define index_N_pid_ia_dyn 		offset_flash_parameter+2
#define index_N_pid_ia_gain 	offset_flash_parameter+3
#define index_N_pid_gain 			offset_flash_parameter+4
#define index_N_kas_pa 				offset_flash_parameter+5
#define index_N_kas_da 				offset_flash_parameter+6
#define index_N_kas_ia_dyn 		offset_flash_parameter+7
#define index_N_kas_ia_gain 	offset_flash_parameter+8
#define index_N_kas_pi 				offset_flash_parameter+9
#define index_N_kas_ii_dyn 		offset_flash_parameter+10
#define index_N_kas_ii_gain 	offset_flash_parameter+11
#define index_N_kas_gain 			offset_flash_parameter+12
#define index_N_sw_kas 				offset_flash_parameter+13
#define index_R_pid_pa 				offset_flash_parameter+14
#define index_R_pid_da 				offset_flash_parameter+15
#define index_R_pid_ia_dyn 		offset_flash_parameter+16
#define index_R_pid_ia_gain 	offset_flash_parameter+17
#define index_R_pid_gain 			offset_flash_parameter+18
#define index_R_kas_pa 				offset_flash_parameter+19
#define index_R_kas_da 				offset_flash_parameter+20
#define index_R_kas_ia_dyn 		offset_flash_parameter+21
#define index_R_kas_ia_gain 	offset_flash_parameter+22
#define index_R_kas_pi 				offset_flash_parameter+23
#define index_R_kas_ii_dyn 		offset_flash_parameter+24
#define index_R_kas_ii_gain 	offset_flash_parameter+25
#define index_R_kas_gain 			offset_flash_parameter+26
#define index_R_sw_kas 				offset_flash_parameter+27
#define index_Y_pid_pa 				offset_flash_parameter+28
#define index_Y_pid_da 				offset_flash_parameter+29
#define index_Y_pid_ia_dyn 		offset_flash_parameter+30
#define index_Y_pid_ia_gain 	offset_flash_parameter+31
#define index_Y_pid_gain 			offset_flash_parameter+32
#define index_Y_kas_pa 				offset_flash_parameter+33
#define index_Y_kas_da 				offset_flash_parameter+34
#define index_Y_kas_ia_dyn 		offset_flash_parameter+35
#define index_Y_kas_ia_gain 	offset_flash_parameter+36
#define index_Y_kas_pi 				offset_flash_parameter+37
#define index_Y_kas_ii_dyn 		offset_flash_parameter+38	
#define index_Y_kas_ii_gain 	offset_flash_parameter+39
#define index_Y_kas_gain 			offset_flash_parameter+40
#define index_Y_sw_kas 				offset_flash_parameter+41

#define number_flash_parameter 42


#define number_flash_val 	number_flash_parameter+number_flash_cal+1	//Gesamtanzahl der gespeicherten Werte


void setup_FLASH();
