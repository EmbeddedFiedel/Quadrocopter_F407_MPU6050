//Normal Includes
#include "Regelung.h"
#include "regelung_x.h"
#include "ch.h"
#include "hal.h"
#include "Lage.h"
#include "Fernsteuerung.h"
#include "Motoren.h"
#include "barometer.h"

float temperatur_;
float altitude_;
float pressure_;
float norm_pressure_;
float accelw_x;
float accelw_y;
float accelw_z;
float accel_z;
float accel_y;
float accel_x;
float accelr_z;
float accelr_y;
float accelr_x;

//Init Models
void init_regler_X(){
	//Zur Parametrierung
	//Input_Kennlinie_initialize();
	//normale Inits
	Regelglied_initialize();
	Schubverteilung0_initialize();
	Inverse_Propeller_initialize();
}


void step_regler_X(){
	static boolean_T OverrunFlag = FALSE;
	
	  /* Check for overrun */
	if (OverrunFlag) {
		
		setMotor_1(0.0);
		setMotor_2(0.0);
		setMotor_3(0.0);
		setMotor_4(0.0);
		return;
	}

	
	OverrunFlag = TRUE;
	//Kennlinie erzeugen
  //Input_Kennlinie_step();
	//barometer
	temperatur_=baro_get_temperatur();
	altitude_=baro_get_altitude();
	pressure_=baro_get_pressure();
	norm_pressure_=baro_get_standardized_pressure();
	accel_z=get_accel_z();
	accel_x=get_accel_x();
	accel_y=get_accel_y();
	accelw_x=get_accel_world_x();
	accelw_y=get_accel_world_y();
	accelw_z=get_accel_world_z();
	accelr_x=get_accel_real_x();
	accelr_y=get_accel_real_y();
	accelr_z=get_accel_real_z();
	
	
	//Sollwerte

	Regelglied_U.In_Soll_Roll=get_euler_roll_soll();
	Regelglied_U.In_Soll_Nick=get_euler_nick_soll();
	Regelglied_U.In_Soll_Gier=get_euler_yaw_soll();
  
	//Istwerte
	//getRoll + Gier funktion negiert, da miniquad nach Luftfahrtnorm
	Regelglied_U.In_Ist_Roll=get_ypr_roll_ist();
	Regelglied_U.In_Ist_Nick=get_ypr_nick_ist(); 
	Regelglied_U.In_Ist_Gier=get_ypr_roll_ist();
	Regelglied_U.In_Ist_V_Roll=get_rate_roll_ist();
	Regelglied_U.In_Ist_V_Nick=get_rate_nick_ist();

	//Gas setzen für Regler und Schubkraftverteilung
  Regelglied_U.Throttle=get_schub_soll()/0.68;
	Schubverteilung0_U.In_Throttle=get_schub_soll()/0.68;
	
	//Step Regler
	Regelglied_step();

	//Set Outputs into Schubkraftverteilung
	Schubverteilung0_U.In_M_Roll=Regelglied_Y.Out_M_Roll;
	Schubverteilung0_U.In_M_Nick=Regelglied_Y.Out_M_Nick;
	Schubverteilung0_U.In_M_Gier=Regelglied_Y.Out_M_Gier;	
	

	//Step Schubkraftverteilung
	Schubverteilung0_step();

	//Set Schubkraftverteilung Outputs to Inputs Propeller_Inverse
	Inverse_Propeller_U.In_F_A=Schubverteilung0_Y.Out_F_A;
	Inverse_Propeller_U.In_F_B=Schubverteilung0_Y.Out_F_B;
	Inverse_Propeller_U.In_F_C=Schubverteilung0_Y.Out_F_C;
	Inverse_Propeller_U.In_F_D=Schubverteilung0_Y.Out_F_D;

	//Step Inverse Propeller
	Inverse_Propeller_step();

	//Step Motor
	setMotor_1(Inverse_Propeller_Y.Out_n_A);
	setMotor_2(Inverse_Propeller_Y.Out_n_B);
	setMotor_3(Inverse_Propeller_Y.Out_n_C);
	setMotor_4(Inverse_Propeller_Y.Out_n_D);
	
// 	setMotor_1(0.2);
// 	setMotor_1(0.0);
// 	setMotor_2(0.2);
// 	setMotor_2(0.0);
// 	setMotor_3(0.22);
// 	setMotor_3(0.0);
// 	setMotor_4(0.22);
// 	setMotor_4(0.0);

	/* Indicate task complete */
	OverrunFlag = FALSE;
	

}

