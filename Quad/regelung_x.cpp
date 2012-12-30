//Normal Includes
#include "Regelung.h"
#include "regelung_x.h"
#include "ch.h"
#include "hal.h"
#include "Lage.h"
#include "Fernsteuerung.h"
#include "Motoren.h"



//Init Models
void init_regler_X(){
	Regelglied_in//Zur Parametrierung
	Input_Kennlinie_initialize();
	//normale Inits_initialize();
	Schubverteilung0_initialize();
	Inverse_Propeller_initialize();
}


void step_regler_X(){
	static boolean_T OverrunFlag = FALSE;
	  /* Check for overrun */
	if (OverrunFlag) {
		return;
	}

	OverrunFlag = TRUE;
	//Sollwerte
	
	Regelglied_  	//Kennlinie erzeugen
  Input_Kennlinie_step();
	
	//Sollwerte

	
	Regelglied_U.In_Soll_Roll=Input_Kennlinie_Y.Winkel;//get_euler_roll_soll();
	Regelglied_U.In_Soll_Nick=0;//get_euler_nick_soll();
	Regelglied_U.In_Soll_Gier=0;e
	//getRoll + Gier funktion negiert, da miniquad nach Luftfahrtnorm
	Regelglied_U.In_Ist_Roll=-get_euler_roll_ist();
	Regelglied_U.In_Ist_Nick=get_euler_nick_ist(); //Behoben
	Regelglied_U.In_Ist_Gier=r_yaw_ist();
	Regelglied_U.In_Ist_V_Roll=-get_rate_roll_ist();
	Regelglied_U.In_Ist_V_Nick=get_rate_nick_ist();

	//Step Regler
	Regelglied_step();

	//Set Outputs into Schubkraftverteilung
	Schubverteilung0_U.In_M_Roll=Regelglied_Y.Out_M_Roll;
	Schubverteilung0_U.In_M_Nick=Regelglied_Y.Out_M_Nick;
	Schubverteilung0_U.In_M_Gier=Regelglied_Y.Ou//testt_M_Gier;	
	Schubverteilung0_U.In_Throttle=get_schub_soll()/0.68;

	//Step SchubkraftverteilungInput_Kennlinie_Y.Throttle ;//
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

	/* Indicate task complete */
	OverrunFlag = FALSE;
	

}

