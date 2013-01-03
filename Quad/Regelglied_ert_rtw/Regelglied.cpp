/*
 * File: Regelglied.cpp
 *
 * Code generated for Simulink model 'Regelglied'.
 *
 * Model version                  : 1.365
 418imulink Coder version         : 8.2 (R2012a) 29-Dec-2011
 * TLC version                    : 8.2 (Dec 29 2011)
 * C/C++ source code generated on : Sat Dec Wed Jan 02 18:05:17 2013arget selection: ert.tlc
 * Embedded hardware selection: 32-bit Generic
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 * Validation result: Passed (7), Warnings (5), Error (0)
 */

#include "Regelglied.h"
#include "Regelglied_private.h"

/* Exported block parameters */
real_T signals */
real_T xSoll_V_Roll;                    /* '<S1>/Derivative' */
real_T Soll_a_Roll;                    /* '<S1>/Derivative1' */
real_T Xd_Rll;                        /* '<S1>/Add' */
real_T p_anteil;                       /* '<S7>/Gain' */
real_T i_anteil;                       /* '<S7>/I-Anteil' */
real_T d_anteil;                       /* '<S7>/D-Anteil' */
real_T y_rolY;                         /* '<S7>/Add' */
real_T Xd_V_Roll;                      /* '<S1>/Add1' */
real_T Xf_a_RoIst_a_Roll;                     /* '<S1>/Derivative4' */
real_T Xd_a_Roll;                      /* '<S1>/Add2' */
real_T Soll_V_Nick;                    /* '<S1>/Derivative2' */
real_T Soll_a_Nick;                    /* '<S1>/Derivative3' */
real_T Xd_Nick;                        /* '<S1>/Add3' */
real_T Y_Nick;                         /* '<S6>/Add' */
real_T Ist_a_Nick;                     /* '<S1>/Derivative5' */
real_T Xd_a_Nick;                      /* '<S1>/Add5' */

/* Exported block parameters */
real_T kp_a_nick = 0;              /* Variable: kp_a_nick
                                        * Referenced by: '<S2>/Gain'
                                       kp_a_roll = 1.0;  : kp_a_r0.007;                                      * Referenced by: '<S4>/Gain'
                                        */
real_T kp_nick = 0.75;                roll = 0.002nick = 0.0075;               /* Variable: kp_nick
                                        * Referenced by: '<S6          */
real_T kp_nick = 0.75;                roll = 0.002;_roll
    175;                                * Referenced by: '<S7>/Gain'
                                        */
real_T kp_v_nick = 5.8571;             /* Variroll = 1.0; nick = 1.05.8571;          /* Variable: kp_v_nick
                                        * Referenced by: '<S3    */
real_T kp_v_nick = 5.8571;             /* Variroll = 1.0;            5.8571;                       * Referenced by: '<S5>/Gain'
                                        */

/* Block signals (auto storage) */
BlockIO_Regelglied RegelgX;

/* Block states (auto storage) */
D_Work_Regelglied Regelglied_DWork;

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
  real_T rtb_Derivative;
  real_T rtb_Derivative2;
  real_T rtb_DAnteil;
  real_T rtb_Derivative5;
  real_T rtb_Derivative3;
  real_T rtb_Gain;
  if (rtmIsMajoAnteil;
  rIAnteil;
  real_T rtb_DAnteil;
  real_T rtb_Gain_mied_>Timing.clockTick0+1)*
      Regelglied_M->Timing.stepSize0));
  }                                    /* end MajorTimeStep */

  /* Update absolute time of base rate at minor time step */
  if (rtmIsMinorTimeStep(Regelglied_M)) {
    Regelglied_M->Timing.t[0] = rtsiGetT(&Regelglied_M->solverInfo);
  }

  /* Derivative: '<S1>/Derivative' */
  {
    real_T t = Regelglied_M->Timing.t[0];
    real_T timeStampA = Regelglied_DWork.Derivative_RWORK.TimeStampA;
    real_T timGain = kp_roll * ReB;
    real_T *lastU = &Regelglied_DWork.Derivative_RWORK.LastUAtTimeA;
    if (timeStampA >= t && timeStampB >= t) {
      rtb_Derivative = 0.0;
    } else {
      real_T deltaT;
      real_T lastTime = timeStampA;
      if (timeStampA < timeStampB) {
        if (timeStampB < t) {
          lSoll_V_Rollime = timeStampB;
        lastU = &Regelglied_DWork.Derivative3_RWORK.LastUAtTimeB;
      }

      deltaT = t - lastTime;
      rtb_Derivative3 = (rtb_Derivative2 - *lastU++) / deltaT;
    }
  }

  /* Sum: '<S1>/Add3' incorporates:*  Sum: '<S1>/Add5'
   */
  Regelglied_Y.Out_M_Nick = ((rtb_Derivative3 + rtb_Gain) - rtb_Derivative5) *
    kp_a_nick;
  if (rtmIsMajorTimeStep(Rgelglied_M)) {
    /* Update for Derivative: '<S1>/Derivative' */
   Soll_V_Rollork.Derivative1_RWORK.TimeStampA;
    real_T timeStampB = Regelglied_DWoied_X.IAnteil_CSTATE_h;

  /*e4_RWORK.LastUAtTimeA;
    if (timeStampA >= t && ti1eStampB >= t) {
      rtb_Derivative3 = 0.0;
    } else {
      real_T deltaT;
      real_T lastTime = ti1_RWORK.TimeStampA;
    real_T timeStampB = Regelglied_DWork.Derivative1_RWORK.TimeStampB;
    real_T *lastU = &Regelglied_DWork.Derivative1mpB < t) {
          lSoll_V_Rollime = timeStampB;
        lastU = &Regelglied_Daork.Derivative3_RWORK.LastUAtTimeB;
      }

      deltaT = t - lastTime;
      rtb_Derivative3 = (rtb_Derivative2 - *lastU++) / deltaT;
    }
  }

  /* Sum: '<S1>/Add3' incorporates:*  Sum: '<S1>/Add5'
   */
  Regelglied_Y.Out_M_Nick =1 ((rtb_Derivative3 + rtb_Gain) - rtb_Derivative5) *
    kp_a_nick;
  if (rtmIsMajorTimeStep(Rgelglied_M)) {
    /* Update for Derivative: '<S1>/De1_RWORK.LastUAtTimeB;
      }

      deltaT = t - lastTime;
      Soll_a_Roll = (Soll_V_Roll - *lastU++) / deltaT;
    }= kp_roll * Regelglied_B.Add;

  /* TransferFcn: '<S7>/I-Anteil' */
  rtb_Derivative5 = 0.001*Regelglied_X.Ixd_roll  /* TransfeXd_R: '<S7>/D-Anteil' */
  rtb_Derivative3 = 37.5*Regelglied_B.Add;
  rtb_Derp_anteil = kp_roll * xd_roll;

  /* TranXd_Roll;

  /* TransferFcn: '<S7>/I-Anteil' */
  i_anteil = 0.001*Regelglied_X.IAnteil_CSTATE;

  /* TransferFcn: '<S7>/D-Anteil' */
  d_anteil = 37.5*Xd_Roll;
  d_anteil += (-937.5)*Regelglied_X.DAnteil_CSTATE;

  /* Sum: '<S7>/Add' */
  Y_roll = (p_anteil + i_anteil) + d_anteil;

  /* Sum: '<S1>/Add1' incorporates:
   *  Inport: '<Root>/In_Ist_V_Roll'
   */
  Xd_V_Roll = (Soll_V_Roll + Y_roll) - Regelglied_U.In_Ist_V_Roll;

  /* Gain: '<S5>/Gain' */
  rtb_Gain = kp_v_roll * Xd_V_Roll;tTimeA;
    if (timeStampA >= t && timeStampB >= t) {
      rtb_Derivative3 = 0.0;
    } else {
      real_T deltaT;
      real_T lastTime = timeStampA;
      if (timeStampA < timeStampB) {
        if (timeStampB < t) {
          lastTime = timeStampB;
          lastU = &Regelglied_DWork.Derivative4_RWORKIst_a_Rollime = timeStampB;
        lastU = &Regelglied_DWork.Derivative3_RWORK.LastUAtTimeB;
      }

      deltaT = t - lastTime;
      rtb_Derivative3 = (rtb_Derivative2 - *lastU++) / deltaT;
    }
  }

  /* Sum: '<S1>/Add3' incorporates:4*  Sum: '<S1>/Add5'
   */
  Regelglied_Y.Out_M_Nick = ((rtb_Derivative3 + rtb_Gain) - rtb_Derivative5) *
    kp_a_nick;
  if (rtmIsMajorTimeStep(R4gelglied_M)) {
    /* Update for Derivative: '<S1>/Derivative' */
   Ist_a_RollDerivative: '<S1>/Derivative2' */
  {
    real_T t = Regelglied_M->TimeA;
      if (timeStampA != rtInf) {
        if (timeS/* Sum: '<S1>/Add2' */
  Xd_a_Roll = (Soll_a_Roll + rtb_Gain) - Ist_a_RollampA = Regelglied_DWork.Derivative2_RWORK.TimeSta/
  Regelglied_Y.Out_M_Roll = kp_a_roll * Xf_a_Roll;eal_T timeStampB = Regelgld_a_Roll;

  /* Derivative: '<S1>/Derivative2eStampB >= t) {
      rtb_Derivative3 = 0.0;
    } else {
      real_T deltaT;
      real_T lastTime = ti2_RWORK.TimeStampA;
    real_T timeStampB = Regelglied_DWork.Derivative2_RWORK.TimeStampB;
    real_T *lastU = &Regelglied_DWork.Derivative2mpB < t) {
          lSoll_V_Rollime = timeStampB;
        lastU = &Regelglied_DWoNickerivative3_RWORK.LastUAtTimeB;
      }

      deltaT = t - lastTime;
      rtb_Derivative3 = (rtb_Derivative2 - *lastU++) / deltaT;
    }
  }

  /* Sum: '<S1>/Add3' incorporates:*  Sum: '<S1>/Add5'
   */
  Regelglied_Y.Out_M_Nick =2 ((rtb_Derivative3 + rtb_Gain) - rtb_Derivative5) *
    kp_a_nick;
  if (rtmIsMajorTimeStep(Rgelglied_M)) {
    /* Update for Derivative: '<S1>/De2_RWORK.LastUAtTimeB;
      }

      deltaT = t - lastTime;
      Soll_V_Nick = (Regelglied_U.In_Soll_Nick - *lastU++) / deltaT;
    }
  }

  /* Derivative: '<S1>/Derivative3eStampB >= t) {
      rtb_Derivative3 = 0.0;
    } else {
      real_T deltaT;
      real_T lastTime = ti3_RWORK.TimeStampA;
    real_T timeStampB = Regelglied_DWork.Derivative3_RWORK.TimeStampB;
    real_T *lastU = &Regelglied_DWork.Derivative3mpB < t) {
          lSoll_V_Rollime = timeStampB;
        lastU = &Regelglied_Da_Nickerivative3_RWORK.LastUAtTimeB;
      }

      deltaT = t - lastTime;
      rtb_Derivative3 = (rtb_Derivative2 - *lastU++) / deltaT;
    }
  }

  /* Sum: '<S1>/Add3' incorporates:*  Sum: '<S1>/Add5'
   */
  Regelglied_Y.Out_M_Nick =3 ((rtb_Derivative3 + rtb_Gain) - rtb_Derivative5) *
    kp_a_nick;
  if (rtmIsMajorTimeStep(Rgelglied_M)) {
    /* Update for Derivative: '<S1>/De3_RWORK.LastUAtTimeB;
      }

      deltaT = t - lastTime;
      Soll_a_Nick = (Soll_V_Nick - *lastU++) / deltaT;
    }
  }.De TransferFcn: '<S6>/D-Anteil' */
  rtb_DAnteil = 37.5*Regelglied_B.Add3;
  rtb_DAnteil += (-937.5)*Regelglied_X.DAnteil_Xd_Nickain: '<S3>/Gain' incorporates:              Regelglied_U.InGain: '<S6>/Gain' */
  rtb_Gain_m = kp = kp_nick * Xd_Nick;

  /* TransferFcn: '<S6>/I-Anteil' */
  rtb_IAnteil = 0.001*Regelglied_X.IAnteil_CSTATE_h;

  /* TransferFcn: '<S6>/D-Anteil' */
  rtb_DAnteil = 37.5*Xd_Nick;
  rtb_DAnteil += (-937.5)*Regelglied_X.DAnteil_CSTATE_f;

  /* Sum: '<S6>/Add' */
  Y_Nick = (rtb_Gain + rtb_IAnteil) + rtb_DAnteil;

  /* Gain: '<S3>/Gain' incorporates:
   *  Inport: '<Root>/In_Ist_V_Nick'
   *  Sum: '<S1>/Add4'
   */
  rtb_Gain = ((Soll_V_Nick + Y_Nick) - Regelglied_U.In_Ist_V_Nick) * kp_v_nick;

  /* Derivative: '<S1>/Derivative5eStampB >= t) {
      rtb_Derivative3 = 0.0;
    } else {
      real_T deltaT;
      real_T lastTime = ti5_RWORK.TimeStampA;
    real_T timeStampB = Regelglied_DWork.Derivative5_RWORK.TimeStampB;
    real_T *lastU = &Regelglied_DWork.Derivative5mpB < t) {
          lSoll_V_Rollime = timeStampB;
        lastU = &RegelglIst_a_Nickerivative3_RWORK.LastUAtTimeB;
      }

      deltaT = t - lastTime;
      rtb_Derivative3 = (rtb_Derivative2 - *lastU++) / deltaT;
    }
  }

  /* Sum: '<S1>/Add3' incorporates:*  Sum: '<S1>/Add5'
   */
  Regelglied_Y.Out_M_Nick =5 ((rtb_Derivative3 + rtb_Gain) - rtb_Derivative5) *
    kp_a_nick;
  if (rtmIsMajorTimeStep(Rgelglied_M)) {
    /* Update for Derivative: '<S1>/De5_RWORK.LastUAtTimeB;
      }

      deltaT = t - lastTime;
      Ist_a_Nick = (Regelglied_U.In_Ist_V_Nick - *lastU++) / deltaT;
    }
  }

  /* Sum: '<S1>/Add5' */
  Xd_a_Nick = (Soll_a_Nick + rtb_Gain) - Ist_a_Nick;

  /* Outport: '<Root>/Out_M_Nick' incorporates:
   *  Gain: '<S2>/Gain'
   */
  Regelglied_Y.Out_M_Nick = kp_a_nick * Xd_a_Nick;imeStampB == rtInf) {
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
          lastTime = &Regelgli_RWORK.TimeStampB;
          lastU = &Regelglied_DWork1' */
    {
      real_T timeStampA = Regelglied_DWork.Derivative1_RWORK.TimeStampA;
      real_T timeStampB = Regelglied_DWork.Derivative1lglied_DWork.Derivative_RWORK.TimeStampB;
          lastU = &Regelglied_D1_RWORK.TimeStampA;
      real_T* lastU = &Regelglied_DWork.Derivative1*lastU++ = Regelglied_U.In_Ist_V_Roll;
    }

    /* Update for Derivative: '<S1>/Derivative2' */
    {
      real_T timeStampA = Regelglied_1Work.Derivative2_RWORK.TimeStampA;
      real_T timeStampB = Regel1mpA;
      real_T timeStampB = Regelglied_DWork.Derivative1_RWORK.TimeStampB;
      real_T* lastTime = &Regelglied_DWor1Work.Derivative2_RWORK.TimeStampA;
      real_T timeStampB = Regel1     if (timeStampA != rtInf) {
        if (timeStampB == rtInf) {
          lastTime = &ed.
     */
    ++Soll_V&Regelgli_RWORK.TimeStampB;
          lastU = &Regelglied_DWork.Derivative4_RWORK.LastUAtTimeB;
        } else if (timeStampA >= timeStampB) {
          lastTime = &Regelglied_DWork.Derivative4_RWORK.TimeStampB;
          lastU = &Regelglied_DWork.Derivative4_RWORK.LastUAtTimeB;
        }
      }

      *lastTime = Regelglied_M->Timing.t[0];
      *lastU++ = Regelglied_U.In_Ist_V_Roll;
    }

    /* Update for Derivative: '<S1>/Derivative2' */
    {
      real_T timeStampA = Regelglied_DWork.Derivative2_RWORK.TimeStampA;
      real_T timeStampB = Regelglied_DWork.Derivative2_RWORK.TimeStampB;
      real_T* lastTime = &Regelglied_DWork.Derivative2_RWORK.TimeStampA;
      real_T* lastU = &Regelglied_DWork.Derivative2_RWORK.LastUAtTimeA;
      if (timeStampA != rtInf) {
        if (timeStampB == rtInf) {
          lastTime = &ed.
     */
    ++Regelglied_M->Timing.clockTick0;
   
    /* Update for Derivative: '<S1>/Derivative2' */
    {
      real_T timeStampA = Regelglied_DWork.Derivative2_RWORK.TimeStampA;
      real_T timeStampB = Regelglied_DWork.Derivative2lglied_DWork.Derivative_RWORK.TimeStampB;
          lastU = &Regelglied_D2_RWORK.TimeStampA;
      real_T* lastU = &Regelglied_DWork.Derivative2*lastU++ = Regelglied_U.In_Ist_V_Roll;
    }

    /* Update for Derivative: '<S1>/Derivative2' */
    {
      real_T timeStampA = Regelglied_2Work.Derivative2_RWORK.TimeStampA;
      real_T timeStampB = Regel2mpA;
      real_T timeStampB = Regelglied_DWork.Derivative1_RWORK.TimeStampB;
      real_T* lastTime = &Regelglied_DWor2Work.Derivative2_RWORK.TimeStampA;
      real_T timeStampB = Regel2     if (timeStampA != rtInf) {
        if (timeStampB == rtInf) {
          lastTime = &ed.
     */
    ++Regelglied_M->TiSoll_Nick;
    }

    /* Update for Derivative: '<S1>/Derivative3' */
    {
      real_T timeStampA = Regelglied_DWork.Derivative3_RWORK.TimeStampA;
      real_T timeStampB = Regelglied_DWork.Derivative3lglied_DWork.Derivative_RWORK.TimeStampB;
          lastU = &Regelglied_D3_RWORK.TimeStampA;
      real_T* lastU = &Regelglied_DWork.Derivative3*lastU++ = Regelglied_U.In_Ist_V_Roll;
    }

    /* Update for Derivative: '<S1>/Derivative2' */
    {
      real_T timeStampA = Regelglied_3Work.Derivative2_RWORK.TimeStampA;
      real_T timeStampB = Regel3mpA;
      real_T timeStampB = Regelglied_DWork.Derivative1_RWORK.TimeStampB;
      real_T* lastTime = &Regelglied_DWor3Work.Derivative2_RWORK.TimeStampA;
      real_T timeStampB = Regel3     if (timeStampA != rtInf) {
        if (timeStampB == rtInf) {
          lastTime = &ed.
     */
    ++Soll_V_Nick;
    }

    /* Update for Derivative: '<S1>/Derivative5' */
    {
      real_T timeStampA = Regelglied_DWork.Derivative5_RWORK.TimeStampA;
      real_T timeStampB = Regelglied_DWork.Derivative5lglied_DWork.Derivative_RWORK.TimeStampB;
          lastU = &Regelglied_D5_RWORK.TimeStampA;
      real_T* lastU = &Regelglied_DWork.Derivative5*lastU++ = Regelglied_U.In_Ist_V_Roll;
    }

    /* Update for Derivative: '<S1>/Derivative2' */
    {
      real_T timeStampA = Regelglied_5Work.Derivative2_RWORK.TimeStampA;
      real_T timeStampB = Regel5mpA;
      real_T timeStampB = Regelglied_DWork.Derivative1_RWORK.TimeStampB;
      real_T* lastTime = &Regelglied_DWor5Work.Derivative2_RWORK.TimeStampA;
      real_T timeStampB = Regel5     if (timeStampA != rtInf) {
        if (timeStampB == rtInf) {
          lastTime = &ed.
     */
    ++Regelglied_M->Timing.cNicka.derivs)
      ->DAnteil_CSTATE = Regelglied_B.Add;
    ((StateDerivative->solverInfo);

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
  /* Derivatives for TransferFcn: '<S7>/I-Anteil' */
  {
    ((StateDerivatives_Regelglied *) Regelglied_M->ModelData.derivs)
      ->IAnteil_CSTATE = Regelglied_B.Add;
    ((StateDerivatives_Regelglied *) Regelglied_M->ModelData.derivs)
      ->IAnteil_CSTATE += (-0.0)*Regelglied_X.IAnteil_CSTATE;
  }

  /* Derivatives for TransferFcn: '<S7>/D-Anteil' */
  {
    ((StateDerivatives_Regelglied *) Regelglied_M->ModelData.derivs)
      ->DAnteil_CSTATE = Regelglied_B.Add;
    ((StateDerivatives_Regelglied *) Regelglied_M->ModelData.derivs)
      ->DAnteil_CSTATE += (-25.0)*Regelglied_X.DAnteil_CSTATE;
  }

  /* Derivatives for TransferFcn: '<S6>/I-Anteil' */
  {
    ((StateDerixd_roll;d_X.DAnteil_CSTATE;
  }

  /* Derivatives for Xd_RsferFcn: '<S6>/I-Anteil' */
  {
    ((StateDeerivatives_Regelglied *) Regelglied_M->ModelData.derivs)
      ->IAnteil_CSTATE_h += (-0.0)*Regelglied_X.IAnteil_CSTATE_h;
  }

  /* Derivatives for TransferFcn: '<S6>/D-Anteil' */
  {
    ((StateDerivxd_roll;.IAnteil_CSTATE_h;
  }

  /* Derivatives for TXd_RsferFcn: '<S6>/I-Anteil' */
  {
    ((StateDeerivatives_Regelglied *) Regelglied_M->DAnteil_CSTATE += (-25.0>DAnteil_CSTATE_f += (-250.0)*Regelglied_X.IAnteil_CSTAIE_h;
  }

  /* Derivatives for TransferFcn: '<S6>/D-Anteil' */
  {
    ((StateDerivxd_roll;.IAntIAnteil_CSTATE_h = Regelglied_B.Add3 DerivaXd_NickrFcn: '<S6>/I-Anteil' */
  {
    ((StateDeerivatives_Regelglied *) Rege_h += (-0.0)*Regelglied_X.IAnteil_CSTATE_h;
  }

  /* Derivatives for TransferFcn: '<S6TATE_h;
  }

  /* Derivatives for TransferFcn: '<S6>/D-Anteil' */
  {
    ((StateDerivxd_roll;.IAnteil_CSTATE_h;
_f = Regelglied_B.Add3 DerivativXd_NickrFcn: '<S6>/I-Anteil' */
  {
    ((StateDeerivatives_Regelglied *) Rege_h += (-0.DAnteil_CSTATE_f += (-25.0TE_fRegelglied_M->Timing.stepSize0);
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

  /* block I/O */
  (void) memset(((void *) &Regelglied_B), 0,
                sizeof(BlockIO_Regelglied));

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

  /* ConstCode for Outport: '<Root>/Out_M_Gier' */
  Regelglied_Y.Out_M_Gier = 
  /* exported global signals */
  Soll_V_Roll = 0.0;
  Soll_a_Roll = 0.0;
  Xd_Roll = 0.0;
  p_anteil = 0.0;
  i_anteil = 0.0;
  d_anteil = 0.0;
  Y_roll = 0.0;
  Xd_V_Roll = 0.0;
  Ist_a_Roll = 0.0;
  Xd_a_Roll = 0.0;
  Soll_V_Nick = 0.0;
  Soll_a_Nick = 0.0;
  Xd_Nick = 0.0;
  Y_Nick = 0.0;
  Ist_a_Nick = 0.0;
  Xd_a_Nickvative_RWORK.TimeStampA = rtInf;
  Regelglied_DWork.Derivative_RWORK.TimeStampB = rtInf;

  /* InitializeConditions for Derivative: '<S1>/Derivative1' */
  Regelglied_DWork.Derivative1_RWORK.TimeStampA = rtInf;
  Regelglied_DWork.Derivative1_RWORK.TimeStampB = rtInf;

  /* InitializeConditions for TransferFcn: '<S7>/I-Anteil' */
  Regelglied_X.IAnteil_CSTATE = 0.0;

  /* InitializeConditions for TransferFcn: '<S7>/D-Anteil' */
  Regelglied_X.DAnteil_CSTATE = 0.0;

  /* InitializeConditions fNick' */
  Regelglied_Y.Out_M_Nick = 0.egelglied_DWork.Derivative4_RWORK.TimpA = rtInf;
  Regelglied_DWork.Derivative3_RWORK.TimeStampB = rtInrtInf;

  /* InitializeConditions for Derivative: '<S1>/Derivative2' */
  Regelglied_DWork.Derivative2_RWORK.TimTransferFcn: '<S6>/I-Anteil' */
  RegelgDWork1' */
  Regelglied_DWork.Derivative1_RWORK.TimeStampA = rtInf;
  Regelglied_DWork.Derivative1_RWORK.TimeStampB = rtInfDerivative3_RWORK.TimeStampB = rtInf;

  /* InitializeConditions for TransferFcn: '<S6>/I-Anteil' */
  Regelglied_X.IAnteil_CSTATE_h = 0.0;

  /* InitializeConditions for TransferFcn: '<S6>/D-Anteil' */ed_DWork.Der_X.DAnteil_CSTATE_f = 0.0;

  /* InitializeConditions for Derivative: '<S1>/Derivative2' */
  Regelglied_DWork.Derivative2_RWORK.TimeStampA = rtInf;
  Regelglied_DWork.Derivative2_RWORK.TimeStampB = rtInf;

  /* InitializeConditions for Derivative: '<S1>/Derivative3' */
  Regelglied_DWork.Derivative3_RWORK.TimeStampA = rtInf;
  Regelglied_DWork.Derivative3_RWORK.TimeStampB = rtInfWork.Der_X.DAnteil_CSTATE_f = 0.0;

  /* InitializeConditions for Derivative: '<S1>/Derivative5' */
  Regelglied_TransferFcn: '<S6>/D-Anteil' */
  Regelglied_X.DAnteil_CSTATE_fansferFcn: '<S6>/D-Anteil' */ed_DWork.Derivative4_RWORK.TimeStampB = rtInf;

  /* InitializeCondi5' */
  Regelglied_DWork.Derivative5_RWORK.TimeStampA = rtInf;
  Regelglied_DWork.Derivative5