/*
 * File: Regelglied.cpp
 *
 * Code generated for Simulink model 'Regelglied'.
 *
 * Model version                  : 1.557
 * Simulink Coder version         : 8.2 (R2012a) 29-Dec-2011
 * TLC version                    : 8.2 (Dec 29 2011)
 * C/C++ source code generated on : Sun Jun 09 14:44:37 2013
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 * Validation result: Passed (7), Warnings (5), Error (0)
 */

#include "Regelglied.h"
#include "Regelglied_private.h"

/* Exported block signals */
real_T Xd_Roll;                        /* '<S1>/Add' */
real_T p_roll_m;                       /* '<S1>/Gain' */
real_T i_roll_m;                       /* '<S1>/Integrator' */
real_T d_roll_m;                       /* '<S1>/Gain2' */
real_T I_roll_offset_m;                /* '<S1>/Integrator2' */
real_T p_nick_m;                       /* '<S1>/Gain3' */
real_T i_nick_m;                       /* '<S1>/Integrator1' */
real_T d_nick_m;                       /* '<S1>/Gain5' */
real_T I_nick_offset_m;                /* '<S1>/Integrator3' */

/* Exported block parameters */
real_T d_nick = -2900.0;               /* Variable: d_nick
                                        * Referenced by: '<S1>/Gain5'
                                        */
real_T d_roll = 2300.0;                /* Variable: d_roll
                                        * Referenced by: '<S1>/Gain2'
                                        */
real_T gain_nick = 1.0;                /* Variable: gain_nick
                                        * Referenced by: '<S1>/Gain7'
                                        */
real_T gain_roll = 1.0;                /* Variable: gain_roll
                                        * Referenced by: '<S1>/Gain6'
                                        */
real_T i_nick = 3600.0;                /* Variable: i_nick
                                        * Referenced by: '<S1>/Gain4'
                                        */
real_T i_nick_offset = 2000;            /* Variable: i_nick_offset
                                        * Referenced by: '<S1>/Gain10'
                                        */
real_T i_roll = 2000.0;                /* Variable: i_roll
                                        * Referenced by: '<S1>/Gain1'
                                        */
real_T i_roll_offset = 1200;            /* Variable: i_roll_offset
                                        * Referenced by: '<S1>/Gain8'
                                        */
real_T kp_nick = 11000.0;              /* Variable: kp_nick
                                        * Referenced by: '<S1>/Gain3'
                                        */
real_T kp_roll = 7900.0;               /* Variable: kp_roll
                                        * Referenced by: '<S1>/Gain'
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
  int_T nXc = 4;
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
  int8_T rtAction;
  real_T rtb_Xd_Roll;
  real_T tmp;
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

  /* Sum: '<S1>/Add' incorporates:
   *  Inport: '<Root>/In_Ist_Roll'
   *  Inport: '<Root>/In_Soll_Roll'
   */
  Xd_Roll = Regelglied_U.In_Soll_Roll - Regelglied_U.In_Ist_Roll;

  /* Gain: '<S1>/Gain' */
  p_roll_m = kp_roll * Xd_Roll;

  /* If: '<S2>/If' incorporates:
   *  Constant: '<S3>/Constant'
   *  Constant: '<S4>/Constant'
   *  Inport: '<Root>/Throttle'
   */
  if (rtmIsMajorTimeStep(Regelglied_M)) {
    if (Regelglied_U.Throttle >= 0.05) {
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
    /* Outputs for IfAction SubSystem: '<S2>/If Action Subsystem' incorporates:
     *  ActionPort: '<S3>/Action Port'
     */
    if (rtmIsMajorTimeStep(Regelglied_M)) {
      Regelglied_B.Merge = 1.0;
    }

    /* End of Outputs for SubSystem: '<S2>/If Action Subsystem' */
    break;

   case 1:
    /* Outputs for IfAction SubSystem: '<S2>/If Action Subsystem1' incorporates:
     *  ActionPort: '<S4>/Action Port'
     */
    if (rtmIsMajorTimeStep(Regelglied_M)) {
      Regelglied_B.Merge = 0.0;
    }

    /* End of Outputs for SubSystem: '<S2>/If Action Subsystem1' */
    break;
  }

  /* End of If: '<S2>/If' */

  /* Integrator: '<S1>/Integrator' */
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

  i_roll_m = Regelglied_X.Integrator_CSTATE;
  if (rtmIsMajorTimeStep(Regelglied_M)) {
    /* Gain: '<S1>/Gain2' incorporates:
     *  Inport: '<Root>/In_Ist_V_Roll'
     */
    d_roll_m = -d_roll * Regelglied_U.In_Ist_V_Roll;
  }

  /* Integrator: '<S1>/Integrator2' */
  I_roll_offset_m = Regelglied_X.Integrator2_CSTATE;

  /* Outport: '<Root>/Out_M_Roll' incorporates:
   *  Gain: '<S1>/Gain6'
   *  Sum: '<S1>/Add2'
   */
  Regelglied_Y.Out_M_Roll = (((p_roll_m + i_roll_m) + d_roll_m) +
    I_roll_offset_m) * gain_roll;

  /* Sum: '<S1>/Add1' incorporates:
   *  Inport: '<Root>/In_Ist_Nick'
   *  Inport: '<Root>/In_Soll_Nick'
   */
  rtb_Xd_Roll = Regelglied_U.In_Soll_Nick - Regelglied_U.In_Ist_Nick;

  /* Gain: '<S1>/Gain3' */
  p_nick_m = kp_nick * rtb_Xd_Roll;

  /* Integrator: '<S1>/Integrator1' */
  if (rtmIsMajorTimeStep(Regelglied_M)) {
    ZCEventType zcEvent;
    zcEvent = rt_ZCFcn(RISING_ZERO_CROSSING,
                       &Regelglied_PrevZCSigState.Integrator1_Reset_ZCE,
                       Regelglied_B.Merge);

    /* evaluate zero-crossings */
    if (zcEvent) {
      Regelglied_X.Integrator1_CSTATE = 0.0;
    }
  }

  i_nick_m = Regelglied_X.Integrator1_CSTATE;
  if (rtmIsMajorTimeStep(Regelglied_M)) {
    /* Gain: '<S1>/Gain5' incorporates:
     *  Inport: '<Root>/In_Ist_V_Nick'
     */
    d_nick_m = -d_nick * Regelglied_U.In_Ist_V_Nick;
  }

  /* Integrator: '<S1>/Integrator3' */
  I_nick_offset_m = Regelglied_X.Integrator3_CSTATE;

  /* Outport: '<Root>/Out_M_Nick' incorporates:
   *  Gain: '<S1>/Gain7'
   *  Sum: '<S1>/Add3'
   */
  Regelglied_Y.Out_M_Nick = (((p_nick_m + i_nick_m) + d_nick_m) +
    I_nick_offset_m) * gain_nick;

  /* Gain: '<S1>/Gain1' */
  Regelglied_B.Gain1 = i_roll * Xd_Roll;

  /* Switch: '<S1>/Switch1' incorporates:
   *  Constant: '<S1>/Constant1'
   */
  if (Regelglied_B.Merge != 0.0) {
    tmp = rtb_Xd_Roll;
  } else {
    tmp = 0.0;
  }

  /* End of Switch: '<S1>/Switch1' */

  /* Gain: '<S1>/Gain10' */
  Regelglied_B.Gain10 = i_nick_offset * tmp;

  /* Gain: '<S1>/Gain4' */
  Regelglied_B.Gain4 = i_nick * rtb_Xd_Roll;

  /* Switch: '<S1>/Switch' incorporates:
   *  Constant: '<S1>/Constant'
   */
  if (Regelglied_B.Merge != 0.0) {
    tmp = Xd_Roll;
  } else {
    tmp = 0.0;
  }

  /* End of Switch: '<S1>/Switch' */

  /* Gain: '<S1>/Gain8' */
  Regelglied_B.Gain8 = i_roll_offset * tmp;
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
  StateDerivatives_Regelglied *_rtXdot;
  _rtXdot = ((StateDerivatives_Regelglied *) Regelglied_M->ModelData.derivs);

  /* Derivatives for Integrator: '<S1>/Integrator' */
  {
    ((StateDerivatives_Regelglied *) Regelglied_M->ModelData.derivs)
      ->Integrator_CSTATE = Regelglied_B.Gain1;
  }

  /* Derivatives for Integrator: '<S1>/Integrator2' */
  _rtXdot->Integrator2_CSTATE = Regelglied_B.Gain8;

  /* Derivatives for Integrator: '<S1>/Integrator1' */
  {
    ((StateDerivatives_Regelglied *) Regelglied_M->ModelData.derivs)
      ->Integrator1_CSTATE = Regelglied_B.Gain4;
  }

  /* Derivatives for Integrator: '<S1>/Integrator3' */
  _rtXdot->Integrator3_CSTATE = Regelglied_B.Gain10;
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
  Xd_Roll = 0.0;
  p_roll_m = 0.0;
  i_roll_m = 0.0;
  d_roll_m = 0.0;
  I_roll_offset_m = 0.0;
  p_nick_m = 0.0;
  i_nick_m = 0.0;
  d_nick_m = 0.0;
  I_nick_offset_m = 0.0;

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

  /* Start for If: '<S2>/If' */
  Regelglied_DWork.If_ActiveSubsystem = -1;

  /* ConstCode for Outport: '<Root>/Out_M_Gier' */
  Regelglied_Y.Out_M_Gier = 0.0;
  Regelglied_PrevZCSigState.Integrator_Reset_ZCE = UNINITIALIZED_ZCSIG;
  Regelglied_PrevZCSigState.Integrator1_Reset_ZCE = UNINITIALIZED_ZCSIG;

  /* InitializeConditions for Merge: '<S2>/Merge' */
  if (rtmIsFirstInitCond(Regelglied_M)) {
    Regelglied_B.Merge = 0.0;
  }

  /* End of InitializeConditions for Merge: '<S2>/Merge' */
  /* InitializeConditions for Integrator: '<S1>/Integrator' */
  Regelglied_X.Integrator_CSTATE = 0.0;

  /* InitializeConditions for Integrator: '<S1>/Integrator2' */
  Regelglied_X.Integrator2_CSTATE = 0.0;

  /* InitializeConditions for Integrator: '<S1>/Integrator1' */
  Regelglied_X.Integrator1_CSTATE = 0.0;

  /* InitializeConditions for Integrator: '<S1>/Integrator3' */
  Regelglied_X.Integrator3_CSTATE = 0.0;

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
