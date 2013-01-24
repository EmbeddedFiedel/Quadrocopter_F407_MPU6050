/*
 * File: Regelglied.cpp
 *
 * Code generated for Simulink model 'Regelglied'.
 *
 * Model version                  : 1.467
 * Simulink Coder version         : 8.2 (R2012a) 29-Dec-2011
 * TLC version                    : 8.2 (Dec 29 2011)
 * C/C++ source code generated on : Thu Jan 24 17:57:01 2013
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
real_T p_anteil;                       /* '<S6>/Gain' */
real_T i_anteil;                       /* '<S6>/Integrator' */
real_T d_anteil;                       /* '<S6>/D-Anteil' */
real_T Y_roll;                         /* '<S6>/Add' */
real_T Xd_V_Roll;                      /* '<S1>/Add1' */
real_T Xd_a_Roll;                      /* '<S1>/Add2' */
real_T Soll_V_Nick;                    /* '<S1>/Derivative2' */
real_T Xd_Nick;                        /* '<S1>/Add3' */
real_T Y_Nick;                         /* '<S7>/Add' */
real_T Xd_a_Nick;                      /* '<S1>/Add5' */
real_T Soll_V_Roll;                    /* '<S1>/Derivative' */
real_T Soll_a_Roll;                    /* '<S1>/Derivative1' */
real_T Soll_a_Nick;                    /* '<S1>/Derivative3' */
real_T Ist_a_Nick;                     /* '<S1>/Derivative5' */
real_T Ist_a_Roll;                     /* '<S1>/Derivative4' */

/* Exported block parameters */
real_T kp_a_nick = 1.0;                /* Variable: kp_a_nick
                                        * Referenced by: '<S2>/Gain'
                                        */
real_T kp_a_roll = 1.0;                /* Variable: kp_a_roll
                                        * Referenced by: '<S4>/Gain'
                                        */
real_T kp_nick = 5.5;                  /* Variable: kp_nick
                                        * Referenced by: '<S7>/Gain'
                                        */
real_T kp_roll = 5.5;                  /* Variable: kp_roll
                                        * Referenced by: '<S6>/Gain'
                                        */
real_T kp_v_nick = 0.2;                /* Variable: kp_v_nick
                                        * Referenced by: '<S3>/Gain'
                                        */
real_T kp_v_roll = 0.2;                /* Variable: kp_v_roll
                                        * Referenced by: '<S5>/Gain'
                                        */
real_T tn_nick = 7.5;                  /* Variable: tn_nick
                                        * Referenced by: '<S7>/Gain1'
                                        */
real_T tn_roll = 7.5;                  /* Variable: tn_roll
                                        * Referenced by: '<S6>/Gain1'
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
  /* local block i/o variables */
  real_T rtb_i_anteil_nick;
  real_T rtb_d_anteil_nick;
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

  /* Sum: '<S1>/Add' incorporates:
   *  Inport: '<Root>/In_Ist_Roll'
   *  Inport: '<Root>/In_Soll_Roll'
   */
  Xd_Roll = Regelglied_U.In_Soll_Roll - Regelglied_U.In_Ist_Roll;

  /* Gain: '<S6>/Gain' */
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

  /* If: '<S8>/If' incorporates:
   *  Constant: '<S10>/Constant'
   *  Constant: '<S9>/Constant'
   */
  if (rtmIsMajorTimeStep(Regelglied_M)) {
    if (rtb_Saturation >= 0.025) {
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
    /* Outputs for IfAction SubSystem: '<S8>/If Action Subsystem' incorporates:
     *  ActionPort: '<S9>/Action Port'
     */
    if (rtmIsMajorTimeStep(Regelglied_M)) {
      Regelglied_B.Merge = 1.0;
    }

    /* End of Outputs for SubSystem: '<S8>/If Action Subsystem' */
    break;

   case 1:
    /* Outputs for IfAction SubSystem: '<S8>/If Action Subsystem1' incorporates:
     *  ActionPort: '<S10>/Action Port'
     */
    if (rtmIsMajorTimeStep(Regelglied_M)) {
      Regelglied_B.Merge = 0.0;
    }

    /* End of Outputs for SubSystem: '<S8>/If Action Subsystem1' */
    break;
  }

  /* End of If: '<S8>/If' */

  /* Integrator: '<S6>/Integrator' */
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

  /* TransferFcn: '<S6>/D-Anteil' */
  d_anteil = 1.4*Xd_Roll;
  d_anteil += (-140.0)*Regelglied_X.DAnteil_CSTATE;

  /* Sum: '<S6>/Add' */
  Y_roll = (p_anteil + i_anteil) + d_anteil;

  /* Sum: '<S1>/Add1' */
  Xd_V_Roll = (0.0 + Y_roll) - 0.0;

  /* Sum: '<S1>/Add2' incorporates:
   *  Gain: '<S5>/Gain'
   */
  Xd_a_Roll = (kp_v_roll * Xd_V_Roll + 0.0) - 0.0;

  /* Outport: '<Root>/Out_M_Roll' incorporates:
   *  Gain: '<S4>/Gain'
   */
  Regelglied_Y.Out_M_Roll = kp_a_roll * Xd_a_Roll;

  /* Derivative: '<S1>/Derivative2' */
  {
    real_T t = Regelglied_M->Timing.t[0];
    real_T timeStampA = Regelglied_DWork.Derivative2_RWORK.TimeStampA;
    real_T timeStampB = Regelglied_DWork.Derivative2_RWORK.TimeStampB;
    real_T *lastU = &Regelglied_DWork.Derivative2_RWORK.LastUAtTimeA;
    if (timeStampA >= t && timeStampB >= t) {
      Soll_V_Nick = 0.0;
    } else {
      real_T deltaT;
      real_T lastTime = timeStampA;
      if (timeStampA < timeStampB) {
        if (timeStampB < t) {
          lastTime = timeStampB;
          lastU = &Regelglied_DWork.Derivative2_RWORK.LastUAtTimeB;
        }
      } else if (timeStampA >= t) {
        lastTime = timeStampB;
        lastU = &Regelglied_DWork.Derivative2_RWORK.LastUAtTimeB;
      }

      deltaT = t - lastTime;
      Soll_V_Nick = (Regelglied_U.In_Soll_Nick - *lastU++) / deltaT;
    }
  }

  /* Sum: '<S1>/Add3' incorporates:
   *  Inport: '<Root>/In_Ist_Nick'
   *  Inport: '<Root>/In_Soll_Nick'
   */
  Xd_Nick = Regelglied_U.In_Soll_Nick - Regelglied_U.In_Ist_Nick;

  /* Gain: '<S7>/Gain' */
  rtb_Saturation = kp_nick * Xd_Nick;

  /* Integrator: '<S7>/Integrator' */
  if (rtmIsMajorTimeStep(Regelglied_M)) {
    ZCEventType zcEvent;
    zcEvent = rt_ZCFcn(RISING_ZERO_CROSSING,
                       &Regelglied_PrevZCSigState.Integrator_Reset_ZCE_h,
                       Regelglied_B.Merge);

    /* evaluate zero-crossings */
    if (zcEvent) {
      Regelglied_X.Integrator_CSTATE_p = 0.0;
    }
  }

  rtb_i_anteil_nick = Regelglied_X.Integrator_CSTATE_p;

  /* TransferFcn: '<S7>/D-Anteil' */
  rtb_d_anteil_nick = 150.0*Xd_Nick;
  rtb_d_anteil_nick += (-15000.0)*Regelglied_X.DAnteil_CSTATE_n;

  /* Sum: '<S7>/Add' */
  Y_Nick = (rtb_Saturation + rtb_i_anteil_nick) + rtb_d_anteil_nick;

  /* Sum: '<S1>/Add5' incorporates:
   *  Gain: '<S3>/Gain'
   *  Inport: '<Root>/In_Ist_V_Nick'
   *  Sum: '<S1>/Add4'
   */
  Xd_a_Nick = (((Soll_V_Nick + Y_Nick) - Regelglied_U.In_Ist_V_Nick) * kp_v_nick
               + 0.0) - 0.0;

  /* Outport: '<Root>/Out_M_Nick' incorporates:
   *  Gain: '<S2>/Gain'
   */
  Regelglied_Y.Out_M_Nick = kp_a_nick * Xd_a_Nick;

  /* Derivative: '<S1>/Derivative' */
  {
    real_T t = Regelglied_M->Timing.t[0];
    real_T timeStampA = Regelglied_DWork.Derivative_RWORK.TimeStampA;
    real_T timeStampB = Regelglied_DWork.Derivative_RWORK.TimeStampB;
    real_T *lastU = &Regelglied_DWork.Derivative_RWORK.LastUAtTimeA;
    if (timeStampA >= t && timeStampB >= t) {
      Soll_V_Roll = 0.0;
    } else {
      real_T deltaT;
      real_T lastTime = timeStampA;
      if (timeStampA < timeStampB) {
        if (timeStampB < t) {
          lastTime = timeStampB;
          lastU = &Regelglied_DWork.Derivative_RWORK.LastUAtTimeB;
        }
      } else if (timeStampA >= t) {
        lastTime = timeStampB;
        lastU = &Regelglied_DWork.Derivative_RWORK.LastUAtTimeB;
      }

      deltaT = t - lastTime;
      Soll_V_Roll = (Regelglied_U.In_Soll_Roll - *lastU++) / deltaT;
    }
  }

  /* Derivative: '<S1>/Derivative1' */
  {
    real_T t = Regelglied_M->Timing.t[0];
    real_T timeStampA = Regelglied_DWork.Derivative1_RWORK.TimeStampA;
    real_T timeStampB = Regelglied_DWork.Derivative1_RWORK.TimeStampB;
    real_T *lastU = &Regelglied_DWork.Derivative1_RWORK.LastUAtTimeA;
    if (timeStampA >= t && timeStampB >= t) {
      Soll_a_Roll = 0.0;
    } else {
      real_T deltaT;
      real_T lastTime = timeStampA;
      if (timeStampA < timeStampB) {
        if (timeStampB < t) {
          lastTime = timeStampB;
          lastU = &Regelglied_DWork.Derivative1_RWORK.LastUAtTimeB;
        }
      } else if (timeStampA >= t) {
        lastTime = timeStampB;
        lastU = &Regelglied_DWork.Derivative1_RWORK.LastUAtTimeB;
      }

      deltaT = t - lastTime;
      Soll_a_Roll = (Soll_V_Roll - *lastU++) / deltaT;
    }
  }

  /* Derivative: '<S1>/Derivative3' */
  {
    real_T t = Regelglied_M->Timing.t[0];
    real_T timeStampA = Regelglied_DWork.Derivative3_RWORK.TimeStampA;
    real_T timeStampB = Regelglied_DWork.Derivative3_RWORK.TimeStampB;
    real_T *lastU = &Regelglied_DWork.Derivative3_RWORK.LastUAtTimeA;
    if (timeStampA >= t && timeStampB >= t) {
      Soll_a_Nick = 0.0;
    } else {
      real_T deltaT;
      real_T lastTime = timeStampA;
      if (timeStampA < timeStampB) {
        if (timeStampB < t) {
          lastTime = timeStampB;
          lastU = &Regelglied_DWork.Derivative3_RWORK.LastUAtTimeB;
        }
      } else if (timeStampA >= t) {
        lastTime = timeStampB;
        lastU = &Regelglied_DWork.Derivative3_RWORK.LastUAtTimeB;
      }

      deltaT = t - lastTime;
      Soll_a_Nick = (Soll_V_Nick - *lastU++) / deltaT;
    }
  }

  /* Derivative: '<S1>/Derivative5' */
  {
    real_T t = Regelglied_M->Timing.t[0];
    real_T timeStampA = Regelglied_DWork.Derivative5_RWORK.TimeStampA;
    real_T timeStampB = Regelglied_DWork.Derivative5_RWORK.TimeStampB;
    real_T *lastU = &Regelglied_DWork.Derivative5_RWORK.LastUAtTimeA;
    if (timeStampA >= t && timeStampB >= t) {
      Ist_a_Nick = 0.0;
    } else {
      real_T deltaT;
      real_T lastTime = timeStampA;
      if (timeStampA < timeStampB) {
        if (timeStampB < t) {
          lastTime = timeStampB;
          lastU = &Regelglied_DWork.Derivative5_RWORK.LastUAtTimeB;
        }
      } else if (timeStampA >= t) {
        lastTime = timeStampB;
        lastU = &Regelglied_DWork.Derivative5_RWORK.LastUAtTimeB;
      }

      deltaT = t - lastTime;
      Ist_a_Nick = (Regelglied_U.In_Ist_V_Nick - *lastU++) / deltaT;
    }
  }

  /* Gain: '<S6>/Gain1' */
  Regelglied_B.Gain1 = 1.0 / tn_roll * Xd_Roll;

  /* Gain: '<S7>/Gain1' */
  Regelglied_B.Gain1_m = 1.0 / tn_nick * Xd_Nick;

  /* Derivative: '<S1>/Derivative4' */
  {
    real_T t = Regelglied_M->Timing.t[0];
    real_T timeStampA = Regelglied_DWork.Derivative4_RWORK.TimeStampA;
    real_T timeStampB = Regelglied_DWork.Derivative4_RWORK.TimeStampB;
    real_T *lastU = &Regelglied_DWork.Derivative4_RWORK.LastUAtTimeA;
    if (timeStampA >= t && timeStampB >= t) {
      Ist_a_Roll = 0.0;
    } else {
      real_T deltaT;
      real_T lastTime = timeStampA;
      if (timeStampA < timeStampB) {
        if (timeStampB < t) {
          lastTime = timeStampB;
          lastU = &Regelglied_DWork.Derivative4_RWORK.LastUAtTimeB;
        }
      } else if (timeStampA >= t) {
        lastTime = timeStampB;
        lastU = &Regelglied_DWork.Derivative4_RWORK.LastUAtTimeB;
      }

      deltaT = t - lastTime;
      Ist_a_Roll = (Regelglied_U.In_Ist_V_Roll - *lastU++) / deltaT;
    }
  }

  if (rtmIsMajorTimeStep(Regelglied_M)) {
    /* Update for Derivative: '<S1>/Derivative2' */
    {
      real_T timeStampA = Regelglied_DWork.Derivative2_RWORK.TimeStampA;
      real_T timeStampB = Regelglied_DWork.Derivative2_RWORK.TimeStampB;
      real_T* lastTime = &Regelglied_DWork.Derivative2_RWORK.TimeStampA;
      real_T* lastU = &Regelglied_DWork.Derivative2_RWORK.LastUAtTimeA;
      if (timeStampA != rtInf) {
        if (timeStampB == rtInf) {
          lastTime = &Regelglied_DWork.Derivative2_RWORK.TimeStampB;
          lastU = &Regelglied_DWork.Derivative2_RWORK.LastUAtTimeB;
        } else if (timeStampA >= timeStampB) {
          lastTime = &Regelglied_DWork.Derivative2_RWORK.TimeStampB;
          lastU = &Regelglied_DWork.Derivative2_RWORK.LastUAtTimeB;
        }
      }

      *lastTime = Regelglied_M->Timing.t[0];
      *lastU++ = Regelglied_U.In_Soll_Nick;
    }

    /* Update for Derivative: '<S1>/Derivative' */
    {
      real_T timeStampA = Regelglied_DWork.Derivative_RWORK.TimeStampA;
      real_T timeStampB = Regelglied_DWork.Derivative_RWORK.TimeStampB;
      real_T* lastTime = &Regelglied_DWork.Derivative_RWORK.TimeStampA;
      real_T* lastU = &Regelglied_DWork.Derivative_RWORK.LastUAtTimeA;
      if (timeStampA != rtInf) {
        if (timeStampB == rtInf) {
          lastTime = &Regelglied_DWork.Derivative_RWORK.TimeStampB;
          lastU = &Regelglied_DWork.Derivative_RWORK.LastUAtTimeB;
        } else if (timeStampA >= timeStampB) {
          lastTime = &Regelglied_DWork.Derivative_RWORK.TimeStampB;
          lastU = &Regelglied_DWork.Derivative_RWORK.LastUAtTimeB;
        }
      }

      *lastTime = Regelglied_M->Timing.t[0];
      *lastU++ = Regelglied_U.In_Soll_Roll;
    }

    /* Update for Derivative: '<S1>/Derivative1' */
    {
      real_T timeStampA = Regelglied_DWork.Derivative1_RWORK.TimeStampA;
      real_T timeStampB = Regelglied_DWork.Derivative1_RWORK.TimeStampB;
      real_T* lastTime = &Regelglied_DWork.Derivative1_RWORK.TimeStampA;
      real_T* lastU = &Regelglied_DWork.Derivative1_RWORK.LastUAtTimeA;
      if (timeStampA != rtInf) {
        if (timeStampB == rtInf) {
          lastTime = &Regelglied_DWork.Derivative1_RWORK.TimeStampB;
          lastU = &Regelglied_DWork.Derivative1_RWORK.LastUAtTimeB;
        } else if (timeStampA >= timeStampB) {
          lastTime = &Regelglied_DWork.Derivative1_RWORK.TimeStampB;
          lastU = &Regelglied_DWork.Derivative1_RWORK.LastUAtTimeB;
        }
      }

      *lastTime = Regelglied_M->Timing.t[0];
      *lastU++ = Soll_V_Roll;
    }

    /* Update for Derivative: '<S1>/Derivative3' */
    {
      real_T timeStampA = Regelglied_DWork.Derivative3_RWORK.TimeStampA;
      real_T timeStampB = Regelglied_DWork.Derivative3_RWORK.TimeStampB;
      real_T* lastTime = &Regelglied_DWork.Derivative3_RWORK.TimeStampA;
      real_T* lastU = &Regelglied_DWork.Derivative3_RWORK.LastUAtTimeA;
      if (timeStampA != rtInf) {
        if (timeStampB == rtInf) {
          lastTime = &Regelglied_DWork.Derivative3_RWORK.TimeStampB;
          lastU = &Regelglied_DWork.Derivative3_RWORK.LastUAtTimeB;
        } else if (timeStampA >= timeStampB) {
          lastTime = &Regelglied_DWork.Derivative3_RWORK.TimeStampB;
          lastU = &Regelglied_DWork.Derivative3_RWORK.LastUAtTimeB;
        }
      }

      *lastTime = Regelglied_M->Timing.t[0];
      *lastU++ = Soll_V_Nick;
    }

    /* Update for Derivative: '<S1>/Derivative5' */
    {
      real_T timeStampA = Regelglied_DWork.Derivative5_RWORK.TimeStampA;
      real_T timeStampB = Regelglied_DWork.Derivative5_RWORK.TimeStampB;
      real_T* lastTime = &Regelglied_DWork.Derivative5_RWORK.TimeStampA;
      real_T* lastU = &Regelglied_DWork.Derivative5_RWORK.LastUAtTimeA;
      if (timeStampA != rtInf) {
        if (timeStampB == rtInf) {
          lastTime = &Regelglied_DWork.Derivative5_RWORK.TimeStampB;
          lastU = &Regelglied_DWork.Derivative5_RWORK.LastUAtTimeB;
        } else if (timeStampA >= timeStampB) {
          lastTime = &Regelglied_DWork.Derivative5_RWORK.TimeStampB;
          lastU = &Regelglied_DWork.Derivative5_RWORK.LastUAtTimeB;
        }
      }

      *lastTime = Regelglied_M->Timing.t[0];
      *lastU++ = Regelglied_U.In_Ist_V_Nick;
    }

    /* Update for Derivative: '<S1>/Derivative4' */
    {
      real_T timeStampA = Regelglied_DWork.Derivative4_RWORK.TimeStampA;
      real_T timeStampB = Regelglied_DWork.Derivative4_RWORK.TimeStampB;
      real_T* lastTime = &Regelglied_DWork.Derivative4_RWORK.TimeStampA;
      real_T* lastU = &Regelglied_DWork.Derivative4_RWORK.LastUAtTimeA;
      if (timeStampA != rtInf) {
        if (timeStampB == rtInf) {
          lastTime = &Regelglied_DWork.Derivative4_RWORK.TimeStampB;
          lastU = &Regelglied_DWork.Derivative4_RWORK.LastUAtTimeB;
        } else if (timeStampA >= timeStampB) {
          lastTime = &Regelglied_DWork.Derivative4_RWORK.TimeStampB;
          lastU = &Regelglied_DWork.Derivative4_RWORK.LastUAtTimeB;
        }
      }

      *lastTime = Regelglied_M->Timing.t[0];
      *lastU++ = Regelglied_U.In_Ist_V_Roll;
    }
  }                                    /* end MajorTimeStep */

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
  /* Derivatives for Integrator: '<S6>/Integrator' */
  {
    ((StateDerivatives_Regelglied *) Regelglied_M->ModelData.derivs)
      ->Integrator_CSTATE = Regelglied_B.Gain1;
  }

  /* Derivatives for TransferFcn: '<S6>/D-Anteil' */
  {
    ((StateDerivatives_Regelglied *) Regelglied_M->ModelData.derivs)
      ->DAnteil_CSTATE = Xd_Roll;
    ((StateDerivatives_Regelglied *) Regelglied_M->ModelData.derivs)
      ->DAnteil_CSTATE += (-100.0)*Regelglied_X.DAnteil_CSTATE;
  }

  /* Derivatives for Integrator: '<S7>/Integrator' */
  {
    ((StateDerivatives_Regelglied *) Regelglied_M->ModelData.derivs)
      ->Integrator_CSTATE_p = Regelglied_B.Gain1_m;
  }

  /* Derivatives for TransferFcn: '<S7>/D-Anteil' */
  {
    ((StateDerivatives_Regelglied *) Regelglied_M->ModelData.derivs)
      ->DAnteil_CSTATE_n = Xd_Nick;
    ((StateDerivatives_Regelglied *) Regelglied_M->ModelData.derivs)
      ->DAnteil_CSTATE_n += (-100.0)*Regelglied_X.DAnteil_CSTATE_n;
  }
}

/* Model initialize function */
void Regelglied_initialize(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

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
  p_anteil = 0.0;
  i_anteil = 0.0;
  d_anteil = 0.0;
  Y_roll = 0.0;
  Xd_V_Roll = 0.0;
  Xd_a_Roll = 0.0;
  Soll_V_Nick = 0.0;
  Xd_Nick = 0.0;
  Y_Nick = 0.0;
  Xd_a_Nick = 0.0;
  Soll_V_Roll = 0.0;
  Soll_a_Roll = 0.0;
  Soll_a_Nick = 0.0;
  Ist_a_Nick = 0.0;
  Ist_a_Roll = 0.0;

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

  /* Start for If: '<S8>/If' */
  Regelglied_DWork.If_ActiveSubsystem = -1;

  /* ConstCode for Outport: '<Root>/Out_M_Gier' */
  Regelglied_Y.Out_M_Gier = 0.0;
  Regelglied_PrevZCSigState.Integrator_Reset_ZCE = UNINITIALIZED_ZCSIG;
  Regelglied_PrevZCSigState.Integrator_Reset_ZCE_h = UNINITIALIZED_ZCSIG;

  /* InitializeConditions for Merge: '<S8>/Merge' */
  if (rtmIsFirstInitCond(Regelglied_M)) {
    Regelglied_B.Merge = 0.0;
  }

  /* End of InitializeConditions for Merge: '<S8>/Merge' */
  /* InitializeConditions for Integrator: '<S6>/Integrator' */
  Regelglied_X.Integrator_CSTATE = 0.0;

  /* InitializeConditions for TransferFcn: '<S6>/D-Anteil' */
  Regelglied_X.DAnteil_CSTATE = 0.0;

  /* InitializeConditions for Derivative: '<S1>/Derivative2' */
  Regelglied_DWork.Derivative2_RWORK.TimeStampA = rtInf;
  Regelglied_DWork.Derivative2_RWORK.TimeStampB = rtInf;

  /* InitializeConditions for Integrator: '<S7>/Integrator' */
  Regelglied_X.Integrator_CSTATE_p = 0.0;

  /* InitializeConditions for TransferFcn: '<S7>/D-Anteil' */
  Regelglied_X.DAnteil_CSTATE_n = 0.0;

  /* InitializeConditions for Derivative: '<S1>/Derivative' */
  Regelglied_DWork.Derivative_RWORK.TimeStampA = rtInf;
  Regelglied_DWork.Derivative_RWORK.TimeStampB = rtInf;

  /* InitializeConditions for Derivative: '<S1>/Derivative1' */
  Regelglied_DWork.Derivative1_RWORK.TimeStampA = rtInf;
  Regelglied_DWork.Derivative1_RWORK.TimeStampB = rtInf;

  /* InitializeConditions for Derivative: '<S1>/Derivative3' */
  Regelglied_DWork.Derivative3_RWORK.TimeStampA = rtInf;
  Regelglied_DWork.Derivative3_RWORK.TimeStampB = rtInf;

  /* InitializeConditions for Derivative: '<S1>/Derivative5' */
  Regelglied_DWork.Derivative5_RWORK.TimeStampA = rtInf;
  Regelglied_DWork.Derivative5_RWORK.TimeStampB = rtInf;

  /* InitializeConditions for Derivative: '<S1>/Derivative4' */
  Regelglied_DWork.Derivative4_RWORK.TimeStampA = rtInf;
  Regelglied_DWork.Derivative4_RWORK.TimeStampB = rtInf;

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
