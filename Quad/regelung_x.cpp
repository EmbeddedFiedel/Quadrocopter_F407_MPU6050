//Normal Includes
#include "Regelung.h"
#include "regelung_x.h"
#include "ch.h"
#include "hal.h"
#include "Lage.h"
#include "Fernsteuerung.h"
#include "Motoren.h"
#include "barometer.h"


//Init Models
void init_regler_X(){
	//Zur Parametrierung
	//Input_Kennlinie_initialize();
	//normale Inits
	Regelglied_initialize();
	Regelglied_U.t_init=chTimeNow();
	Schubverteilung0_initialize();
	Inverse_Propeller_initialize();
	Hoehenregelung_initialize();
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
	
	//Sollwerte

	Regelglied_U.In_Soll_Roll=get_euler_roll_soll();
	Regelglied_U.In_Soll_Nick=get_euler_nick_soll();
	Regelglied_U.In_Soll_Gier=get_euler_yaw_soll();
  
	//Istwerte
	//getRoll + Gier funktion negiert, da miniquad nach Luftfahrtnorm
	Regelglied_U.t_sys=chTimeNow();
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

	//Set Outputs into Hoehenregelung

	Hoehenregelung_U.Throttle=get_schub_soll()/0.68;
	Hoehenregelung_U.Hoehe=baro_get_altitude();
	Hoehenregelung_U.Beschleunigung_Z=get_accel_world_z();
	
	//Hoehenregelung
	Hoehenregelung_step();

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

