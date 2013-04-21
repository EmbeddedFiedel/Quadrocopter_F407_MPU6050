/*
 * File: Regelglied.cpp
 *
 * Code generated for Simulink model 'Regelglied'.
 *
 * Model version                  : 1.524
 * Simulink Coder version         : 8.2 (R2012a) 29-Dec-2011
 * TLC version                    : 8.2 (Dec 29 2011)
 * C/C++ source code generated on : Sun Mar 10 15:02:26 2013
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 * Validation result: Passed (6), Warnings (6), Error (0)
 */

#include "Regelglied.h"
#include "Regelglied_private.h"

/* Exported block signals */
real_T Soll_V_Roll;                    /* '<S1>/D-Anteil' */
real_T Xd_Roll;                        /* '<S1>/Add' */
real_T p_anteil;                       /* '<S7>/Gain' */
real_T i_anteil;                       /* '<S7>/Integrator' */
real_T d_anteil;                       /* '<S7>/D-Anteil' */
real_T I_offset;                       /* '<S7>/Integrator1' */
real_T Y_roll;                         /* '<S7>/Add' */
real_T Xd_V_Roll;                      /* '<S1>/Add1' */
real_T Soll_a_Roll;                    /* '<S1>/D-Anteil2' */
real_T Ist_a_Roll;                     /* '<S1>/D-Anteil4' */
real_T Xd_a_Roll;                      /* '<S1>/Add2' */
real_T Soll_V_Nick;                    /* '<S1>/D-Anteil1' */
real_T Xd_Nick;                        /* '<S1>/Add3' */
real_T p_anteil_nick;                  /* '<S8>/Gain' */
real_T i_anteil_nick;                  /* '<S8>/Integrator' */
real_T d_anteil_nick;                  /* '<S8>/D-Anteil' */
real_T I_offset_nick;                  /* '<S8>/Integrator1' */
real_T Y_Nick;                         /* '<S8>/Add' */
real_T Soll_a_Nick;                    /* '<S1>/D-Anteil3' */
real_T Ist_a_Nick;                     /* '<S1>/D-Anteil5' */
real_T Xd_a_Nick;                      /* '<S1>/Add5' */
uint8_T Inti_rdy;                      /* '<S10>/Compare' */

/* Exported block parameters */
real_T ena_byp_a = 1.0; /* Variable: ena_byp_a
* Referenced by: '<S1>/Constant2'
*/
real_T ena_pid = 1; /* Variable: ena_pid
* Referenced by: '<S1>/Constant'
*/
real_T gain_d_soll_a = 1.0; /* Variable: gain_d_soll_a
* Referenced by:
* '<S1>/Gain2'
* '<S1>/Gain3'
*/
real_T gain_d_soll_v = 1.0; /* Variable: gain_d_soll_v
* Referenced by:
* '<S1>/Gain'
* '<S1>/Gain1'
*/
real_T kp_a_nick = 0.07;	/* Variable: kp_a_nick
* Referenced by: '<S2>/Gain'
*/
real_T kp_a_roll = 0.07; /* Variable: kp_a_roll
* Referenced by: '<S4>/Gain'
*/
real_T kp_nick =13.7; /* Variable: kp_nick
* Referenced by: '<S7>/Gain'
*/
real_T kp_roll = 18.8; /* Variable: kp_roll
* Referenced by: '<S6>/Gain'
*/
real_T kp_v_nick = 0.875; /* Variable: kp_v_nick
* Referenced by: '<S3>/Gain'
*/
real_T kp_v_roll = 0.75; /* Variable: kp_v_roll= 0.675
* Referenced by: '<S5>/Gain'
*/
real_T tn_nick = 0.48; /* Variable: tn_nick
* Referenced by: '<S7>/Gain1'
*/
real_T tn_roll = 0.42; /* Variable: tn_roll
* Referenced by: '<S6>/Gain1'
*/
real_T tv_nick = 3.7;/* Variable: tv_nick
* Referenced by: '<S7>/Gain2'
*/
real_T tv_roll = 2.0;                /* Variable: tv_roll
                                        * Referenced by: '<S6>/Gain2'
                                        */

real_T tn_roll_init = tn_roll;            /* Variable: tn_roll_init
                                        * Referenced by: '<S6>/Gain3'
                                        */
real_T tn_nick_init = tn_nick;            /* Variable: tn_roll_init
                                        * Referenced by: '<S6>/Gain3'
                                        */
real_T t_init_i = 9900000;                /* Variable: t_init_i
                                        * Referenced by: '<S9>/Constant'
                                        */

/* Block signals (auto storage) */
BlockIO_Regelglied Regelglied_B;

/* Continuous states */
ContinuousStates_Regelglied Regelglied_X;

/* Block states (auto storage) */
D_Work_Regelglied Regelglied_DWork;

/* Previous zero-crossings (trigger) states */
PrevZCSigStates_Regelglied Regelglied_PrevZCSigState;

/* External inputs (root inport signals with auto storage) */
ExternalInputs_Regelglied Regelglied_U;

/* External outputs (root outports fed by signals with auto storage) */
ExternalOutputs_Regelglied Regelglied_Y;

/* Real-time model */
RT_MODEL_Regelglied Regelglied_M_;
RT_MODEL_Regelglied *const Regelglied_M = &Regelglied_M_;

/*
 * This function updates continuous states using the ODE3 fixed-step
 * solver algorithm
 */
static void rt_ertODEUpdateContinuousStates(RTWSolverInfo *si )
{
  /* Solver Matrices */
  static const real_T rt_ODE3_A[3] = {
    1.0/2.0, 3.0/4.0, 1.0
  };

  static const real_T rt_ODE3_B[3][3] = {
    { 1.0/2.0, 0.0, 0.0 },

    { 0.0, 3.0/4.0, 0.0 },

    { 2.0/9.0, 1.0/3.0, 4.0/9.0 }
  };

  time_T t = rtsiGetT(si);
  time_T tnew = rtsiGetSolverStopTime(si);
  time_T h = rtsiGetStepSize(si);
  real_T *x = rtsiGetContStates(si);
  ODE3_IntgData *id = (ODE3_IntgData *)rtsiGetSolverData(si);
  real_T *y = id->y;
  real_T *f0 = id->f[0];
  real_T *f1 = id->f[1];
  real_T *f2 = id->f[2];
  real_T hB[3];
  int_T i;
  int_T nXc = 12;
  rtsiSetSimTimeStep(si,MINOR_TIME_STEP);

  /* Save the state values at time t in y, we'll use x as ynew. */
  (void) memcpy(y, x,
                nXc*sizeof(real_T));

  /* Assumes that rtsiSetT and ModelOutputs are up-to-date */
  /* f0 = f(t,y) */
  rtsiSetdX(si, f0);
  Regelglied_derivatives();

  /* f(:,2) = feval(odefile, t + hA(1), y + f*hB(:,1), args(:)(*)); */
  hB[0] = h * rt_ODE3_B[0][0];
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0]);
  }

  rtsiSetT(si, t + h*rt_ODE3_A[0]);
  rtsiSetdX(si, f1);
  Regelglied_step();
  Regelglied_derivatives();

  /* f(:,3) = feval(odefile, t + hA(2), y + f*hB(:,2), args(:)(*)); */
  for (i = 0; i <= 1; i++) {
    hB[i] = h * rt_ODE3_B[1][i];
  }

  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0] + f1[i]*hB[1]);
  }

  rtsiSetT(si, t + h*rt_ODE3_A[1]);
  rtsiSetdX(si, f2);
  Regelglied_step();
  Regelglied_derivatives();

  /* tnew = t + hA(3);
     ynew = y + f*hB(:,3); */
  for (i = 0; i <= 2; i++) {
    hB[i] = h * rt_ODE3_B[2][i];
  }

  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0] + f1[i]*hB[1] + f2[i]*hB[2]);
  }

  rtsiSetT(si, tnew);
  rtsiSetSimTimeStep(si,MAJOR_TIME_STEP);
}

/* Model step function */
void Regelglied_step(void)
{
  real_T rtb_Saturation;
  int8_T rtAction;
  if (rtmIsMajorTimeStep(Regelglied_M)) {
    /* set solver stop time */
    rtsiSetSolverStopTime(&Regelglied_M->solverInfo,
                          ((Regelglied_M->Timing.clockTick0+1)*
      Regelglied_M->Timing.stepSize0));
  }                                    /* end MajorTimeStep */

  /* Update absolute time of base rate at minor time step */
  if (rtmIsMinorTimeStep(Regelglied_M)) {
    Regelglied_M->Timing.t[0] = rtsiGetT(&Regelglied_M->solverInfo);
  }

  /* Gain: '<S1>/Gain' incorporates:
   *  Inport: '<Root>/In_Soll_Roll'
   */
  Regelglied_B.Gain = gain_d_soll_v * Regelglied_U.In_Soll_Roll;

  /* TransferFcn: '<S1>/D-Anteil' */
  Soll_V_Roll = 2.6315789473684212*Regelglied_B.Gain;
  Soll_V_Roll += (-6.9252077562326884)*Regelglied_X.DAnteil_CSTATE;

  /* Sum: '<S1>/Add' incorporates:
   *  Inport: '<Root>/In_Ist_Roll'
   *  Inport: '<Root>/In_Soll_Roll'
   */
  Xd_Roll = Regelglied_U.In_Soll_Roll - Regelglied_U.In_Ist_Roll;

  /* Gain: '<S7>/Gain' */
  p_anteil = kp_roll * Xd_Roll;

  /* Saturate: '<S1>/Saturation' incorporates:
   *  Inport: '<Root>/Throttle'
   */
  if (Regelglied_U.Throttle >= 1.0) {
    rtb_Saturation = 1.0;
  } else if (Regelglied_U.Throttle <= 0.0) {
    rtb_Saturation = 0.0;
  } else {
    rtb_Saturation = Regelglied_U.Throttle;
  }

  /* End of Saturate: '<S1>/Saturation' */

  /* If: '<S9>/If' incorporates:
   *  Constant: '<S11>/Constant'
   *  Constant: '<S12>/Constant'
   */
  if (rtmIsMajorTimeStep(Regelglied_M)) {
    if (rtb_Saturation >= 0.01) {
      rtAction = 0;
    } else {
      rtAction = 1;
    }

    Regelglied_DWork.If_ActiveSubsystem = rtAction;
  } else {
    rtAction = Regelglied_DWork.If_ActiveSubsystem;
  }

  switch (rtAction) {
   case 0:
    /* Outputs for IfAction SubSystem: '<S9>/If Action Subsystem' incorporates:
     *  ActionPort: '<S11>/Action Port'
     */
    if (rtmIsMajorTimeStep(Regelglied_M)) {
      Regelglied_B.Merge = 1.0;
    }

    /* End of Outputs for SubSystem: '<S9>/If Action Subsystem' */
    break;

   case 1:
    /* Outputs for IfAction SubSystem: '<S9>/If Action Subsystem1' incorporates:
     *  ActionPort: '<S12>/Action Port'
     */
    if (rtmIsMajorTimeStep(Regelglied_M)) {
      Regelglied_B.Merge = 0.0;
    }

    /* End of Outputs for SubSystem: '<S9>/If Action Subsystem1' */
    break;
  }

  /* End of If: '<S9>/If' */

  /* Integrator: '<S7>/Integrator' */
  if (rtmIsMajorTimeStep(Regelglied_M)) {
    ZCEventType zcEvent;
    zcEvent = rt_ZCFcn(RISING_ZERO_CROSSING,
                       &Regelglied_PrevZCSigState.Integrator_Reset_ZCE,
                       Regelglied_B.Merge);

    /* evaluate zero-crossings */
    if (zcEvent) {
      Regelglied_X.Integrator_CSTATE = 0.0;
    }
  }

  i_anteil = Regelglied_X.Integrator_CSTATE;

  /* Gain: '<S7>/Gain2' */
  Regelglied_B.Gain2 = tv_roll * Xd_Roll;

  /* TransferFcn: '<S7>/D-Anteil' */
  d_anteil = 100.0*Regelglied_B.Gain2;
  d_anteil += (-10000.0)*Regelglied_X.DAnteil_CSTATE_p;

  /* RelationalOperator: '<S10>/Compare' incorporates:
   *  Constant: '<S10>/Constant'
   *  Inport: '<Root>/t_init'
   *  Inport: '<Root>/t_sys'
   *  Sum: '<S2>/Add'
   */
  Inti_rdy = (uint8_T)(Regelglied_U.t_sys - Regelglied_U.t_init <= t_init_i);

  /* Switch: '<S7>/Switch2' incorporates:
   *  Constant: '<S7>/Constant'
   */
  if (Inti_rdy > 0) {
    Regelglied_B.Switch2 = Regelglied_B.Merge;
  } else {
    Regelglied_B.Switch2 = 0.0;
  }

  /* End of Switch: '<S7>/Switch2' */

  /* Integrator: '<S7>/Integrator1' */
  if (rtmIsMajorTimeStep(Regelglied_M)) {
    ZCEventType zcEvent;
    zcEvent = rt_ZCFcn(RISING_ZERO_CROSSING,
                       &Regelglied_PrevZCSigState.Integrator1_Reset_ZCE,
                       Regelglied_B.Switch2);

    /* evaluate zero-crossings */
    if (zcEvent) {
      Regelglied_X.Integrator1_CSTATE = 0.0;
    }
  }

  I_offset = Regelglied_X.Integrator1_CSTATE;

  /* Sum: '<S7>/Add' */
  Y_roll = ((p_anteil + i_anteil) + d_anteil) + I_offset;

  /* Switch: '<S1>/Switch1' incorporates:
   *  Constant: '<S1>/Constant'
   *  Constant: '<S1>/Constant1'
   */
  if (ena_pid >= 0.0) {
    rtb_Saturation = Y_roll;
  } else {
    rtb_Saturation = 0.0;
  }

  /* End of Switch: '<S1>/Switch1' */

  /* Sum: '<S1>/Add1' incorporates:
   *  Inport: '<Root>/In_Ist_V_Roll'
   */
  Xd_V_Roll = (Soll_V_Roll + rtb_Saturation) - Regelglied_U.In_Ist_V_Roll;

  /* Gain: '<S6>/Gain' */
  rtb_Saturation = kp_v_roll * Xd_V_Roll;

  /* TransferFcn: '<S1>/D-Anteil2' */
  Soll_a_Roll = 2.5*Soll_V_Roll;
  Soll_a_Roll += (-6.25)*Regelglied_X.DAnteil2_CSTATE;
  if (rtmIsMajorTimeStep(Regelglied_M)) {
    /* Gain: '<S1>/Gain2' incorporates:
     *  Inport: '<Root>/In_Ist_V_Roll'
     */
    Regelglied_B.Gain2_p = gain_d_soll_a * Regelglied_U.In_Ist_V_Roll;
  }

  /* TransferFcn: '<S1>/D-Anteil4' */
  Ist_a_Roll = 1.6666666666666667*Regelglied_B.Gain2_p;
  Ist_a_Roll += (-2.7777777777777781)*Regelglied_X.DAnteil4_CSTATE;

  /* Sum: '<S1>/Add2' */
  Xd_a_Roll = (Soll_a_Roll + rtb_Saturation) - Ist_a_Roll;

  /* Switch: '<S1>/Switch2' incorporates:
   *  Constant: '<S1>/Constant2'
   */
  if (ena_byp_a >= 0.0) {
    /* Outport: '<Root>/Out_M_Roll' incorporates:
     *  Gain: '<S1>/Gain5'
     */
    Regelglied_Y.Out_M_Roll = kp_a_roll * rtb_Saturation;
  } else {
    /* Outport: '<Root>/Out_M_Roll' incorporates:
     *  Gain: '<S5>/Gain'
     */
    Regelglied_Y.Out_M_Roll = kp_a_roll * Xd_a_Roll;
  }

  /* End of Switch: '<S1>/Switch2' */

  /* Gain: '<S1>/Gain1' incorporates:
   *  Inport: '<Root>/In_Soll_Nick'
   */
  Regelglied_B.Gain1 = gain_d_soll_v * Regelglied_U.In_Soll_Nick;

  /* TransferFcn: '<S1>/D-Anteil1' */
  Soll_V_Nick = 2.6315789473684212*Regelglied_B.Gain1;
  Soll_V_Nick += (-6.9252077562326884)*Regelglied_X.DAnteil1_CSTATE;

  /* Sum: '<S1>/Add3' incorporates:
   *  Inport: '<Root>/In_Ist_Nick'
   *  Inport: '<Root>/In_Soll_Nick'
   */
  Xd_Nick = Regelglied_U.In_Soll_Nick - Regelglied_U.In_Ist_Nick;

  /* Gain: '<S8>/Gain' */
  p_anteil_nick = kp_nick * Xd_Nick;

  /* Integrator: '<S8>/Integrator' */
  if (rtmIsMajorTimeStep(Regelglied_M)) {
    ZCEventType zcEvent;
    zcEvent = rt_ZCFcn(RISING_ZERO_CROSSING,
                       &Regelglied_PrevZCSigState.Integrator_Reset_ZCE_m,
                       Regelglied_B.Merge);

    /* evaluate zero-crossings */
    if (zcEvent) {
      Regelglied_X.Integrator_CSTATE_l = 0.0;
    }
  }

  i_anteil_nick = Regelglied_X.Integrator_CSTATE_l;

  /* Gain: '<S8>/Gain2' */
  Regelglied_B.Gain2_a = tv_nick * Xd_Nick;

  /* TransferFcn: '<S8>/D-Anteil' */
  d_anteil_nick = 100.0*Regelglied_B.Gain2_a;
  d_anteil_nick += (-10000.0)*Regelglied_X.DAnteil_CSTATE_pj;

  /* Switch: '<S8>/Switch2' incorporates:
   *  Constant: '<S8>/Constant'
   */
  if (Inti_rdy > 0) {
    Regelglied_B.Switch2_m = Regelglied_B.Merge;
  } else {
    Regelglied_B.Switch2_m = 0.0;
  }

  /* End of Switch: '<S8>/Switch2' */

  /* Integrator: '<S8>/Integrator1' */
  if (rtmIsMajorTimeStep(Regelglied_M)) {
    ZCEventType zcEvent;
    zcEvent = rt_ZCFcn(RISING_ZERO_CROSSING,
                       &Regelglied_PrevZCSigState.Integrator1_Reset_ZCE_f,
                       Regelglied_B.Switch2_m);

    /* evaluate zero-crossings */
    if (zcEvent) {
      Regelglied_X.Integrator1_CSTATE_m = 0.0;
    }
  }

  I_offset_nick = Regelglied_X.Integrator1_CSTATE_m;

  /* Sum: '<S8>/Add' */
  Y_Nick = ((p_anteil_nick + i_anteil_nick) + d_anteil_nick) + I_offset_nick;

  /* Switch: '<S1>/Switch' incorporates:
   *  Constant: '<S1>/Constant'
   *  Constant: '<S1>/Constant1'
   */
  if (ena_pid >= 0.0) {
    rtb_Saturation = Y_Nick;
  } else {
    rtb_Saturation = 0.0;
  }

  /* End of Switch: '<S1>/Switch' */

  /* Gain: '<S4>/Gain' incorporates:
   *  Inport: '<Root>/In_Ist_V_Nick'
   *  Sum: '<S1>/Add4'
   */
  rtb_Saturation = ((Soll_V_Nick + rtb_Saturation) - Regelglied_U.In_Ist_V_Nick)
    * kp_v_nick;

  /* TransferFcn: '<S1>/D-Anteil3' */
  Soll_a_Nick = 2.5*Soll_V_Nick;
  Soll_a_Nick += (-6.25)*Regelglied_X.DAnteil3_CSTATE;
  if (rtmIsMajorTimeStep(Regelglied_M)) {
    /* Gain: '<S1>/Gain3' incorporates:
     *  Inport: '<Root>/In_Ist_V_Nick'
     */
    Regelglied_B.Gain3 = gain_d_soll_a * Regelglied_U.In_Ist_V_Nick;
  }

  /* TransferFcn: '<S1>/D-Anteil5' */
  Ist_a_Nick = 1.6666666666666667*Regelglied_B.Gain3;
  Ist_a_Nick += (-2.7777777777777781)*Regelglied_X.DAnteil5_CSTATE;

  /* Sum: '<S1>/Add5' */
  Xd_a_Nick = (Soll_a_Nick + rtb_Saturation) - Ist_a_Nick;

  /* Switch: '<S1>/Switch3' incorporates:
   *  Constant: '<S1>/Constant2'
   */
  if (ena_byp_a >= 0.0) {
    /* Outport: '<Root>/Out_M_Nick' incorporates:
     *  Gain: '<S1>/Gain4'
     */
    Regelglied_Y.Out_M_Nick = kp_a_nick * rtb_Saturation;
  } else {
    /* Outport: '<Root>/Out_M_Nick' incorporates:
     *  Gain: '<S3>/Gain'
     */
    Regelglied_Y.Out_M_Nick = kp_a_nick * Xd_a_Nick;
  }

  /* End of Switch: '<S1>/Switch3' */

  /* Switch: '<S7>/Switch1' incorporates:
   *  Constant: '<S7>/Constant'
   */
  if (Inti_rdy > 0) {
    rtb_Saturation = 0.0;
  } else {
    rtb_Saturation = Xd_Roll;
  }

  /* End of Switch: '<S7>/Switch1' */

  /* Gain: '<S7>/Gain1' */
  Regelglied_B.Gain1_d = 1.0 / tn_roll * rtb_Saturation;

  /* Switch: '<S7>/Switch' incorporates:
   *  Constant: '<S7>/Constant'
   */
  if (Inti_rdy > 0) {
    rtb_Saturation = Xd_Roll;
  } else {
    rtb_Saturation = 0.0;
  }

  /* End of Switch: '<S7>/Switch' */

  /* Gain: '<S7>/Gain3' */
  Regelglied_B.Gain3_j = 1.0 / tn_roll_init * rtb_Saturation;

  /* Switch: '<S8>/Switch1' incorporates:
   *  Constant: '<S8>/Constant'
   */
  if (Inti_rdy > 0) {
    rtb_Saturation = 0.0;
  } else {
    rtb_Saturation = Xd_Nick;
  }

  /* End of Switch: '<S8>/Switch1' */

  /* Gain: '<S8>/Gain1' */
  Regelglied_B.Gain1_p = 1.0 / tn_nick * rtb_Saturation;

  /* Switch: '<S8>/Switch' incorporates:
   *  Constant: '<S8>/Constant'
   */
  if (Inti_rdy > 0) {
    rtb_Saturation = Xd_Nick;
  } else {
    rtb_Saturation = 0.0;
  }

  /* End of Switch: '<S8>/Switch' */

  /* Gain: '<S8>/Gain3' */
  Regelglied_B.Gain3_e = 1.0 / tn_nick_init * rtb_Saturation;
  if (rtmIsMajorTimeStep(Regelglied_M)) {
    rt_ertODEUpdateContinuousStates(&Regelglied_M->solverInfo);

    /* Update absolute time for base rate */
    /* The "clockTick0" counts the number of times the code of this task has
     * been executed. The absolute time is the multiplication of "clockTick0"
     * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
     * overflow during the application lifespan selected.
     */
    ++Regelglied_M->Timing.clockTick0;
    Regelglied_M->Timing.t[0] = rtsiGetSolverStopTime(&Regelglied_M->solverInfo);

    {
      /* Update absolute timer for sample time: [0.01s, 0.0s] */
      /* The "clockTick1" counts the number of times the code of this task has
       * been executed. The resolution of this integer timer is 0.01, which is the step size
       * of the task. Size of "clockTick1" ensures timer will not overflow during the
       * application lifespan selected.
       */
      Regelglied_M->Timing.clockTick1++;
    }
  }                                    /* end MajorTimeStep */
}

/* Derivatives for root system: '<Root>' */
void Regelglied_derivatives(void)
{
  /* Derivatives for TransferFcn: '<S1>/D-Anteil' */
  {
    ((StateDerivatives_Regelglied *) Regelglied_M->ModelData.derivs)
      ->DAnteil_CSTATE = Regelglied_B.Gain;
    ((StateDerivatives_Regelglied *) Regelglied_M->ModelData.derivs)
      ->DAnteil_CSTATE += (-2.6315789473684212)*Regelglied_X.DAnteil_CSTATE;
  }

  /* Derivatives for Integrator: '<S7>/Integrator' */
  {
    ((StateDerivatives_Regelglied *) Regelglied_M->ModelData.derivs)
      ->Integrator_CSTATE = Regelglied_B.Gain1_d;
  }

  /* Derivatives for TransferFcn: '<S7>/D-Anteil' */
  {
    ((StateDerivatives_Regelglied *) Regelglied_M->ModelData.derivs)
      ->DAnteil_CSTATE_p = Regelglied_B.Gain2;
    ((StateDerivatives_Regelglied *) Regelglied_M->ModelData.derivs)
      ->DAnteil_CSTATE_p += (-100.0)*Regelglied_X.DAnteil_CSTATE_p;
  }

  /* Derivatives for Integrator: '<S7>/Integrator1' */
  {
    ((StateDerivatives_Regelglied *) Regelglied_M->ModelData.derivs)
      ->Integrator1_CSTATE = Regelglied_B.Gain3_j;
  }

  /* Derivatives for TransferFcn: '<S1>/D-Anteil2' */
  {
    ((StateDerivatives_Regelglied *) Regelglied_M->ModelData.derivs)
      ->DAnteil2_CSTATE = Soll_V_Roll;
    ((StateDerivatives_Regelglied *) Regelglied_M->ModelData.derivs)
      ->DAnteil2_CSTATE += (-2.5)*Regelglied_X.DAnteil2_CSTATE;
  }

  /* Derivatives for TransferFcn: '<S1>/D-Anteil4' */
  {
    ((StateDerivatives_Regelglied *) Regelglied_M->ModelData.derivs)
      ->DAnteil4_CSTATE = Regelglied_B.Gain2_p;
    ((StateDerivatives_Regelglied *) Regelglied_M->ModelData.derivs)
      ->DAnteil4_CSTATE += (-1.6666666666666667)*Regelglied_X.DAnteil4_CSTATE;
  }

  /* Derivatives for TransferFcn: '<S1>/D-Anteil1' */
  {
    ((StateDerivatives_Regelglied *) Regelglied_M->ModelData.derivs)
      ->DAnteil1_CSTATE = Regelglied_B.Gain1;
    ((StateDerivatives_Regelglied *) Regelglied_M->ModelData.derivs)
      ->DAnteil1_CSTATE += (-2.6315789473684212)*Regelglied_X.DAnteil1_CSTATE;
  }

  /* Derivatives for Integrator: '<S8>/Integrator' */
  {
    ((StateDerivatives_Regelglied *) Regelglied_M->ModelData.derivs)
      ->Integrator_CSTATE_l = Regelglied_B.Gain1_p;
  }

  /* Derivatives for TransferFcn: '<S8>/D-Anteil' */
  {
    ((StateDerivatives_Regelglied *) Regelglied_M->ModelData.derivs)
      ->DAnteil_CSTATE_pj = Regelglied_B.Gain2_a;
    ((StateDerivatives_Regelglied *) Regelglied_M->ModelData.derivs)
      ->DAnteil_CSTATE_pj += (-100.0)*Regelglied_X.DAnteil_CSTATE_pj;
  }

  /* Derivatives for Integrator: '<S8>/Integrator1' */
  {
    ((StateDerivatives_Regelglied *) Regelglied_M->ModelData.derivs)
      ->Integrator1_CSTATE_m = Regelglied_B.Gain3_e;
  }

  /* Derivatives for TransferFcn: '<S1>/D-Anteil3' */
  {
    ((StateDerivatives_Regelglied *) Regelglied_M->ModelData.derivs)
      ->DAnteil3_CSTATE = Soll_V_Nick;
    ((StateDerivatives_Regelglied *) Regelglied_M->ModelData.derivs)
      ->DAnteil3_CSTATE += (-2.5)*Regelglied_X.DAnteil3_CSTATE;
  }

  /* Derivatives for TransferFcn: '<S1>/D-Anteil5' */
  {
    ((StateDerivatives_Regelglied *) Regelglied_M->ModelData.derivs)
      ->DAnteil5_CSTATE = Regelglied_B.Gain3;
    ((StateDerivatives_Regelglied *) Regelglied_M->ModelData.derivs)
      ->DAnteil5_CSTATE += (-1.6666666666666667)*Regelglied_X.DAnteil5_CSTATE;
  }
}

/* Model initialize function */
void Regelglied_initialize(void)
{
  /* Registration code */

  /* initialize real-time model */
  (void) memset((void *)Regelglied_M, 0,
                sizeof(RT_MODEL_Regelglied));

  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&Regelglied_M->solverInfo,
                          &Regelglied_M->Timing.simTimeStep);
    rtsiSetTPtr(&Regelglied_M->solverInfo, &rtmGetTPtr(Regelglied_M));
    rtsiSetStepSizePtr(&Regelglied_M->solverInfo,
                       &Regelglied_M->Timing.stepSize0);
    rtsiSetdXPtr(&Regelglied_M->solverInfo, &Regelglied_M->ModelData.derivs);
    rtsiSetContStatesPtr(&Regelglied_M->solverInfo,
                         &Regelglied_M->ModelData.contStates);
    rtsiSetNumContStatesPtr(&Regelglied_M->solverInfo,
      &Regelglied_M->Sizes.numContStates);
    rtsiSetErrorStatusPtr(&Regelglied_M->solverInfo, ((const char_T **)
      (&rtmGetErrorStatus(Regelglied_M))));
    rtsiSetRTModelPtr(&Regelglied_M->solverInfo, Regelglied_M);
  }

  rtsiSetSimTimeStep(&Regelglied_M->solverInfo, MAJOR_TIME_STEP);
  Regelglied_M->ModelData.intgData.y = Regelglied_M->ModelData.odeY;
  Regelglied_M->ModelData.intgData.f[0] = Regelglied_M->ModelData.odeF[0];
  Regelglied_M->ModelData.intgData.f[1] = Regelglied_M->ModelData.odeF[1];
  Regelglied_M->ModelData.intgData.f[2] = Regelglied_M->ModelData.odeF[2];
  Regelglied_M->ModelData.contStates = ((real_T *) &Regelglied_X);
  rtsiSetSolverData(&Regelglied_M->solverInfo, (void *)
                    &Regelglied_M->ModelData.intgData);
  rtsiSetSolverName(&Regelglied_M->solverInfo,"ode3");
  rtmSetTPtr(Regelglied_M, &Regelglied_M->Timing.tArray[0]);
  Regelglied_M->Timing.stepSize0 = 0.01;
  rtmSetFirstInitCond(Regelglied_M, 1);

  /* block I/O */
  (void) memset(((void *) &Regelglied_B), 0,
                sizeof(BlockIO_Regelglied));

  /* exported global signals */
  Soll_V_Roll = 0.0;
  Xd_Roll = 0.0;
  p_anteil = 0.0;
  i_anteil = 0.0;
  d_anteil = 0.0;
  I_offset = 0.0;
  Y_roll = 0.0;
  Xd_V_Roll = 0.0;
  Soll_a_Roll = 0.0;
  Ist_a_Roll = 0.0;
  Xd_a_Roll = 0.0;
  Soll_V_Nick = 0.0;
  Xd_Nick = 0.0;
  p_anteil_nick = 0.0;
  i_anteil_nick = 0.0;
  d_anteil_nick = 0.0;
  I_offset_nick = 0.0;
  Y_Nick = 0.0;
  Soll_a_Nick = 0.0;
  Ist_a_Nick = 0.0;
  Xd_a_Nick = 0.0;
  Inti_rdy = 0U;

  /* states (continuous) */
  {
    (void) memset((void *)&Regelglied_X, 0,
                  sizeof(ContinuousStates_Regelglied));
  }

  /* states (dwork) */
  (void) memset((void *)&Regelglied_DWork, 0,
                sizeof(D_Work_Regelglied));

  /* external inputs */
  (void) memset((void *)&Regelglied_U, 0,
                sizeof(ExternalInputs_Regelglied));

  /* external outputs */
  (void) memset((void *)&Regelglied_Y, 0,
                sizeof(ExternalOutputs_Regelglied));

  /* Start for If: '<S9>/If' */
  Regelglied_DWork.If_ActiveSubsystem = -1;

  /* ConstCode for Outport: '<Root>/Out_M_Gier' */
  Regelglied_Y.Out_M_Gier = 0.0;
  Regelglied_PrevZCSigState.Integrator_Reset_ZCE = UNINITIALIZED_ZCSIG;
  Regelglied_PrevZCSigState.Integrator1_Reset_ZCE = UNINITIALIZED_ZCSIG;
  Regelglied_PrevZCSigState.Integrator_Reset_ZCE_m = UNINITIALIZED_ZCSIG;
  Regelglied_PrevZCSigState.Integrator1_Reset_ZCE_f = UNINITIALIZED_ZCSIG;

  /* InitializeConditions for TransferFcn: '<S1>/D-Anteil' */
  Regelglied_X.DAnteil_CSTATE = 0.0;

  /* InitializeConditions for Merge: '<S9>/Merge' */
  if (rtmIsFirstInitCond(Regelglied_M)) {
    Regelglied_B.Merge = 0.0;
  }

  /* End of InitializeConditions for Merge: '<S9>/Merge' */
  /* InitializeConditions for Integrator: '<S7>/Integrator' */
  Regelglied_X.Integrator_CSTATE = 0.0;

  /* InitializeConditions for TransferFcn: '<S7>/D-Anteil' */
  Regelglied_X.DAnteil_CSTATE_p = 0.0;

  /* InitializeConditions for Integrator: '<S7>/Integrator1' */
  Regelglied_X.Integrator1_CSTATE = 0.0;

  /* InitializeConditions for TransferFcn: '<S1>/D-Anteil2' */
  Regelglied_X.DAnteil2_CSTATE = 0.0;

  /* InitializeConditions for TransferFcn: '<S1>/D-Anteil4' */
  Regelglied_X.DAnteil4_CSTATE = 0.0;

  /* InitializeConditions for TransferFcn: '<S1>/D-Anteil1' */
  Regelglied_X.DAnteil1_CSTATE = 0.0;

  /* InitializeConditions for Integrator: '<S8>/Integrator' */
  Regelglied_X.Integrator_CSTATE_l = 0.0;

  /* InitializeConditions for TransferFcn: '<S8>/D-Anteil' */
  Regelglied_X.DAnteil_CSTATE_pj = 0.0;

  /* InitializeConditions for Integrator: '<S8>/Integrator1' */
  Regelglied_X.Integrator1_CSTATE_m = 0.0;

  /* InitializeConditions for TransferFcn: '<S1>/D-Anteil3' */
  Regelglied_X.DAnteil3_CSTATE = 0.0;

  /* InitializeConditions for TransferFcn: '<S1>/D-Anteil5' */
  Regelglied_X.DAnteil5_CSTATE = 0.0;

  /* set "at time zero" to false */
  if (rtmIsFirstInitCond(Regelglied_M)) {
    rtmSetFirstInitCond(Regelglied_M, 0);
  }
}

/* Model terminate function */
void Regelglied_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
