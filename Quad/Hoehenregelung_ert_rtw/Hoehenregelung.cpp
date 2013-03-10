/*
 * File: Hoehenregelung.cpp
 *
 * Code generated for Simulink model 'Hoehenregelung'.
 *
 * Model version                  : 1.512
 * Simulink Coder version         : 8.2 (R2012a) 29-Dec-2011
 * TLC version                    : 8.2 (Dec 29 2011)
 * C/C++ source code generated on : Sun Mar 10 11:02:24 2013
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 * Validation result: Passed (6), Warnings (6), Error (0)
 */

#include "Hoehenregelung.h"
#include "Hoehenregelung_private.h"

/* Exported block signals */
real_T th_dt;                          /* '<S1>/d//dt' */
real_T v_z;                            /* '<S1>/Integrator' */
real_T h_z;                            /* '<S1>/Integrator Limited' */
boolean_T h_rest_integrator;           /* '<S1>/Logical Operator' */

/* Exported block parameters */
real_T a_dead = 0.1;                   /* Variable: a_dead
                                        * Referenced by: '<S1>/Dead Zone'
                                        */
real_T mges = 0.5;                     /* Variable: mges
                                        * Referenced by: '<S3>/Gain'
                                        */
real_T throttle_d_dt = 0.3;            /* Variable: throttle_d_dt
                                        * Referenced by: '<S2>/Constant'
                                        */

/* Block signals (auto storage) */
BlockIO_Hoehenregelung Hoehenregelung_B;

/* Continuous states */
ContinuousStates_Hoehenregelung Hoehenregelung_X;

/* Block states (auto storage) */
D_Work_Hoehenregelung Hoehenregelung_DWork;

/* Previous zero-crossings (trigger) states */
PrevZCSigStates_Hoehenregelung Hoehenregelung_PrevZCSigState;

/* External inputs (root inport signals with auto storage) */
ExternalInputs_Hoehenregelung Hoehenregelung_U;

/* External outputs (root outports fed by signals with auto storage) */
ExternalOutputs_Hoehenregelung Hoehenregelung_Y;

/* Real-time model */
RT_MODEL_Hoehenregelung Hoehenregelung_M_;
RT_MODEL_Hoehenregelung *const Hoehenregelung_M = &Hoehenregelung_M_;

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
  int_T nXc = 3;
  rtsiSetSimTimeStep(si,MINOR_TIME_STEP);

  /* Save the state values at time t in y, we'll use x as ynew. */
  (void) memcpy(y, x,
                nXc*sizeof(real_T));

  /* Assumes that rtsiSetT and ModelOutputs are up-to-date */
  /* f0 = f(t,y) */
  rtsiSetdX(si, f0);
  Hoehenregelung_derivatives();

  /* f(:,2) = feval(odefile, t + hA(1), y + f*hB(:,1), args(:)(*)); */
  hB[0] = h * rt_ODE3_B[0][0];
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0]);
  }

  rtsiSetT(si, t + h*rt_ODE3_A[0]);
  rtsiSetdX(si, f1);
  Hoehenregelung_step();
  Hoehenregelung_derivatives();

  /* f(:,3) = feval(odefile, t + hA(2), y + f*hB(:,2), args(:)(*)); */
  for (i = 0; i <= 1; i++) {
    hB[i] = h * rt_ODE3_B[1][i];
  }

  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0] + f1[i]*hB[1]);
  }

  rtsiSetT(si, t + h*rt_ODE3_A[1]);
  rtsiSetdX(si, f2);
  Hoehenregelung_step();
  Hoehenregelung_derivatives();

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
void Hoehenregelung_step(void)
{
  uint8_T rtb_Compare;
  int8_T rtAction;
  if (rtmIsMajorTimeStep(Hoehenregelung_M)) {
    /* set solver stop time */
    rtsiSetSolverStopTime(&Hoehenregelung_M->solverInfo,
                          ((Hoehenregelung_M->Timing.clockTick0+1)*
      Hoehenregelung_M->Timing.stepSize0));
  }                                    /* end MajorTimeStep */

  /* Update absolute time of base rate at minor time step */
  if (rtmIsMinorTimeStep(Hoehenregelung_M)) {
    Hoehenregelung_M->Timing.t[0] = rtsiGetT(&Hoehenregelung_M->solverInfo);
  }

  /* TransferFcn: '<S1>/d//dt' */
  th_dt = 4.0*Hoehenregelung_U.Throttle;
  th_dt += (-16.0)*Hoehenregelung_X.ddt_CSTATE;

  /* RelationalOperator: '<S2>/Compare' incorporates:
   *  Abs: '<S1>/Abs'
   *  Constant: '<S2>/Constant'
   */
  rtb_Compare = (uint8_T)(fabs(th_dt) <= throttle_d_dt);

  /* If: '<S1>/If' incorporates:
   *  Inport: '<Root>/Throttle'
   *  Inport: '<S4>/In1'
   */
  if (rtmIsMajorTimeStep(Hoehenregelung_M)) {
    rtAction = 0;
    Hoehenregelung_DWork.If_ActiveSubsystem = 0;
  } else {
    rtAction = Hoehenregelung_DWork.If_ActiveSubsystem;
  }

  switch (rtAction) {
   case 0:
    /* Outputs for IfAction SubSystem: '<S1>/If Action Subsystem' incorporates:
     *  ActionPort: '<S3>/Action Port'
     */
    /* Gain: '<S3>/Gain' incorporates:
     *  Inport: '<Root>/Throttle'
     */
    Hoehenregelung_B.Merge = -mges * Hoehenregelung_U.Throttle;

    /* End of Outputs for SubSystem: '<S1>/If Action Subsystem' */
    break;

   case 1:
    /* Outputs for IfAction SubSystem: '<S1>/If Action Subsystem1' incorporates:
     *  ActionPort: '<S4>/Action Port'
     */
    Hoehenregelung_B.Merge = Hoehenregelung_U.Throttle;

    /* End of Outputs for SubSystem: '<S1>/If Action Subsystem1' */
    break;
  }

  /* End of If: '<S1>/If' */

  /* Outport: '<Root>/Schubkraft' */
  Hoehenregelung_Y.Schubkraft = Hoehenregelung_B.Merge;

  /* DeadZone: '<S1>/Dead Zone' incorporates:
   *  Inport: '<Root>/Beschleunigung_Z'
   */
  if (Hoehenregelung_U.Beschleunigung_Z > a_dead) {
    Hoehenregelung_B.DeadZone = Hoehenregelung_U.Beschleunigung_Z - a_dead;
  } else if (Hoehenregelung_U.Beschleunigung_Z >= -a_dead) {
    Hoehenregelung_B.DeadZone = 0.0;
  } else {
    Hoehenregelung_B.DeadZone = Hoehenregelung_U.Beschleunigung_Z - (-a_dead);
  }

  /* End of DeadZone: '<S1>/Dead Zone' */

  /* Logic: '<S1>/Logical Operator' */
  h_rest_integrator = !(rtb_Compare != 0);

  /* Integrator: '<S1>/Integrator' */
  if (rtmIsMajorTimeStep(Hoehenregelung_M)) {
    ZCEventType zcEvent;
    zcEvent = ((ZCEventType) (!h_rest_integrator &&
                (Hoehenregelung_PrevZCSigState.Integrator_Reset_ZCE !=
                 ZERO_ZCSIG)));

    /* evaluate zero-crossings */
    Hoehenregelung_PrevZCSigState.Integrator_Reset_ZCE = (ZCSigState)
      h_rest_integrator;
    if (zcEvent) {
      Hoehenregelung_X.Integrator_CSTATE = 0.0;
    }
  }

  v_z = Hoehenregelung_X.Integrator_CSTATE;

  /* Integrator: '<S1>/Integrator Limited' incorporates:
   *  Inport: '<Root>/Hoehe'
   */
  if (rtmIsMajorTimeStep(Hoehenregelung_M)) {
    ZCEventType zcEvent;
    zcEvent = ((ZCEventType) (!h_rest_integrator &&
                (Hoehenregelung_PrevZCSigState.IntegratorLimited_Reset_ZCE !=
                 ZERO_ZCSIG)));

    /* evaluate zero-crossings */
    Hoehenregelung_PrevZCSigState.IntegratorLimited_Reset_ZCE = (ZCSigState)
      h_rest_integrator;
    if (zcEvent || Hoehenregelung_DWork.IntegratorLimited_IWORK.IcNeedsLoading)
    {
      Hoehenregelung_X.IntegratorLimited_CSTATE = Hoehenregelung_U.Hoehe;
    }

    Hoehenregelung_DWork.IntegratorLimited_IWORK.IcNeedsLoading = 0;
  }

  h_z = Hoehenregelung_X.IntegratorLimited_CSTATE;
  if (rtmIsMajorTimeStep(Hoehenregelung_M)) {
    rt_ertODEUpdateContinuousStates(&Hoehenregelung_M->solverInfo);

    /* Update absolute time for base rate */
    /* The "clockTick0" counts the number of times the code of this task has
     * been executed. The absolute time is the multiplication of "clockTick0"
     * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
     * overflow during the application lifespan selected.
     */
    ++Hoehenregelung_M->Timing.clockTick0;
    Hoehenregelung_M->Timing.t[0] = rtsiGetSolverStopTime
      (&Hoehenregelung_M->solverInfo);

    {
      /* Update absolute timer for sample time: [0.01s, 0.0s] */
      /* The "clockTick1" counts the number of times the code of this task has
       * been executed. The resolution of this integer timer is 0.01, which is the step size
       * of the task. Size of "clockTick1" ensures timer will not overflow during the
       * application lifespan selected.
       */
      Hoehenregelung_M->Timing.clockTick1++;
    }
  }                                    /* end MajorTimeStep */
}

/* Derivatives for root system: '<Root>' */
void Hoehenregelung_derivatives(void)
{
  /* Derivatives for TransferFcn: '<S1>/d//dt' */
  {
    ((StateDerivatives_Hoehenregelung *) Hoehenregelung_M->ModelData.derivs)
      ->ddt_CSTATE = Hoehenregelung_U.Throttle;
    ((StateDerivatives_Hoehenregelung *) Hoehenregelung_M->ModelData.derivs)
      ->ddt_CSTATE += (-4.0)*Hoehenregelung_X.ddt_CSTATE;
  }

  /* Derivatives for Integrator: '<S1>/Integrator' */
  {
    ((StateDerivatives_Hoehenregelung *) Hoehenregelung_M->ModelData.derivs)
      ->Integrator_CSTATE = Hoehenregelung_B.DeadZone;
  }

  /* Derivatives for Integrator: '<S1>/Integrator Limited' */
  {
    ((StateDerivatives_Hoehenregelung *) Hoehenregelung_M->ModelData.derivs)
      ->IntegratorLimited_CSTATE = v_z;
  }
}

/* Model initialize function */
void Hoehenregelung_initialize(void)
{
  /* Registration code */

  /* initialize real-time model */
  (void) memset((void *)Hoehenregelung_M, 0,
                sizeof(RT_MODEL_Hoehenregelung));

  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&Hoehenregelung_M->solverInfo,
                          &Hoehenregelung_M->Timing.simTimeStep);
    rtsiSetTPtr(&Hoehenregelung_M->solverInfo, &rtmGetTPtr(Hoehenregelung_M));
    rtsiSetStepSizePtr(&Hoehenregelung_M->solverInfo,
                       &Hoehenregelung_M->Timing.stepSize0);
    rtsiSetdXPtr(&Hoehenregelung_M->solverInfo,
                 &Hoehenregelung_M->ModelData.derivs);
    rtsiSetContStatesPtr(&Hoehenregelung_M->solverInfo,
                         &Hoehenregelung_M->ModelData.contStates);
    rtsiSetNumContStatesPtr(&Hoehenregelung_M->solverInfo,
      &Hoehenregelung_M->Sizes.numContStates);
    rtsiSetErrorStatusPtr(&Hoehenregelung_M->solverInfo, ((const char_T **)
      (&rtmGetErrorStatus(Hoehenregelung_M))));
    rtsiSetRTModelPtr(&Hoehenregelung_M->solverInfo, Hoehenregelung_M);
  }

  rtsiSetSimTimeStep(&Hoehenregelung_M->solverInfo, MAJOR_TIME_STEP);
  Hoehenregelung_M->ModelData.intgData.y = Hoehenregelung_M->ModelData.odeY;
  Hoehenregelung_M->ModelData.intgData.f[0] = Hoehenregelung_M->ModelData.odeF[0];
  Hoehenregelung_M->ModelData.intgData.f[1] = Hoehenregelung_M->ModelData.odeF[1];
  Hoehenregelung_M->ModelData.intgData.f[2] = Hoehenregelung_M->ModelData.odeF[2];
  Hoehenregelung_M->ModelData.contStates = ((real_T *) &Hoehenregelung_X);
  rtsiSetSolverData(&Hoehenregelung_M->solverInfo, (void *)
                    &Hoehenregelung_M->ModelData.intgData);
  rtsiSetSolverName(&Hoehenregelung_M->solverInfo,"ode3");
  rtmSetTPtr(Hoehenregelung_M, &Hoehenregelung_M->Timing.tArray[0]);
  Hoehenregelung_M->Timing.stepSize0 = 0.01;
  rtmSetFirstInitCond(Hoehenregelung_M, 1);

  /* block I/O */
  (void) memset(((void *) &Hoehenregelung_B), 0,
                sizeof(BlockIO_Hoehenregelung));

  /* exported global signals */
  th_dt = 0.0;
  v_z = 0.0;
  h_z = 0.0;
  h_rest_integrator = FALSE;

  /* states (continuous) */
  {
    (void) memset((void *)&Hoehenregelung_X, 0,
                  sizeof(ContinuousStates_Hoehenregelung));
  }

  /* states (dwork) */
  (void) memset((void *)&Hoehenregelung_DWork, 0,
                sizeof(D_Work_Hoehenregelung));

  /* external inputs */
  (void) memset((void *)&Hoehenregelung_U, 0,
                sizeof(ExternalInputs_Hoehenregelung));

  /* external outputs */
  Hoehenregelung_Y.Schubkraft = 0.0;

  /* Start for If: '<S1>/If' */
  Hoehenregelung_DWork.If_ActiveSubsystem = -1;
  Hoehenregelung_PrevZCSigState.Integrator_Reset_ZCE = UNINITIALIZED_ZCSIG;
  Hoehenregelung_PrevZCSigState.IntegratorLimited_Reset_ZCE =
    UNINITIALIZED_ZCSIG;

  /* InitializeConditions for TransferFcn: '<S1>/d//dt' */
  Hoehenregelung_X.ddt_CSTATE = 0.0;

  /* InitializeConditions for Merge: '<S1>/Merge' */
  if (rtmIsFirstInitCond(Hoehenregelung_M)) {
    Hoehenregelung_B.Merge = 0.0;
  }

  /* End of InitializeConditions for Merge: '<S1>/Merge' */
  /* InitializeConditions for Integrator: '<S1>/Integrator' */
  Hoehenregelung_X.Integrator_CSTATE = 0.0;

  /* InitializeConditions for Integrator: '<S1>/Integrator Limited' */
  if (rtmIsFirstInitCond(Hoehenregelung_M)) {
    Hoehenregelung_X.IntegratorLimited_CSTATE = 0.0;
  }

  Hoehenregelung_DWork.IntegratorLimited_IWORK.IcNeedsLoading = 1;

  /* set "at time zero" to false */
  if (rtmIsFirstInitCond(Hoehenregelung_M)) {
    rtmSetFirstInitCond(Hoehenregelung_M, 0);
  }
}

/* Model terminate function */
void Hoehenregelung_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
