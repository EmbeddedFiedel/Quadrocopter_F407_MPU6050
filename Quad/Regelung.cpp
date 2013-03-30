#include "Regelung.h"
#include "regelung_x.h"
#include "ch.h"
#include "hal.h"
#include "Lage.h"
#include "Fernsteuerung.h"
#include "Motoren.h"
#include "tm.h"
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
float di_Nick = 0;
float pi_Nick = 0;
float ii_Nick = -10;
float ia_Nick = 0;
float pa_Nick = 0;
float ei_Nick = 0;
float ei_Nick_alt = 0;
float ea_Nick = 0;
float Soll_v_Nick = 0;
float aNick;

float v_Roll_tp1 = 0;   	
float v_Roll_tp1_alt = 0;
float di_Roll = 0;
float pi_Roll = 0;
float ii_Roll = 0;
float ia_Roll = 0;
float pa_Roll = 0;
float ei_Roll = 0;
float ei_Roll_alt = 0;
float ea_Roll = 0;
float Soll_v_Roll = 0;
float aRoll; 

float v_Yaw_tp1 = 0;   	
float v_Yaw_tp1_alt = 0;
float di_Yaw = 0;
float pi_Yaw = 0;
float ii_Yaw = 0;
float ia_Yaw = 0;
float pa_Yaw = 0;
float ei_Yaw = 0;
float ei_Yaw_alt = 0;
float ea_Yaw = 0;
float Soll_v_Yaw = 0;
float aYaw;
volatile unsigned short tmp111;

#include "ch.h"
#include "hal.h"
#include "chprintf.h"
#include "Datalogger.h"
#include "ff.h"

	uint32_t regelung_timebuffer[50];
	int regelung_databuffer[50][52];
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
	//int filecounter = 0;	
	//stringstream ss;

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
					f_printf(&Fil_regelung, "Parameter: kp_a_nick = %d, kp_a_roll = %d, kp_nick = %d, kp_roll = %d, kp_v_nick = %d, kp_v_roll = %d, tn_nick = %d, tn_roll = %d, tv_nick = %d, tv_roll = %d \r\nTime_regelung;In_Throttle;In_Soll_Nick;In_Soll_Roll;In_Soll_Gier;In_Ist_Nick;In_Ist_Roll;In_Ist_Gier;In_Ist_V_Nick;In_Ist_V_Roll;Out_M_Roll;Out_M_Nick;Out_M_Gier;Out_F_A;Out_F_B;Out_F_C;Out_F_D;Out_n_A;Out_n_B;Out_n_C;Out_n_D;Soll_V_Roll;Xd_Roll;p_anteil;i_anteil;d_anteil;Y_roll;Xd_V_Roll;Ist_a_Roll;Xd_a_Roll;Soll_V_Nick;Soll_a_Nick;Xd_Nick;Y_Nick;Ist_a_Nick;Xd_a_Nick;get_ypr_nick_ist;get_ypr_roll_ist;get_ypr_yaw_ist;Soll_a_Roll;p_anteil_nick;i_anteil_nick;d_anteil_nick;fifoCount;Ist_hoehe;Ist_beschleunigung_z;Ist_clac_hoehe;integrator_rst;Out_Schubkraft;th_dt;Inti_rdy;I_offset_Roll_;I_offset_nick_\r\n", (long int)(kp_a_nick*10000) , (long int)(kp_a_roll*10000), (long int)(kp_nick*10000), (long int)(kp_roll*10000), (long int)(kp_v_nick*10000), (long int)(kp_v_roll*10000), (long int)(tn_nick*10000), (long int)(tn_roll*10000),(long int)(tv_nick*10000),(long int)(tv_roll*10000));
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
			f_printf(&Fil_regelung, "%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d\r\n",
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
							regelung_databuffer[readcounter][38],
							regelung_databuffer[readcounter][39],
							regelung_databuffer[readcounter][40],
							regelung_databuffer[readcounter][41],
							regelung_databuffer[readcounter][42],
							regelung_databuffer[readcounter][43],
							regelung_databuffer[readcounter][44],
							regelung_databuffer[readcounter][45],
							regelung_databuffer[readcounter][46],
							regelung_databuffer[readcounter][47],
							regelung_databuffer[readcounter][48],
							regelung_databuffer[readcounter][49],
							regelung_databuffer[readcounter][50],
							regelung_databuffer[readcounter][51]);
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
	regelung_databuffer[writecounter][0]=(long int)(Schubverteilung0_U.In_Throttle*10000);
	regelung_databuffer[writecounter][1]=(long int)(Regelglied_U.In_Soll_Nick*10000);
	regelung_databuffer[writecounter][2]=(long int)(Regelglied_U.In_Soll_Roll*10000);
	regelung_databuffer[writecounter][3]=(long int)(Regelglied_U.In_Soll_Gier*10000);
	regelung_databuffer[writecounter][4]=(long int)(Regelglied_U.In_Ist_Nick*10000);
	regelung_databuffer[writecounter][5]=(long int)(Regelglied_U.In_Ist_Roll*10000);
	regelung_databuffer[writecounter][6]=(long int)(Regelglied_U.In_Ist_Gier*10000);
	regelung_databuffer[writecounter][7]=(long int)(Regelglied_U.In_Ist_V_Nick*10000);
	regelung_databuffer[writecounter][8]=(long int)(Regelglied_U.In_Ist_V_Roll*10000);
	regelung_databuffer[writecounter][9]=(long int)(Regelglied_Y.Out_M_Roll*10000);
	regelung_databuffer[writecounter][10]=(long int)(Regelglied_Y.Out_M_Nick*10000);
	regelung_databuffer[writecounter][11]=(long int)(Regelglied_Y.Out_M_Gier*10000);
	regelung_databuffer[writecounter][12]=(long int)(Schubverteilung0_Y.Out_F_A*10000);
	regelung_databuffer[writecounter][13]=(long int)(Schubverteilung0_Y.Out_F_B*10000);
	regelung_databuffer[writecounter][14]=(long int)(Schubverteilung0_Y.Out_F_C*10000);
	regelung_databuffer[writecounter][15]=(long int)(Schubverteilung0_Y.Out_F_D*10000);
	regelung_databuffer[writecounter][16]=(long int)(Inverse_Propeller_Y.Out_n_A*10000);
	regelung_databuffer[writecounter][17]=(long int)(Inverse_Propeller_Y.Out_n_B*10000);
	regelung_databuffer[writecounter][18]=(long int)(Inverse_Propeller_Y.Out_n_C*10000);
	regelung_databuffer[writecounter][19]=(long int)(Inverse_Propeller_Y.Out_n_D*10000);
	regelung_databuffer[writecounter][20]=(long int)(Soll_V_Roll*10000);
	regelung_databuffer[writecounter][21]=(long int)(Xd_Roll*10000);
	regelung_databuffer[writecounter][22]=(long int)(p_anteil*10000);
	regelung_databuffer[writecounter][23]=(long int)(i_anteil*10000);
	regelung_databuffer[writecounter][24]=(long int)(d_anteil*10000);
	regelung_databuffer[writecounter][25]=(long int)(Y_roll*10000);
	regelung_databuffer[writecounter][26]=(long int)(Xd_V_Roll*10000);
	regelung_databuffer[writecounter][27]=(long int)(Ist_a_Roll*10000);
	regelung_databuffer[writecounter][28]=(long int)(Xd_a_Roll*10000);
	regelung_databuffer[writecounter][29]=(long int)(Soll_V_Nick*10000);
	regelung_databuffer[writecounter][30]=(long int)(Soll_a_Nick*10000);
	regelung_databuffer[writecounter][31]=(long int)(Xd_Nick*10000);
	regelung_databuffer[writecounter][32]=(long int)(Y_Nick*10000);
	regelung_databuffer[writecounter][33]=(long int)(Ist_a_Nick*10000);
	regelung_databuffer[writecounter][34]=(long int)(Xd_a_Nick*10000);
	regelung_databuffer[writecounter][35]=(long int)(get_ypr_nick_ist()*10000);
	regelung_databuffer[writecounter][36]=(long int)(get_ypr_roll_ist()*10000);
	regelung_databuffer[writecounter][37]=(long int)(get_ypr_yaw_ist()*10000);
	regelung_databuffer[writecounter][38]=(long int)(Soll_a_Roll*10000);
	regelung_databuffer[writecounter][39]=(long int)(p_anteil_nick*10000);
	regelung_databuffer[writecounter][40]=(long int)(i_anteil_nick*10000);
	regelung_databuffer[writecounter][41]=(long int)(d_anteil_nick*10000);
	regelung_databuffer[writecounter][42]=get_fifo_count();
	regelung_databuffer[writecounter][43]=(long int)(Hoehenregelung_U.Hoehe*10000);
	regelung_databuffer[writecounter][44]=(long int)(Hoehenregelung_U.Beschleunigung_Z*10000);
	regelung_databuffer[writecounter][45]=(long int)(h_z*10000);	
	regelung_databuffer[writecounter][46]=(long int)(h_rest_integrator*10000);
	regelung_databuffer[writecounter][47]=(long int)(Hoehenregelung_Y.Schubkraft*10000);
	regelung_databuffer[writecounter][48]=(long int)(th_dt*10000);
	regelung_databuffer[writecounter][49]=(long int)(Inti_rdy*10000);
	regelung_databuffer[writecounter][50]=(long int)(I_offset*10000);
	regelung_databuffer[writecounter][51]=(long int)(I_offset_nick*10000);
	writecounter++;
	if(writecounter >=50)writecounter=0;
}

void Regelung(void)
{
	/// Werte �bernehmen

	inSchub = get_schub_soll();
	
	inNickSollLage = 0;
	inRollSollLage = get_euler_roll_soll();
	inYawSollLage  = 0;
	
	inYawIstLage = get_euler_yaw_ist();
	inNickIstLage = get_euler_nick_ist();
	inRollIstLage = get_euler_roll_ist();
		 
	inRollIstV = get_rate_roll_ist();
	inNickIstV = get_rate_nick_ist();
	inYawIstV = get_rate_yaw_ist();

	/////////////////////////// Nick-Regler berechnen //////////////////////////////////////////
   //�u�erer Regler
   ea_Nick = (inNickSollLage) - (inNickIstLage+0.08*inNickIstV);  // Eingang in den �u�eren Regler
   if(inSchub > 0.1 && inSchub <=1 && inNickIstLage < 0.2 && inNickIstLage > -0.2)
   	ia_Nick = 0.01 * ea_Nick + ia_Nick;	//I-Anteil nur nahe der Nulllage ver�ndern  
   //Soll_v_Nick = ea_Nick*1 + ia_Nick*0.02; //Sollgeschwindigkeit des �u�eren Reglers
   //Soll_v_Nick = inNickSollLage*2.F;
   Soll_v_Nick = 0;
   //innerer Regler
   v_Nick_tp1 = 0.9*v_Nick_tp1 + 0.1*inNickIstV; // Tiefpass-gefilterter Gyrowert
   ei_Nick= Soll_v_Nick - v_Nick_tp1;	//Eingang in die innere Regelung
   pi_Nick = ei_Nick * 0.07;	//p-Anteil

   if(inSchub > 0.1 && inSchub <=1 /*&& inNickIstLage < 0.5 && inNickIstLage > -0.5*/)
   	ii_Nick = 0.08 * ei_Nick + ii_Nick;	//I-Anteil nur nahe der Nulllage ver�ndern 

   di_Nick = (ei_Nick - ei_Nick_alt)*10; //d-Anteil  
   if (di_Nick > 1.5)
	   di_Nick = 1.5;
   else if(di_Nick < -1.5)
	   di_Nick = -1.5;	  //Saturierung des D-Anteils

   aNick = (pi_Nick + (ii_Nick)*0.03 + di_Nick)*567; //Ausgang des inneren Reglers	 

	/////////////////////////// Roll-Regler berechnen ////////////////////////////////////////// 

   /*ea_Roll = (inRollSollLage) - (inRollIstLage+0.08*inRollIstV);  // Eingang in den �u�eren Regler
   if(inSchub > 0.1 && inSchub <=1 && inRollIstLage < 0.2 && inRollIstLage > -0.2)
   	ia_Roll = 0.01 * ea_Roll + ia_Roll;	//I-Anteil nur nahe der Nulllage ver�ndern  
   Soll_v_Roll = ea_Roll*2 + ia_Roll*0.02; //Sollgeschwindigkeit des �u�eren Reglers
   //Soll_v_Roll = inRollSollLage;

   //innerer Regler
   v_Roll_tp1 = 0.95*v_Roll_tp1 + 0.05*inRollIstV; // Tiefpass-gefilterter Gyrowert
   ei_Roll= Soll_v_Roll - v_Roll_tp1;	//Eingang in die innere Regelung
   pi_Roll = ei_Roll * 0.5;	//p-Anteil
   if(inSchub > 0.1 && inSchub <=1 && inRollIstLage < 0.2 && inRollIstLage > -0.2)
   	ii_Roll = 0.1 * ei_Roll + ii_Roll;	//I-Anteil nur nahe der Nulllage ver�ndern 

   di_Roll = (ei_Roll - ei_Roll_alt)*20; //d-Anteil  
   if (di_Roll > 1.5)
	   di_Roll = 1.5;
   else if(di_Roll < -1.5)
	   di_Roll = -1.5;	  //Saturierung des D-Anteils

   aRoll = (pi_Roll + (ii_Roll+37)*0.02 + di_Roll)*567; //Ausgang des inneren Reglers		 

	ea_Roll = (inRollSollLage) - (inRollIstLage+0.08*inRollIstV);  // Eingang in den �u�eren Regler
   if(inSchub > 0.1 && inSchub <=1 && inRollIstLage < 0.2 && inRollIstLage > -0.2)
   	ia_Roll = 0.01 * ea_Roll + ia_Roll;	//I-Anteil nur nahe der Nulllage ver�ndern  
   Soll_v_Roll = ea_Roll*2 + ia_Roll*0.02; //Sollgeschwindigkeit des �u�eren Reglers
   Soll_v_Roll = inRollSollLage;

   //innerer Regler
   v_Roll_tp1 = 0.95*v_Roll_tp1 + 0.05*inRollIstV; // Tiefpass-gefilterter Gyrowert
   ei_Roll= Soll_v_Roll - v_Roll_tp1;	//Eingang in die innere Regelung
   pi_Roll = ei_Roll * 0.1;	//p-Anteil
   if(inSchub > 0.1 && inSchub <=1 /*&& inRollIstLage < 0.2 && inRollIstLage > -0.2)
   	ii_Roll = 0.01 * ei_Roll + ii_Roll;	//I-Anteil nur nahe der Nulllage ver�ndern 


   di_Roll = (ei_Roll - ei_Roll_alt)*10; //d-Anteil  
   if (di_Roll > 1.5)
	   di_Roll = 1.5;
   else if(di_Roll < -1.5)
	   di_Roll = -1.5;	  //Saturierung des D-Anteils

   aRoll = (pi_Roll + (ii_Roll)*0.005 + di_Roll)*567; //Ausgang des inneren Reglers	 */

   aRoll = ((inRollSollLage) - (inRollIstLage))*567*-0.5;

	/////////////////////////// Yaw-Regler berechnen ////////////////////////////////////////// 

   ea_Yaw = (inYawSollLage) - (inYawIstLage+0.08*inYawIstV);  // Eingang in den �u�eren Regler
   if(inSchub > 0.1 && inSchub <=1 && inYawIstLage < 0.2 && inYawIstLage > -0.2)
   	ia_Yaw = 0.01 * ea_Yaw + ia_Yaw;	//I-Anteil nur nahe der Nulllage ver�ndern  
   Soll_v_Yaw = ea_Yaw*0.5 + ia_Yaw*0.002; //Sollgeschwindigkeit des �u�eren Reglers
   //Soll_v_Yaw = inYawSollLage;

   //innerer Regler
   v_Yaw_tp1 = 0.99*v_Yaw_tp1 + 0.01*inYawIstV; // Tiefpass-gefilterter Gyrowert
   ei_Yaw= Soll_v_Yaw - v_Yaw_tp1;	//Eingang in die innere Regelung
   pi_Yaw = ei_Yaw * 0.4;	//p-Anteil
   if(inSchub > 0.1 && inSchub <=1 && inYawIstLage < 0.2 && inYawIstLage > -0.2)
   	ii_Yaw = 0.01 * ei_Yaw + ii_Yaw;	//I-Anteil nur nahe der Nulllage ver�ndern 

   di_Yaw = (ei_Yaw - ei_Yaw_alt)*20; //d-Anteil  
   if (di_Yaw > 1.5)
	   di_Yaw = 1.5;
   else if(di_Yaw < -1.5)
	   di_Yaw = -1.5;	  //Saturierung des D-Anteils

   aYaw = (pi_Yaw + (ii_Yaw)*0.1 + di_Yaw)*567; //Ausgang des inneren Reglers
   
   if (aYaw > 2000.F)
	   aYaw = 2000.F;
   else if(aYaw < -2000.F)
	   aYaw = -2000.F;	  //Saturierung des Yaw-Anteils
  	
	/////////////////////////// Motorwerte setzen //////////////////////////////////////////  
   if(inSchub > 0.1 && inSchub <=1)
   {
     Schub_Offset = 6000 * inSchub;
	 aNick = 0.F;	
	 aYaw = 0.F;

     outMotor3 = Schub_Offset - aRoll + aYaw;
     //outMotor1 = Schub_Offset + aNick - aYaw;
     //outMotor4 = Schub_Offset - aNick - aYaw; 
  	 outMotor1 = 0.F;
 	 outMotor4 = 0.F;
     outMotor2 = Schub_Offset + aRoll + aYaw;
   }
   else 
   {					
  		outMotor1 = 0.F;
  		outMotor2 = 0.F;
		outMotor3 = 0.F;
		outMotor4 = 0.F;
   }

	/////////////////////////// Motorwerte saturieren und �bergeben //////////////////////// 
   if (outMotor1 > 6800.F) 	setMotor_1(6800.F);
   else if(outMotor1 < 0.F) setMotor_1(0.F);
   else setMotor_1(outMotor1/6800);

   if (outMotor2 > 6800.F)  setMotor_2(6800.F);
   else if(outMotor2 < 0.F) setMotor_2(0.F);
   else setMotor_2(outMotor2/6800);
	 
   if (outMotor3 > 6800.F)  setMotor_3(6800.F);
   else if(outMotor3 < 0.F) setMotor_3(0.F);
   else setMotor_3(outMotor3/6800);
	 
   if (outMotor4 > 6800.F)  setMotor_4(6800.F);
   else if(outMotor4 < 0.F) setMotor_4(0.F);	
   else setMotor_4(outMotor4/6800);	

	/////////////////////////// Alte Werte merken ////////////////////////////////////////// 

   v_Nick_tp1_alt = v_Nick_tp1;
   ei_Nick_alt = ei_Nick;

   v_Roll_tp1_alt = v_Roll_tp1;
   ei_Roll_alt = ei_Roll;

   v_Yaw_tp1_alt = v_Yaw_tp1;
   ei_Yaw_alt = ei_Yaw;

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
		//Für X Konfig Regelung();
		step_regler_X();
		datalog_regelung();
		chThdSleepUntil(time);
  }
}




void setup_Regelung()
{
	chThdCreateStatic(RegelungSyncThreadWorkingArea, sizeof(RegelungSyncThreadWorkingArea), HIGHPRIO, RegelungSyncthread, NULL);
	chThdCreateStatic(RegelungPrintThreadWorkingArea, sizeof(RegelungPrintThreadWorkingArea), NORMALPRIO, RegelungPrintthread, NULL);
	chThdCreateStatic(RegelungThreadWorkingArea, sizeof(RegelungThreadWorkingArea), ABSPRIO, Regelungsthread, NULL);
	init_regler_X();
	
}
