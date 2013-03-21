#include "Regelung.h"
#include "ch.h"
#include "hal.h"
#include "Lage.h"
#include "Fernsteuerung.h"
#include "Motoren.h"
#include "tm.h"

#include "ch.h"
#include "hal.h"
#include "chprintf.h"
#include "Datalogger.h"
#include "ff.h"
#include "GCS.h"

#include <string>
#include <sstream>
using namespace std;

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
int counter = 0;


float v_Nick_tp1 = 0;   	
float v_Nick_tp1_alt = 0;
float da_Nick = 0;
float pa_Nick = 0;
float ia_Nick = 0;
float ii_Nick = 0;
float pi_Nick = 0;
float ei_Nick = 0;
float ea_Nick_alt = 0;
float ea_Nick = 0;
float Soll_v_Nick = 0;
float aNick;
int	sw_kas_Nick = 0;
	 
float v_Roll_tp1 = 0;   	
float v_Roll_tp1_alt = 0;
float da_Roll = 0;
float pa_Roll = 0;
float ia_Roll = 0;
float ii_Roll = 0;
float pi_Roll = 0;
float ei_Roll = 0;
float ea_Roll_alt = 0;
float ea_Roll = 0;
float Soll_v_Roll = 0;
float aRoll;	
int	sw_kas_Roll = 0;

float v_Yaw_tp1 = 0;   	
float v_Yaw_tp1_alt = 0;
float da_Yaw = 0;
float pa_Yaw = 0;
float ia_Yaw = 0;
float ii_Yaw = 0;
float pi_Yaw = 0;
float ei_Yaw = 0;
float ea_Yaw_alt = 0;
float ea_Yaw = 0;
float Soll_v_Yaw = 0;
float aYaw;
int	sw_kas_Yaw = 0;

extern struct global_struct global_data;

uint32_t regelung_timebuffer[50];
int regelung_databuffer[50][39];
uint8_t readcounter = 0;
uint8_t writecounter = 0;
static FIL Fil_regelung;			/* File object */
FRESULT rc_datalog;				/* Result code */
	
bool_t datalog_regelung_opened = 0;
bool_t datalog_regelung_syncing = 0;
static TimeMeasurement regelungdatalogsync_tmup;
char fileName[20] = "Reg00.TXT";
/*
 * Working area for RegelungSyncThread
 */
static WORKING_AREA(RegelungSyncThreadWorkingArea, 2048);
/*
 * RegelungSyncThread
 */
static msg_t RegelungSyncthread(void *arg) 
{
  	while (TRUE) 
	{
		if(Datalogger_ready() && !datalog_regelung_opened)
		{
				rc_datalog = f_open(&Fil_regelung, fileName, FA_WRITE | FA_CREATE_NEW/* | FA_CREATE_ALWAYS*/);
				if(rc_datalog == FR_EXIST)
				{
					if(fileName[4] < 57)
					{
						fileName[4]++;
					}
					else
					{
						fileName[3]++;
						fileName[4] = 48;
						
					}
				}
				else if(rc_datalog != FR_OK)
				{
					chprintf((BaseChannel *) &SD2, "SD Quadregelung.TXT: f_open() failed %d\r\n", rc_datalog);
					chThdSleepMilliseconds(10);
				}
				else
				{
					f_printf(&Fil_regelung, "Time_regelung;inSchub;inNickSollLage;inRollSollLage;inYawSollLage;inNickIstLage;inRollIstLage;inYawIstLage;inNickIstV;inRollIstV;inYawIstV;pa_Nick;pa_Roll;pa_Yaw;ia_Nick;ia_Roll;ia_Yaw;Soll_v_Nick;Soll_v_Roll;Soll_v_Yaw;ii_Nick;ii_Roll;ii_Yaw;pi_Nick;pi_Roll;pi_Yaw;da_Nick;da_Roll;da_Yaw;v_Nick_tp1;v_Roll_tp1;v_Yaw_tp1;aNick;aRoll;aYaw;outMotor1;outMotor2;outMotor3;outMotor4;fifoCount\r\n");	
					rc_datalog = f_sync(&Fil_regelung);	
					if(rc_datalog != FR_OK)
					{
						chprintf((BaseChannel *) &SD2, "SD Quadregelung.TXT: initial f_sync() failed %d\r\n", rc_datalog);
						chThdSleepMilliseconds(10);
					}	
					else
					{
						chprintf((BaseChannel *) &SD2, "SD Quadregelung.TXT: opened successfull\r\n");
						chThdSleepMilliseconds(10);
						datalog_regelung_opened = TRUE;	
					}
				}
				chThdSleepMilliseconds(10);
		}
		else if (Datalogger_ready() && datalog_regelung_opened)
		{
			datalog_regelung_syncing = 1;
			rc_datalog = f_sync(&Fil_regelung);
			if(rc_datalog != FR_OK)
			{
				chprintf((BaseChannel *) &SD2, "SD Quadregelung.TXT: datalog f_sync() failed %d\r\n", rc_datalog);
			}	
			else
			{
				chprintf((BaseChannel *) &SD2, "SD Quadregelung.TXT: datalog f_sync() succeeded\r\n");
			}
			datalog_regelung_syncing = 0;
			chThdSleepMilliseconds(1000);
		}
		else
		{
			chThdSleepMilliseconds(10);
		}		
  	}	
}							 
/*
* Working area for LageSync
*/
static WORKING_AREA(RegelungPrintThreadWorkingArea, 2048);
/*
 * LageSync
 */
static msg_t RegelungPrintthread(void *arg) 
{
	while (TRUE) 
	{
		if(readcounter!=writecounter && Datalogger_ready() && datalog_regelung_opened && datalog_regelung_syncing==0)
		{
			systime_t time = chTimeNow();     // Tnow
			chprintf((BaseChannel *) &SD2, "Printing:%d\r\n",time);
			f_printf(&Fil_regelung, "%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d\r\n",
							regelung_timebuffer[readcounter],
							regelung_databuffer[readcounter][0],
							regelung_databuffer[readcounter][1],
							regelung_databuffer[readcounter][2],
							regelung_databuffer[readcounter][3],
							regelung_databuffer[readcounter][4],
							regelung_databuffer[readcounter][5],
							regelung_databuffer[readcounter][6],
							regelung_databuffer[readcounter][7],
							regelung_databuffer[readcounter][8],
							regelung_databuffer[readcounter][9],
              				regelung_databuffer[readcounter][10],
							regelung_databuffer[readcounter][11],
							regelung_databuffer[readcounter][12],
							regelung_databuffer[readcounter][13],
							regelung_databuffer[readcounter][14],
							regelung_databuffer[readcounter][15],
							regelung_databuffer[readcounter][16],
							regelung_databuffer[readcounter][17],
							regelung_databuffer[readcounter][18],
							regelung_databuffer[readcounter][19],
							regelung_databuffer[readcounter][20],
							regelung_databuffer[readcounter][21],
							regelung_databuffer[readcounter][22],
							regelung_databuffer[readcounter][23],
							regelung_databuffer[readcounter][24],
							regelung_databuffer[readcounter][25],
							regelung_databuffer[readcounter][26],
							regelung_databuffer[readcounter][27],
							regelung_databuffer[readcounter][28],
							regelung_databuffer[readcounter][29],
							regelung_databuffer[readcounter][30],
							regelung_databuffer[readcounter][31],
							regelung_databuffer[readcounter][32],
							regelung_databuffer[readcounter][33],
							regelung_databuffer[readcounter][34],
							regelung_databuffer[readcounter][35],
							regelung_databuffer[readcounter][36],
							regelung_databuffer[readcounter][37],
							regelung_databuffer[readcounter][38]);
							readcounter++;
							if(readcounter >=50)readcounter=0;
		}
		chThdSleepMilliseconds(2);
	}
}
 
void datalog_regelung(void)
{
	uint32_t system_time = chTimeNow();
	regelung_timebuffer[writecounter] = system_time;
	regelung_databuffer[writecounter][0]=(int)(inSchub*100);
	regelung_databuffer[writecounter][1]=(int)(inNickSollLage*100);
	regelung_databuffer[writecounter][2]=(int)(inRollSollLage*100);
	regelung_databuffer[writecounter][3]=(int)(inYawSollLage*100);
	regelung_databuffer[writecounter][4]=(int)(inNickIstLage*100);
	regelung_databuffer[writecounter][5]=(int)(inRollIstLage*100);
	regelung_databuffer[writecounter][6]=(int)(inYawIstLage*100);
	regelung_databuffer[writecounter][7]=(int)(inNickIstV*100);
	regelung_databuffer[writecounter][8]=(int)(inRollIstV*100);
	regelung_databuffer[writecounter][9]=(int)(inYawIstV*100);
	regelung_databuffer[writecounter][10]=(int)(pa_Nick*100);
	regelung_databuffer[writecounter][11]=(int)(pa_Roll*100);
	regelung_databuffer[writecounter][12]=(int)(pa_Yaw*100);
	regelung_databuffer[writecounter][13]=(int)(ia_Nick*100);
	regelung_databuffer[writecounter][14]=(int)(ia_Roll*100);
	regelung_databuffer[writecounter][15]=(int)(ia_Yaw*100);
	regelung_databuffer[writecounter][16]=(int)(Soll_v_Nick*100);
	regelung_databuffer[writecounter][17]=(int)(Soll_v_Roll*100);
	regelung_databuffer[writecounter][18]=(int)(Soll_v_Yaw*100);
	regelung_databuffer[writecounter][19]=(int)(ii_Nick*100);
	regelung_databuffer[writecounter][20]=(int)(ii_Roll*100);
	regelung_databuffer[writecounter][21]=(int)(ii_Yaw*100);
	regelung_databuffer[writecounter][22]=(int)(pi_Nick*100);
	regelung_databuffer[writecounter][23]=(int)(pi_Roll*100);
	regelung_databuffer[writecounter][24]=(int)(pi_Yaw*100);
	regelung_databuffer[writecounter][25]=(int)(da_Nick*100);
	regelung_databuffer[writecounter][26]=(int)(da_Roll*100);
	regelung_databuffer[writecounter][27]=(int)(da_Yaw*100);
	regelung_databuffer[writecounter][28]=(int)(v_Nick_tp1*100);
	regelung_databuffer[writecounter][29]=(int)(v_Roll_tp1*100);
	regelung_databuffer[writecounter][30]=(int)(v_Yaw_tp1*100);
	regelung_databuffer[writecounter][31]=(int)(aNick*100);
	regelung_databuffer[writecounter][32]=(int)(aRoll*100);
	regelung_databuffer[writecounter][33]=(int)(aYaw*100);
	regelung_databuffer[writecounter][34]=(int)(outMotor1);
	regelung_databuffer[writecounter][35]=(int)(outMotor2);
	regelung_databuffer[writecounter][36]=(int)(outMotor3);
	regelung_databuffer[writecounter][37]=(int)(outMotor4);
	regelung_databuffer[writecounter][38]=get_fifo_count();
	
	writecounter++;
	
	if(writecounter >=50)writecounter=0;
}
float SatFloat(float value,float min,float max)
{
		if (value > max)
			return(max);
		else if(value < min)
			return(min);
		else
			return(value);
}

float KaskadierterRegler(float in, float &gyro, float &pa, float &da, float &ia_dyn, float &ia_gain, float &pi, float &ii_dyn, float &ii_gain, float &out_gain, int achse)
{
	if(achse==1) //nick
	{ 
		ea_Nick = in;	//Eingang äußerer Regler
		   
		pa_Nick = ea_Nick * pa;					//P-Regler außen
		da_Nick = (ea_Nick - ea_Nick_alt)*da;	//D-Regler außen	
		if(inSchub > 0.2)
			ia_Nick = ia_dyn * ea_Nick + ia_Nick;		//I-Regler außen
	   
		Soll_v_Nick = (pa_Nick + ia_Nick*ia_gain + da_Nick); //Ausgang äußerer Regler
		ei_Nick = Soll_v_Nick-gyro; //Eingang innerer Regler

		pi_Nick = ei_Nick * pi;	//P-Regler innen 
		if(inSchub > 0.2)
			ii_Nick = SatFloat(ii_dyn * ei_Nick + ii_Nick, -15.0, 15.0);	//I-Regler innen 

		return((pi_Nick + ii_Nick*ii_gain)*out_gain); //Reglerausgang
	}
	else if(achse==2) //roll
	{
		ea_Roll = in;	//Eingang äußerer Regler
		   		 
		pa_Roll = ea_Roll * pa;					//P-Regler außen
		da_Roll = (ea_Roll - ea_Roll_alt)*da;	//D-Regler außen	
		if(inSchub > 0.2)
			ia_Roll = ia_dyn * ea_Roll + ia_Roll;		//I-Regler außen
	   
		Soll_v_Roll = (pa_Roll + ia_Roll*ia_gain + da_Roll); //Ausgang äußerer Regler
	   //ei_Roll = Soll_v_Roll - gyro;
		ei_Roll = inRollSollLage - gyro; //Eingang innerer Regler

		pi_Roll = ei_Roll * pi;	//P-Regler innen 
		if(inSchub > 0.2)
			ii_Roll = SatFloat(ii_dyn * ei_Roll + ii_Roll, -15.0, 15.0);	//I-Regler innen 

		return((pi_Roll + ii_Roll*ii_gain)*out_gain); //Reglerausgang
	}
	else if(achse==3) //yaw
	{
		ea_Yaw = in;	//Eingang äußerer Regler
		   		
		pa_Yaw = ea_Yaw * pa;					//P-Regler außen
		da_Yaw = (ea_Yaw - ea_Yaw_alt)*da;	//D-Regler außen	
		if(inSchub > 0.2)
			ia_Yaw = ia_dyn * ea_Yaw + ia_Yaw;		//I-Regler außen
	   
		Soll_v_Yaw = (pa_Yaw + ia_Yaw*ia_gain + da_Yaw); //Ausgang äußerer Regler
	   //ei_Yaw = Soll_v_Yaw - gyro;
		ei_Yaw = inYawSollLage - gyro; //Eingang innerer Regler

		pi_Yaw = ei_Yaw * pi;	//P-Regler innen 
		if(inSchub > 0.2)
			ii_Yaw = SatFloat(ii_dyn * ei_Yaw + ii_Yaw, -15.0, 15.0);	//I-Regler innen 

		return((pi_Yaw + ii_Roll*ii_gain)*out_gain); //Reglerausgang
	}

	return(0); 
}

float PIDRegler(float in, float &pa, float &da, float &ia_dyn, float &ia_gain, float &out_gain, int achse)
{
	if(achse==1) //nick
	{ 
		ea_Nick = in;	//Eingang äußerer Regler
		   	   
		pa_Nick = ea_Nick * pa;					//P-Regler außen
		da_Nick = (ea_Nick - ea_Nick_alt)*da;	//D-Regler außen	
		if(inSchub > 0.2)
			ia_Nick = SatFloat(ia_dyn * ea_Nick + ia_Nick,-5,5);		//I-Regler außen

		return((pa_Nick + ia_Nick*ia_gain + da_Nick)*out_gain); //Reglerausgang
	}
	else if(achse==2) //roll
	{ 
		ea_Roll = in;	//Eingang äußerer Regler
		   	   
		pa_Roll = ea_Roll * pa;					//P-Regler außen
		da_Roll = (ea_Roll - ea_Roll_alt)*da;	//D-Regler außen	
		if(inSchub > 0.2)
			ia_Roll = SatFloat(ia_dyn * ea_Roll + ia_Roll,-5,5);		//I-Regler außen

		return((pa_Roll + ia_Roll*ia_gain + da_Roll)*out_gain); //Reglerausgang		 
	}
	else if(achse==3) //yaw
	{ 
		ea_Yaw = in;	//Eingang äußerer Regler
		   	   
		pa_Yaw = ea_Yaw * pa;					//P-Regler außen
		da_Yaw = (ea_Yaw - ea_Yaw_alt)*da;	//D-Regler außen	
		if(inSchub > 0.2)
			ia_Yaw = SatFloat(ia_dyn * ea_Yaw + ia_Yaw,-5,5);		//I-Regler außen

		return((pa_Yaw + ia_Yaw*ia_gain + da_Yaw)*out_gain); //Reglerausgang		
	}

	return(0); 
}


void Regelung(void)
{
	/// Werte übernehmen

	inSchub = get_schub_soll();
	
	inNickSollLage = (2*get_euler_nick_soll())*0.05+0.95*inNickSollLage;
	inRollSollLage = (2*get_euler_roll_soll())*0.05+0.95*inRollSollLage;
	inYawSollLage  = 0;

	inYawIstLage = get_ypr_yaw_ist();
	inNickIstLage = get_ypr_nick_ist();
	inRollIstLage = get_ypr_roll_ist();
		 
	inRollIstV = get_rate_roll_ist();
	inNickIstV = -get_rate_nick_ist();
	inYawIstV = get_rate_yaw_ist();

	/////////////////////////// Nick-Regler berechnen //////////////////////////////////////////

	int offset_Param=0;

	if(global_data.param[13+offset_Param])
	{
		aNick = KaskadierterRegler((inNickSollLage-inNickIstLage),inNickIstV,global_data.param[5+offset_Param],global_data.param[6+offset_Param],global_data.param[7+offset_Param],global_data.param[8+offset_Param],global_data.param[9+offset_Param],global_data.param[10+offset_Param],global_data.param[11+offset_Param],global_data.param[12+offset_Param],1);
	}
	else
	{
		aNick = PIDRegler((inNickSollLage-inNickIstLage),global_data.param[0+offset_Param],global_data.param[1+offset_Param],global_data.param[2+offset_Param],global_data.param[3+offset_Param],global_data.param[4+offset_Param],1);
	}
	/////////////////////////// Roll-Regler berechnen ////////////////////////////////////////// 
	
	offset_Param=14;

	if(global_data.param[13+offset_Param])
	{
		aRoll = KaskadierterRegler((inRollSollLage-inRollIstLage),inRollIstV,global_data.param[5+offset_Param],global_data.param[6+offset_Param],global_data.param[7+offset_Param],global_data.param[8+offset_Param],global_data.param[9+offset_Param],global_data.param[10+offset_Param],global_data.param[11+offset_Param],global_data.param[12+offset_Param],2);
	}
	else
	{
		aRoll = PIDRegler((inRollSollLage-inRollIstLage),global_data.param[0+offset_Param],global_data.param[1+offset_Param],global_data.param[2+offset_Param],global_data.param[3+offset_Param],global_data.param[4+offset_Param],2);
	} 

	/////////////////////////// Yaw-Regler berechnen ////////////////////////////////////////// 
	
	offset_Param=28;

	if(global_data.param[13+offset_Param])
	{
		aYaw = KaskadierterRegler((inRollSollLage-inRollIstLage),inYawIstV,global_data.param[5+offset_Param],global_data.param[6+offset_Param],global_data.param[7+offset_Param],global_data.param[8+offset_Param],global_data.param[9+offset_Param],global_data.param[10+offset_Param],global_data.param[11+offset_Param],global_data.param[12+offset_Param],3);
	}
	else
	{
		aYaw = PIDRegler((inRollSollLage-inRollIstLage),global_data.param[0+offset_Param],global_data.param[1+offset_Param],global_data.param[2+offset_Param],global_data.param[3+offset_Param],global_data.param[4+offset_Param],3);
	}
  	
	/////////////////////////// Motorwerte setzen //////////////////////////////////////////  
   if(inSchub > 0.1 && inSchub <=1)
   {
     Schub_Offset = 6000 * inSchub;
	 aYaw = 0.F;

     outMotor1 = Schub_Offset + aNick - aYaw;
     outMotor4 = Schub_Offset - aNick - aYaw; 

     outMotor2 = 0;//Schub_Offset + aRoll + aYaw;	  
     outMotor3 = 0;//Schub_Offset - aRoll + aYaw;
   }
   else 
   {					
  		outMotor1 = 0.F;
  		outMotor2 = 0.F;
		outMotor3 = 0.F;
		outMotor4 = 0.F;
   }

	/////////////////////////// Motorwerte saturieren und ï¿½bergeben //////////////////////// 
   if (outMotor1 > 6800.F) 	setMotor_1(1.F);
   else if(outMotor1 < 0.F) setMotor_1(0.F);
   else setMotor_1(outMotor1/6800);

   if (outMotor2 > 6800.F)  setMotor_2(1.F);
   else if(outMotor2 < 0.F) setMotor_2(0.F);
   else setMotor_2(outMotor2/6800);
	 
   if (outMotor3 > 6800.F)  setMotor_3(1.F);
   else if(outMotor3 < 0.F) setMotor_3(0.F);
   else setMotor_3(outMotor3/6800);
	 
   if (outMotor4 > 6800.F)  setMotor_4(1.F);
   else if(outMotor4 < 0.F) setMotor_4(0.F);	
   else setMotor_4(outMotor4/6800);	

	/////////////////////////// Alte Werte merken ////////////////////////////////////////// 

   v_Nick_tp1_alt = v_Nick_tp1;
   ea_Nick_alt = ea_Nick;

   v_Roll_tp1_alt = v_Roll_tp1;
   ea_Roll_alt = ea_Roll;

   v_Yaw_tp1_alt = v_Yaw_tp1;
   ea_Yaw_alt = ea_Yaw;

   datalog_regelung();
}
/*
 * Working area for Regelung
 */
static WORKING_AREA(RegelungThreadWorkingArea, 2048);
//int dennis=0; 
/*
 * Regelungsthread
 */
systime_t delta_t;
static msg_t Regelungsthread(void *arg) {
 
  systime_t time = chTimeNow();     // Tnow
  while (TRUE) 
	{
		delta_t = MS2ST(10);
		time += delta_t;            // Next deadline
		Regelung();
		chThdSleepUntil(time);
  }
}




void setup_Regelung()
{
	chThdCreateStatic(RegelungSyncThreadWorkingArea, sizeof(RegelungSyncThreadWorkingArea), HIGHPRIO, RegelungSyncthread, NULL);
	chThdCreateStatic(RegelungPrintThreadWorkingArea, sizeof(RegelungPrintThreadWorkingArea), NORMALPRIO, RegelungPrintthread, NULL);
	chThdCreateStatic(RegelungThreadWorkingArea, sizeof(RegelungThreadWorkingArea), ABSPRIO, Regelungsthread, NULL);
}
