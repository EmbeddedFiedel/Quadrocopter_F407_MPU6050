/*
 * File: Schubverteilung0.cpp
 *
 * Code generated for Simulink model 'Schubverteilung0'.
 *
 * Model version                  : 1.467
 * Simulink Coder version         : 8.2 (R2012a) 29-Dec-2011
 * TLC version                    : 8.2 (Dec 29 2011)
 * C/C++ source code generated on : Thu Jan 24 17:57:57 2013
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 * Validation result: Passed (8), Warnings (4), Error (0)
 */

#include "Schubverteilung0.h"
#include "Schubverteilung0_private.h"

/* Block signals (auto storage) */
BlockIO_Schubverteilung0 Schubverteilung0_B;

/* External inputs (root inport signals with auto storage) */
ExternalInputs_Schubverteilung0 Schubverteilung0_U;

/* External outputs (root outports fed by signals with auto storage) */
ExternalOutputs_Schubverteilung Schubverteilung0_Y;

/* Real-time model */
RT_MODEL_Schubverteilung0 Schubverteilung0_M_;
RT_MODEL_Schubverteilung0 *const Schubverteilung0_M = &Schubverteilung0_M_;

/*
 * Output and update for action system:
 *    '<S2>/If Action Subsystem'
 *    '<S3>/If Action Subsystem'
 *    '<S4>/If Action Subsystem'
 *    '<S5>/If Action Subsystem'
 *    '<S6>/If Action Subsystem'
 *    '<S7>/If Action Subsystem'
 *    '<S8>/If Action Subsystem'
 *    '<S9>/If Action Subsystem'
 */
void Schubverteilu_IfActionSubsystem(real_T rtu_0, real_T *rty_Out1)
{
  /* Inport: '<S10>/In1' */
  *rty_Out1 = rtu_0;
}

/* Model step function */
void Schubverteilung0_step(void)
{
  /* local block i/o variables */
  real_T rtb_Add4;
  real_T rtb_Add5;
  real_T rtb_Add6;
  real_T rtb_Add7;
  real_T rtb_Saturation;
  real_T rtb_Gain4;

  /* Saturate: '<S1>/Saturation' incorporates:
   *  Inport: '<Root>/In_Throttle'
   */
  if (Schubverteilung0_U.In_Throttle >= 1.0) {
    rtb_Saturation = 1.0;
  } else if (Schubverteilung0_U.In_Throttle <= 0.0) {
    rtb_Saturation = 0.0;
  } else {
    rtb_Saturation = Schubverteilung0_U.In_Throttle;
  }

  /* End of Saturate: '<S1>/Saturation' */

  /* Gain: '<S1>/Gain4' */
  rtb_Gain4 = faktor * 9.81 / 1000.0 * 418.5561064490916 * rtb_Saturation;

  /* Sum: '<S1>/Add4' incorporates:
   *  Gain: '<S1>/Gain'
   *  Inport: '<Root>/In_M_Gier'
   *  Inport: '<Root>/In_M_Nick'
   *  Inport: '<Root>/In_M_Roll'
   *  Sum: '<S1>/Add'
   */
  rtb_Add4 = ((Schubverteilung0_U.In_M_Roll + Schubverteilung0_U.In_M_Nick) +
              Schubverteilung0_U.In_M_Gier) * 0.25 + rtb_Gain4;

  /* If: '<S6>/If' incorporates:
   *  Constant: '<S19>/Constant'
   */
  if (rtb_Saturation >= 0.025) {
    /* Outputs for IfAction SubSystem: '<S6>/If Action Subsystem' incorporates:
     *  ActionPort: '<S18>/Action Port'
     */
    Schubverteilu_IfActionSubsystem(rtb_Add4, &Schubverteilung0_B.Merge);

    /* End of Outputs for SubSystem: '<S6>/If Action Subsystem' */
  } else {
    /* Outputs for IfAction SubSystem: '<S6>/If Action Subsystem1' incorporates:
     *  ActionPort: '<S19>/Action Port'
     */
    Schubverteilung0_B.Merge = 0.0;

    /* End of Outputs for SubSystem: '<S6>/If Action Subsystem1' */
  }

  /* End of If: '<S6>/If' */

  /* If: '<S2>/If' incorporates:
   *  Constant: '<S11>/Constant'
   */
  if (Schubverteilung0_B.Merge >= 0.0) {
    /* Outputs for IfAction SubSystem: '<S2>/If Action Subsystem' incorporates:
     *  ActionPort: '<S10>/Action Port'
     */
    Schubverteilu_IfActionSubsystem(Schubverteilung0_B.Merge,
      &Schubverteilung0_B.Merge_m);

    /* End of Outputs for SubSystem: '<S2>/If Action Subsystem' */
  } else {
    /* Outputs for IfAction SubSystem: '<S2>/If Action Subsystem1' incorporates:
     *  ActionPort: '<S11>/Action Port'
     */
    Schubverteilung0_B.Merge_m = 0.0;

    /* End of Outputs for SubSystem: '<S2>/If Action Subsystem1' */
  }

  /* End of If: '<S2>/If' */

  /* Outport: '<Root>/Out_F_A' */
  Schubverteilung0_Y.Out_F_A = Schubverteilung0_B.Merge_m;

  /* Sum: '<S1>/Add5' incorporates:
   *  Gain: '<S1>/Gain1'
   *  Inport: '<Root>/In_M_Gier'
   *  Inport: '<Root>/In_M_Nick'
   *  Inport: '<Root>/In_M_Roll'
   *  Sum: '<S1>/Add1'
   */
  rtb_Add5 = ((Schubverteilung0_U.In_M_Nick - Schubverteilung0_U.In_M_Roll) -
              Schubverteilung0_U.In_M_Gier) * 0.25 + rtb_Gain4;

  /* If: '<S7>/If' incorporates:
   *  Constant: '<S21>/Constant'
   */
  if (rtb_Saturation >= 0.025) {
    /* Outputs for IfAction SubSystem: '<S7>/If Action Subsystem' incorporates:
     *  ActionPort: '<S20>/Action Port'
     */
    Schubverteilu_IfActionSubsystem(rtb_Add5, &Schubverteilung0_B.Merge_d);

    /* End of Outputs for SubSystem: '<S7>/If Action Subsystem' */
  } else {
    /* Outputs for IfAction SubSystem: '<S7>/If Action Subsystem1' incorporates:
     *  ActionPort: '<S21>/Action Port'
     */
    Schubverteilung0_B.Merge_d = 0.0;

    /* End of Outputs for SubSystem: '<S7>/If Action Subsystem1' */
  }

  /* End of If: '<S7>/If' */

  /* If: '<S3>/If' incorporates:
   *  Constant: '<S13>/Constant'
   */
  if (Schubverteilung0_B.Merge_d >= 0.0) {
    /* Outputs for IfAction SubSystem: '<S3>/If Action Subsystem' incorporates:
     *  ActionPort: '<S12>/Action Port'
     */
    Schubverteilu_IfActionSubsystem(Schubverteilung0_B.Merge_d,
      &Schubverteilung0_B.Merge_g);

    /* End of Outputs for SubSystem: '<S3>/If Action Subsystem' */
  } else {
    /* Outputs for IfAction SubSystem: '<S3>/If Action Subsystem1' incorporates:
     *  ActionPort: '<S13>/Action Port'
     */
    Schubverteilung0_B.Merge_g = 0.0;

    /* End of Outputs for SubSystem: '<S3>/If Action Subsystem1' */
  }

  /* End of If: '<S3>/If' */

  /* Outport: '<Root>/Out_F_B' */
  Schubverteilung0_Y.Out_F_B = Schubverteilung0_B.Merge_g;

  /* Sum: '<S1>/Add6' incorporates:
   *  Gain: '<S1>/Gain2'
   *  Inport: '<Root>/In_M_Gier'
   *  Inport: '<Root>/In_M_Nick'
   *  Inport: '<Root>/In_M_Roll'
   *  Sum: '<S1>/Add2'
   */
  rtb_Add6 = ((Schubverteilung0_U.In_M_Gier - Schubverteilung0_U.In_M_Roll) -
              Schubverteilung0_U.In_M_Nick) * 0.25 + rtb_Gain4;

  /* If: '<S8>/If' incorporates:
   *  Constant: '<S23>/Constant'
   */
  if (rtb_Saturation >= 0.025) {
    /* Outputs for IfAction SubSystem: '<S8>/If Action Subsystem' incorporates:
     *  ActionPort: '<S22>/Action Port'
     */
    Schubverteilu_IfActionSubsystem(rtb_Add6, &Schubverteilung0_B.Merge_l);

    /* End of Outputs for SubSystem: '<S8>/If Action Subsystem' */
  } else {
    /* Outputs for IfAction SubSystem: '<S8>/If Action Subsystem1' incorporates:
     *  ActionPort: '<S23>/Action Port'
     */
    Schubverteilung0_B.Merge_l = 0.0;

    /* End of Outputs for SubSystem: '<S8>/If Action Subsystem1' */
  }

  /* End of If: '<S8>/If' */

  /* If: '<S4>/If' incorporates:
   *  Constant: '<S15>/Constant'
   */
  if (Schubverteilung0_B.Merge_l >= 0.0) {
    /* Outputs for IfAction SubSystem: '<S4>/If Action Subsystem' incorporates:
     *  ActionPort: '<S14>/Action Port'
     */
    Schubverteilu_IfActionSubsystem(Schubverteilung0_B.Merge_l,
      &Schubverteilung0_B.Merge_p);

    /* End of Outputs for SubSystem: '<S4>/If Action Subsystem' */
  } else {
    /* Outputs for IfAction SubSystem: '<S4>/If Action Subsystem1' incorporates:
     *  ActionPort: '<S15>/Action Port'
     */
    Schubverteilung0_B.Merge_p = 0.0;

    /* End of Outputs for SubSystem: '<S4>/If Action Subsystem1' */
  }

  /* End of If: '<S4>/If' */

  /* Outport: '<Root>/Out_F_C' */
  Schubverteilung0_Y.Out_F_C = Schubverteilung0_B.Merge_p;

  /* Sum: '<S1>/Add7' incorporates:
   *  Gain: '<S1>/Gain3'
   *  Inport: '<Root>/In_M_Gier'
   *  Inport: '<Root>/In_M_Nick'
   *  Inport: '<Root>/In_M_Roll'
   *  Sum: '<S1>/Add3'
   */
  rtb_Add7 = ((Schubverteilung0_U.In_M_Roll - Schubverteilung0_U.In_M_Nick) -
              Schubverteilung0_U.In_M_Gier) * 0.25 + rtb_Gain4;

  /* If: '<S9>/If' incorporates:
   *  Constant: '<S25>/Constant'
   */
  if (rtb_Saturation >= 0.025) {
    /* Outputs for IfAction SubSystem: '<S9>/If Action Subsystem' incorporates:
     *  ActionPort: '<S24>/Action Port'
     */
    Schubverteilu_IfActionSubsystem(rtb_Add7, &Schubverteilung0_B.Merge_gn);

    /* End of Outputs for SubSystem: '<S9>/If Action Subsystem' */
  } else {
    /* Outputs for IfAction SubSystem: '<S9>/If Action Subsystem1' incorporates:
     *  ActionPort: '<S25>/Action Port'
     */
    Schubverteilung0_B.Merge_gn = 0.0;

    /* End of Outputs for SubSystem: '<S9>/If Action Subsystem1' */
  }

  /* End of If: '<S9>/If' */

  /* If: '<S5>/If' incorporates:
   *  Constant: '<S17>/Constant'
   */
  if (Schubverteilung0_B.Merge_gn >= 0.0) {
    /* Outputs for IfAction SubSystem: '<S5>/If Action Subsystem' incorporates:
     *  ActionPort: '<S16>/Action Port'
     */
    Schubverteilu_IfActionSubsystem(Schubverteilung0_B.Merge_gn,
      &Schubverteilung0_B.Merge_pb);

    /* End of Outputs for SubSystem: '<S5>/If Action Subsystem' */
  } else {
    /* Outputs for IfAction SubSystem: '<S5>/If Action Subsystem1' incorporates:
     *  ActionPort: '<S17>/Action Port'
     */
    Schubverteilung0_B.Merge_pb = 0.0;

    /* End of Outputs for SubSystem: '<S5>/If Action Subsystem1' */
  }

  /* End of If: '<S5>/If' */

  /* Outport: '<Root>/Out_F_D' */
  Schubverteilung0_Y.Out_F_D = Schubverteilung0_B.Merge_pb;
}

/* Model initialize function */
void Schubverteilung0_initialize(void)
{
  /* Registration code */

  /* initialize real-time model */
  (void) memset((void *)Schubverteilung0_M, 0,
                sizeof(RT_MODEL_Schubverteilung0));
  rtmSetFirstInitCond(Schubverteilung0_M, 1);

  /* block I/O */
  (void) memset(((void *) &Schubverteilung0_B), 0,
                sizeof(BlockIO_Schubverteilung0));

  /* external inputs */
  (void) memset((void *)&Schubverteilung0_U, 0,
                sizeof(ExternalInputs_Schubverteilung0));

  /* external outputs */
  (void) memset((void *)&Schubverteilung0_Y, 0,
                sizeof(ExternalOutputs_Schubverteilung));

  /* InitializeConditions for Merge: '<S6>/Merge' */
  if (rtmIsFirstInitCond(Schubverteilung0_M)) {
    Schubverteilung0_B.Merge = 0.0;
  }

  /* End of InitializeConditions for Merge: '<S6>/Merge' */

  /* InitializeConditions for Merge: '<S2>/Merge' */
  if (rtmIsFirstInitCond(Schubverteilung0_M)) {
    Schubverteilung0_B.Merge_m = 0.0;
  }

  /* End of InitializeConditions for Merge: '<S2>/Merge' */

  /* InitializeConditions for Merge: '<S7>/Merge' */
  if (rtmIsFirstInitCond(Schubverteilung0_M)) {
    Schubverteilung0_B.Merge_d = 0.0;
  }

  /* End of InitializeConditions for Merge: '<S7>/Merge' */

  /* InitializeConditions for Merge: '<S3>/Merge' */
  if (rtmIsFirstInitCond(Schubverteilung0_M)) {
    Schubverteilung0_B.Merge_g = 0.0;
  }

  /* End of InitializeConditions for Merge: '<S3>/Merge' */

  /* InitializeConditions for Merge: '<S8>/Merge' */
  if (rtmIsFirstInitCond(Schubverteilung0_M)) {
    Schubverteilung0_B.Merge_l = 0.0;
  }

  /* End of InitializeConditions for Merge: '<S8>/Merge' */

  /* InitializeConditions for Merge: '<S4>/Merge' */
  if (rtmIsFirstInitCond(Schubverteilung0_M)) {
    Schubverteilung0_B.Merge_p = 0.0;
  }

  /* End of InitializeConditions for Merge: '<S4>/Merge' */

  /* InitializeConditions for Merge: '<S9>/Merge' */
  if (rtmIsFirstInitCond(Schubverteilung0_M)) {
    Schubverteilung0_B.Merge_gn = 0.0;
  }

  /* End of InitializeConditions for Merge: '<S9>/Merge' */

  /* InitializeConditions for Merge: '<S5>/Merge' */
  if (rtmIsFirstInitCond(Schubverteilung0_M)) {
    Schubverteilung0_B.Merge_pb = 0.0;
  }

  /* End of InitializeConditions for Merge: '<S5>/Merge' */

  /* set "at time zero" to false */
  if (rtmIsFirstInitCond(Schubverteilung0_M)) {
    rtmSetFirstInitCond(Schubverteilung0_M, 0);
  }
}

/* Model terminate function */
void Schubverteilung0_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
