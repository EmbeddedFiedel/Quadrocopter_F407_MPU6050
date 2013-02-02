/*
This file is part of Quadrocopter_F407_MPU6050 project.

TrunetCopter program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Fernsteuerung.h"
#include "chprintf.h"
#include "GCS.h"

#define RC_IN_RANGE(x) (((x)>900 && (x)<2300))

/**********************************
***															***
***	Defines für Kalibrierung		***
***															***
**********************************/
#define max_roll 	0.5
#define max_nick 	0.5
#define max_yaw 	0.5
#define max_schub 1.0


short RC_INPUT_CHANNELS_Offset[4] = {-1500,-1500,-1100,-1500};
volatile unsigned short RC_INPUT_CHANNELS[4], RC_INPUT_LAST_TCNT,tmp=0;
char PPM_FRAME_GOOD = 1;
static bool_t Fernsteuerung_ready_flag = FALSE;

/**********************************
***															***
***	Variablen für Kalibrierung	***
***															***
**********************************/
uint16_t rc_roll_max=2000, rc_roll_min=1000, rc_roll_null=1500;
uint16_t rc_nick_max=2000, rc_nick_min=1000, rc_nick_null=1500;
uint16_t rc_yaw_max=2000, rc_yaw_min=1000, rc_yaw_null=1500;
uint16_t rc_schub_max=2000, rc_schub_null=1000;
uint16_t first_visit_roll = 1,  first_visit_nick=1, first_visit_yaw=1,first_visit_schub=1;
uint16_t calibration_active=0;
uint16_t timer_finish=1;

static struct VirtualTimer vt;
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


//DEBUGGING
unsigned short tmp_dbg;

void rx_channel1_interrupt(EXTDriver *extp, expchannel_t channel) {
		(void)extp;
		(void)channel;

		chSysLockFromIsr();
		if (palReadPad(GPIOE, 10) == PAL_LOW) {
			tmp_dbg = TIM4->CNT - RC_INPUT_LAST_TCNT;
			if (RC_IN_RANGE(tmp_dbg)) 
			{	
				RC_INPUT_CHANNELS[0] = tmp_dbg;
				if (calibration_active)
				{
					if (first_visit_roll)
					{
						rc_roll_null=RC_INPUT_CHANNELS[0];
						rc_roll_min=RC_INPUT_CHANNELS[0];
						rc_roll_max=RC_INPUT_CHANNELS[0];
						first_visit_roll=0;
					}
					else
					{
						rc_roll_min = RC_INPUT_CHANNELS[0] < rc_roll_min ? RC_INPUT_CHANNELS[0] : rc_roll_min;
						rc_roll_max = RC_INPUT_CHANNELS[0] > rc_roll_max ? RC_INPUT_CHANNELS[0] : rc_roll_max;
					}
					
				}
			}
		}
		RC_INPUT_LAST_TCNT = TIM4->CNT;
		chSysUnlockFromIsr();
}

void rx_channel2_interrupt(EXTDriver *extp, expchannel_t channel) {
		(void)extp;
		(void)channel;

		chSysLockFromIsr();
		if (palReadPad(GPIOE, 11) == PAL_LOW) {
			unsigned short tmp = TIM4->CNT - RC_INPUT_LAST_TCNT;
			if (RC_IN_RANGE(tmp)) 
			{
				RC_INPUT_CHANNELS[1] = tmp;
				if (calibration_active)
				{
					if (first_visit_nick)
					{
						rc_nick_null=RC_INPUT_CHANNELS[1];
						rc_nick_min=RC_INPUT_CHANNELS[1];
						rc_nick_max=RC_INPUT_CHANNELS[1];
						first_visit_nick=0;
					}
					else
					{
						rc_nick_min = RC_INPUT_CHANNELS[1] < rc_nick_min ? RC_INPUT_CHANNELS[1] : rc_nick_min;
						rc_nick_max = RC_INPUT_CHANNELS[1] > rc_nick_max ? RC_INPUT_CHANNELS[1] : rc_nick_max;
					}
					
				}
			}
		}
		RC_INPUT_LAST_TCNT = TIM4->CNT;
		chSysUnlockFromIsr(); 		  
}
void rx_channel3_interrupt(EXTDriver *extp, expchannel_t channel) {
		(void)extp;
		(void)channel;

		chSysLockFromIsr();
		if (palReadPad(GPIOE, 12) == PAL_LOW) {
			unsigned short tmp = TIM4->CNT - RC_INPUT_LAST_TCNT;
			if (RC_IN_RANGE(tmp)) 
			{
				RC_INPUT_CHANNELS[2] = tmp;
				if (calibration_active)
				{
					if (first_visit_schub)
					{
						rc_schub_null=RC_INPUT_CHANNELS[2];
						rc_schub_max=RC_INPUT_CHANNELS[2];
						first_visit_schub=0;
					}
					else
					{
						rc_schub_null = RC_INPUT_CHANNELS[2] < rc_schub_null ? RC_INPUT_CHANNELS[2] : rc_schub_null;
						rc_schub_max = RC_INPUT_CHANNELS[2] > rc_schub_max ? RC_INPUT_CHANNELS[2] : rc_schub_max;
					}
				}
			}
		}
		RC_INPUT_LAST_TCNT = TIM4->CNT;
		chSysUnlockFromIsr(); 
}
void rx_channel4_interrupt(EXTDriver *extp, expchannel_t channel) {
		(void)extp;
		(void)channel;

		chSysLockFromIsr();
		if (palReadPad(GPIOE, 13) == PAL_LOW) {
			unsigned short tmp = TIM4->CNT - RC_INPUT_LAST_TCNT;
			if (RC_IN_RANGE(tmp)) 
			{
				RC_INPUT_CHANNELS[3] = tmp;
				if (calibration_active)
				{
					if (first_visit_yaw)
					{
						rc_yaw_null=RC_INPUT_CHANNELS[3];
						rc_yaw_min=RC_INPUT_CHANNELS[3];
						rc_yaw_max=RC_INPUT_CHANNELS[3];
						first_visit_yaw=0;
					}
					else
					{
						rc_yaw_min = RC_INPUT_CHANNELS[3] < rc_yaw_min ? RC_INPUT_CHANNELS[3] : rc_yaw_min;
						rc_yaw_max = RC_INPUT_CHANNELS[3] > rc_yaw_max ? RC_INPUT_CHANNELS[3] : rc_yaw_max;
					}
					
				}
			}
		}
		RC_INPUT_LAST_TCNT = TIM4->CNT;
		chSysUnlockFromIsr();	  
}

// ================================================================
// ===                      INITIAL SETUP                       ===
// ================================================================

void setup_Fernsteuerung() 
{
	/*
	* Enable Timer 4
	*/

	RC_INPUT_CHANNELS[0] = -RC_INPUT_CHANNELS_Offset[0]; 
	RC_INPUT_CHANNELS[1] = -RC_INPUT_CHANNELS_Offset[1];
	RC_INPUT_CHANNELS[2] = -RC_INPUT_CHANNELS_Offset[2];
	RC_INPUT_CHANNELS[3] = -RC_INPUT_CHANNELS_Offset[3];

	TIM4->CR1 = 0x0000;
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
	TIM4->SMCR = 0; // slave mode disabled
	TIM4->PSC = 84; // 84 mhz maximum apb1 bus speed
	TIM4->ARR = 0xffff;
	TIM4->SR = 0;
	TIM4->DIER = 0;
	TIM4->CR1 = 0x0001;
	if(EXTD1.state == EXT_ACTIVE)
	{
    //chprintf((BaseChannel *) &SD2, "Fernsteuerung Init erfolgreich\r\n");
		
	send_statustext(MAV_SEVERITY_ALERT, "Fernsteuerung Initialization finished");
		Fernsteuerung_ready_flag = TRUE;
	}
	else
	{
    //chprintf((BaseChannel *) &SD2, "Fernsteuerung Init failed, ExtInt nicht konfiguriert\r\n");
	}
	/*
	* Einstellungen fuer Kalibration
	*/
}

int16_t get_chan1_scaled()
{
    return ((int16_t)((RC_INPUT_CHANNELS[0] + RC_INPUT_CHANNELS_Offset[0])*20));
}
int16_t get_chan2_scaled()
{
    return ((int16_t)((RC_INPUT_CHANNELS[1] + RC_INPUT_CHANNELS_Offset[1])*20));
}
int16_t get_chan3_scaled()
{
    return ((int16_t)((RC_INPUT_CHANNELS[2] + RC_INPUT_CHANNELS_Offset[2])*10));
}
int16_t get_chan4_scaled()
{
    return ((int16_t)((RC_INPUT_CHANNELS[3] + RC_INPUT_CHANNELS_Offset[3])*20));
}

float get_euler_nick_soll() 
{
	if(Fernsteuerung_ready_flag && !calibration_active) 
		return RC_INPUT_CHANNELS[1] > rc_nick_null ? max_nick/(rc_nick_max-rc_nick_null)*(RC_INPUT_CHANNELS[1]-rc_nick_null) :  \
		max_nick/(rc_nick_null-rc_nick_min)*(RC_INPUT_CHANNELS[1]-rc_nick_null) ;
	else return 0;
}
float get_euler_roll_soll() 
{
	if(Fernsteuerung_ready_flag && !calibration_active) 
		return RC_INPUT_CHANNELS[0] > rc_roll_null ? max_roll/(rc_roll_max-rc_roll_null)*(RC_INPUT_CHANNELS[0]-rc_roll_null) :  \
		max_roll/(rc_roll_null-rc_roll_min)*(RC_INPUT_CHANNELS[0]-rc_roll_null) ;
	else return 0;
}
float get_schub_soll() 
{
	if(Fernsteuerung_ready_flag && !calibration_active) 
		return  max_schub/(rc_schub_max-rc_schub_null)*(RC_INPUT_CHANNELS[2]-rc_schub_null);
	else return 0;
}
float get_euler_yaw_soll() 
{
	if(Fernsteuerung_ready_flag && !calibration_active) 
		return RC_INPUT_CHANNELS[3] > rc_yaw_null ? max_yaw/(rc_yaw_max-rc_yaw_null)*(RC_INPUT_CHANNELS[3]-rc_yaw_null) :  \
		max_yaw/(rc_yaw_null-rc_yaw_min)*(RC_INPUT_CHANNELS[3]-rc_yaw_null) ;
	else return 0;
}


/******************************************************************
***																															***
***									Kalibrationsroutine													***
***					-setzt und loescht Kalibrationsmodus-								***
******************************************************************/
void start_calib()
{
					palSetPad(GPIOD, GPIOD_LED4);
					palSetPad(GPIOD, GPIOD_LED5);
					palSetPad(GPIOD, GPIOD_LED6);
					calibration_active = 1;
					first_visit_roll=1;
					first_visit_nick=1;
					first_visit_yaw=1;
					first_visit_schub=1;	
}
void stop_calib()
{
					palClearPad(GPIOD, GPIOD_LED4);
					palClearPad(GPIOD, GPIOD_LED5);
					palClearPad(GPIOD, GPIOD_LED6);
					calibration_active = 0;
}

void calib_interrupt(EXTDriver *extp, expchannel_t channel)
{
	static uint16_t on_off = 0;
	if (timer_finish)
	{
				if (on_off)			//Kalibration aus
				{
					palClearPad(GPIOD, GPIOD_LED4);
					palClearPad(GPIOD, GPIOD_LED5);
					palClearPad(GPIOD, GPIOD_LED6);
					calibration_active = 0;
				}
				else 						//Kalibration ein
				{
					palSetPad(GPIOD, GPIOD_LED4);
					palSetPad(GPIOD, GPIOD_LED5);
					palSetPad(GPIOD, GPIOD_LED6);
					calibration_active = 1;
					first_visit_roll=1;
					first_visit_nick=1;
					first_visit_yaw=1;
					first_visit_schub=1;
				}
				on_off = on_off== 1 ? 0 : 1 ;	
				timer_finish=0;
				chVTSetI(&vt,MS2ST(700),timer_handler,0);
	}
}


/******************************************************************
***																															***
***									Timerroutine																***
***					-setzt Variable timer_finish-												***
******************************************************************/

void timer_handler(void *arg)
{
	timer_finish=1;
}

