/*
 * File: Input_Kennlinie.cpp
 *
 * Code generated for Simulink model 'Input_Kennlinie'.
 *
 * Model version                  : 1.392
 * Simulink Coder version         : 8.2 (R2012a) 29-Dec-2011
 * TLC version                    : 8.2 (Dec 29 2011)
 * C/C++ source code generated on : Sun Dec 30 12:57:46 2012
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: 32-bit Generic
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 * Validation result: Passed (7), Warnings (5), Error (0)
 */

#include "Input_Kennlinie.h"
#include "Input_Kennlinie_private.h"

/* External outputs (root outports fed by signals with auto storage) */
ExternalOutputs_Input_Kennlinie Input_Kennlinie_Y;

/* Real-time model */
RT_MODEL_Input_Kennlinie Input_Kennlinie_M_;
RT_MODEL_Input_Kennlinie *const Input_Kennlinie_M = &Input_Kennlinie_M_;

/* Model step function */
void Input_Kennlinie_step(void)
{
  /* local block i/o variables */
  real_T rtb_Step1;
  real_T rtb_Step;
  real_T currentTime;
  real_T rtb_Output_d;
  real_T rtb_Output;
  real_T rtb_Output_l;

  /* Step: '<S4>/Step' */
  currentTime = Input_Kennlinie_M->Timing.t[0];
  if (currentTime < 10.0) {
    rtb_Step1 = 0.0;
  } else {
    rtb_Step1 = 2.0;
  }

  /* End of Step: '<S4>/Step' */

  /* Clock: '<S4>/Clock' */
  rtb_Step = Input_Kennlinie_M->Timing.t[0];

  /* Sum: '<S4>/Output' incorporates:
   *  Constant: '<S4>/Constant'
   *  Product: '<S4>/Product'
   *  Sum: '<S4>/Sum'
   */
  rtb_Output = (rtb_Step - 10.0) * rtb_Step1;

  /* Step: '<S3>/Step' */
  currentTime = Input_Kennlinie_M->Timing.t[0];
  if (currentTime < 10.2) {
    rtb_Step = 0.0;
  } else {
    rtb_Step = 2.0;
  }

  /* End of Step: '<S3>/Step' */

  /* Clock: '<S3>/Clock' */
  rtb_Step1 = Input_Kennlinie_M->Timing.t[0];

  /* Sum: '<S3>/Output' incorporates:
   *  Constant: '<S3>/Constant'
   *  Product: '<S3>/Product'
   *  Sum: '<S3>/Sum'
   */
  rtb_Output_l = (rtb_Step1 - 10.2) * rtb_Step;

  /* Step: '<S5>/Step' */
  currentTime = Input_Kennlinie_M->Timing.t[0];
  if (currentTime < 13.5) {
    rtb_Step = 0.0;
  } else {
    rtb_Step = 2.0;
  }

  /* End of Step: '<S5>/Step' */

  /* Clock: '<S5>/Clock' */
  rtb_Step1 = Input_Kennlinie_M->Timing.t[0];

  /* Sum: '<S5>/Output' incorporates:
   *  Constant: '<S5>/Constant'
   *  Product: '<S5>/Product'
   *  Sum: '<S5>/Sum'
   */
  rtb_Output_d = (rtb_Step1 - 13.5) * rtb_Step;

  /* Step: '<S2>/Step' */
  currentTime = Input_Kennlinie_M->Timing.t[0];
  if (currentTime < 13.7) {
    rtb_Step = 0.0;
  } else {
    rtb_Step = 2.0;
  }

  /* End of Step: '<S2>/Step' */

  /* Clock: '<S2>/Clock' */
  rtb_Step1 = Input_Kennlinie_M->Timing.t[0];

  /* Outport: '<Root>/Winkel' incorporates:
   *  Constant: '<S2>/Constant'
   *  Product: '<S2>/Product'
   *  Sum: '<S1>/Add1'
   *  Sum: '<S1>/Add3'
   *  Sum: '<S2>/Sum'
   */
  Input_Kennlinie_Y.Winkel = (rtb_Output - rtb_Output_l) - (rtb_Output_d -
    (rtb_Step1 - 13.7) * rtb_Step);

  /* Step: '<S1>/Step' */
  currentTime = Input_Kennlinie_M->Timing.t[0];
  if (currentTime < 9.8) {
    rtb_Step = 0.0;
  } else {
    rtb_Step = 0.3;
  }

  /* End of Step: '<S1>/Step' */

  /* Step: '<S1>/Step1' */
  currentTime = Input_Kennlinie_M->Timing.t[0];
  if (currentTime < 15.0) {
    rtb_Step1 = 0.0;
  } else {
    rtb_Step1 = 0.3;
  }

  /* End of Step: '<S1>/Step1' */

  /* Outport: '<Root>/Throttle' incorporates:
   *  Sum: '<S1>/Add2'
   */
  Input_Kennlinie_Y.Throttle = rtb_Step - rtb_Step1;

  /* Update absolute time for base rate */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The absolute time is the multiplication of "clockTick0"
   * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
   * overflow during the application lifespan selected.
   */
  Input_Kennlinie_M->Timing.t[0] =
    (++Input_Kennlinie_M->Timing.clockTick0) *
    Input_Kennlinie_M->Timing.stepSize0;

  {
    /* Update absolute timer for sample time: [0.01s, 0.0s] */
    /* The "clockTick1" counts the number of times the code of this task has
     * been executed. The resolution of this integer timer is 0.01, which is the step size
     * of the task. Size of "clockTick1" ensures timer will not overflow during the
     * application lifespan selected.
     */
    Input_Kennlinie_M->Timing.clockTick1++;
  }
}

/* Model initialize function */
void Input_Kennlinie_initialize(void)
{
  /* Registration code */

  /* initialize real-time model */
  (void) memset((void *)Input_Kennlinie_M, 0,
                sizeof(RT_MODEL_Input_Kennlinie));

  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&Input_Kennlinie_M->solverInfo,
                          &Input_Kennlinie_M->Timing.simTimeStep);
    rtsiSetTPtr(&Input_Kennlinie_M->solverInfo, &rtmGetTPtr(Input_Kennlinie_M));
    rtsiSetStepSizePtr(&Input_Kennlinie_M->solverInfo,
                       &Input_Kennlinie_M->Timing.stepSize0);
    rtsiSetErrorStatusPtr(&Input_Kennlinie_M->solverInfo, ((const char_T **)
      (&rtmGetErrorStatus(Input_Kennlinie_M))));
    rtsiSetRTModelPtr(&Input_Kennlinie_M->solverInfo, Input_Kennlinie_M);
  }

  rtsiSetSimTimeStep(&Input_Kennlinie_M->solverInfo, MAJOR_TIME_STEP);
  rtsiSetSolverName(&Input_Kennlinie_M->solverInfo,"FixedStepDiscrete");
  rtmSetTPtr(Input_Kennlinie_M, &Input_Kennlinie_M->Timing.tArray[0]);
  Input_Kennlinie_M->Timing.stepSize0 = 0.01;

  /* external outputs */
  (void) memset((void *)&Input_Kennlinie_Y, 0,
                sizeof(ExternalOutputs_Input_Kennlinie));
}

/* Model terminate function */
void Input_Kennlinie_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
