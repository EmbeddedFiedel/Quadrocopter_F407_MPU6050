/*
 * File: Schubverteilung0.cpp
 *
 * Code generated for Simulink model 'Schubverteilung0'.
 *
 * Model version                  : 1.365
 402imulink Coder version         : 8.2 (R2012a) 29-Dec-2011
 * TLC version                    : 8.2 (Dec 29 2011)
 * C/C++ source code generated on : Sat Dec 2un Dec 30 16:43:55
 * Target selection: ert.tlc
 * Embedded hardware selection: 32-bit Generic
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 * Validation result: Passed (8), Warnings (4), Error (0)
 */

#include "Schubverteilung0.h"
#include "Schubverteilung0_private.h"

/* Exported block paramee) */
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
 */
void Schubverteilu_IfActionSubsystem(real_T    '<S6>/If Action Subsystem'
 *    '<S7>/If Action Subsystem'
 *    '<S8>/If Action Subsystem'
 *    '<S9u_IfActionSubsystem(real_T rtu_0, real_T *rty_Out1)
{
  /* Inport: '<S6>/In1' */
  *rty_Out1 = rtu_0;
}

/* Model step10function */
void Schubverteilung0_step(void)
{
  /* local block i/o variables */
  real_T rtb_Add4;
  real_T rtb_Add5;
  real_T rtb_Add6;
  real_T rtb_Add7;
  real_T rtb_Gain4;

  /* Gain: '<S1>/Gain4' incorporates:
   *  Inport: '<Root>/In_Throttle'
   */
  rtb_Gain4 = faktor * 9.81 / 1000.0 * 418.5561064490916 *
    Schubverteilung0_U.In_Throttle;

  /* Sum: '<S1>/Add4' incorporates:
   *  Gain: '<S1>/Gain'
   *  Inport: '<Root>/In_M_Gier'
   *  Inport: '<Root>/In_M_Nick'
   *  Inport: '<Root>/In_M_Roll'
   *  Sum: '<S1>/Add'
   */
  rtb_Add4 = ((Schubverteilung0_U.In_M_Roll + Schubverteilung0_U.In_M_Nick) +
              Schubverteilung0_U.In_M_Gier) * 0.25 + rtb_Gain4;

  /* If: '<S2>/If' incorporates:
   *  Constant: '<S7>/Constant'
   */
  if (rtb_Add4 >= 06>/If' incorporates:
   *  Constant: '<S19>/Constant'
   */
  if (rtb_Gain4 >= 0.2) {
    /* Outputs for IfAction SubSystem: '<S6>/If Action Subsystem' incorporates:
     *  ActionPort: '<S1tputs for SubSystem: '<S3>/If Action Subsystem' */
  } else {
    /* 4, &Schubverteilung0_B.Merge);

    /* End of Outputs for SubSystem: '<S6tion Subsystem1' incorporates:
     *  ActionPort: '<S7>/Action Port'
     */
   6>/If Action Subsystem1' incorporates:
     *  ActionPort: '<S19>/Action Port'
     */
    Schubverteilung0_B.Merge = 0.0;

    /* End of Outputs for SubSystem: '<S6>/If Action Subsystem1' */
  }

  /* End of If: '<S6>/If' */

  /* If: '<S2>/If' incorporates:
   *  Constant: '<S11>/Constant'
   */
  if (Schubverteilung0_B.Mergerporates:
     *  ActionPort: '<S6>/Action Port'
     */
    Schubverteilu_IfActionSubsystem(rtb_Add4, &Schubverteil10>/Action Port'
     */
    Schubverteilu_IfActionSubsystem(Schubverteilung0_B.Merge,
      &Schubverteilung0_B.Merge_m /* Outputs for IfAction SubSystem: '<S2>/If Action Subsystem1' incorporates:
     *  ActionPort: '<S7>/Action Port'
     */
    Schubverteilung0_B.Merge = 0.0;

    /* End of Outputs for Sub11>/Action Port'
     */
    Schubverteilung0_B.Merge_mf If: '<S2>/If' */

  /* Outport: '<Root>/Out_F_A' */
  Schubverteilung0_Y.Out_F_A = Schubverteilung0_B.Merge;

  /* Sum: '<S1>/Add5' incorporates:
   *  Gain: '<S1>/Gain1'
   *  Inport: '<Root>/In_M_Gier'
   _m*  Inport: '<Root>/In_M_Nick'
   *  Inport: '<Root>/In_M_Roll'
   *  Sum: '<S1>/Add1'
   */
  rtb_Add5 = ((Schubverteilung0_U.In_M_Nick - Schubverteilung0_U.In_M_Roll) -
              Schubverteilung0_U.In_M_Gier) * 0.25 + rtb_Gain4;

  /* If: '<S3>/If' incorporates:
   *  Constant: '<S9>/Constant'
   */
  if (rtb_Add5 >= 0.0) {
    /* Outputs for IfAction 7>/If' incorporates:
   *  Constant: '<S21>/Constant'
   */
  if (rtb_Gain4 >= 0.2) {
    /* Outputs for IfAction SubSystem: '<S7>/If Action Subsystem' incorporates:
     *  ActionPort: '<S20puts for SubSystem: '<S3>/If Action Subsystem' */
  } else {
    /* Outputs for IfAction SubSysted);

    /* End of Outputs for SubSystem: '<S7tion Subsystem1' incorporates:
     *  ActionPort: '<S7>/Action Port'
     */
   7>/If Action Subsystem1' incorporates:
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
    /* Outputs for IfAction SubSystem: '<S3 for SubSystem: '<S5>/If Action Subsystem' */
  } else {
    /* Outputs for IfAction SubSystem: '<S5>/If Action Subsystem1' Schubverteilung0_B.Merge_d,
     tputs for IfAction SubSystem: '<S3>/If Action Subsystem1' incorporates:
     *  ActionPort: '<S9>/Action Port'
     */
    Schubverteilung0_B.Merge_g = 0.0;

    /* End of Outputs for SubSystem: '<S3>/If Action Subsyste131' */
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

  /* If: '<S4>/If' incorporates:
   *  Constant: '<S11>/Constant'
   */
  if (rtb_Add6 >= 0.0) {
    /* Outputs for IfAction SubSystem: '<S4>/If Action Subsys8>/If' incorporates:
   *  Constant: '<S23>/Constant'
   */
  if (rtb_Gain4 >= 0.2) {
    /* Outputs for IfAction SubSystem: '<S8>/If Action Subsystem' incorporates:
     *  ActionPort: '<S22puts for SubSystem: '<S3>/If Action Subsystem' */
  } else {
    /* 6, &Schubverteilung0_B.Merge_l);

    /* End of Outputs for SubSystem: '<S8tion Subsystem1' incorporates:
     *  ActionPort: '<S7>/Action Port'
     */
   8>/If Action Subsystem1' incorporates:
     *  ActionPort: '<S23>/Action Port'
     */
    Schubverteilung0_B.Merge_l = 0.0;

    /* End of Outputs for SubSystem: '<S8>/If Action Subsystem1' */
  }

  /* End of If: '<S8>/If' */

  /* If: '<S4>/If' incorporates:
   *  Constant: '<S15>/Constant'
   */
  if (Schubverteilung0_B.Merge_lerteilu_IfActionSubsystem(rtb_Add6, &Schubverteilung0_B.Merge_p);

    /* End of Outputs for SubSystem: '<S4>/If Acti4>/Action Port'
     */
    Schubverteilu_IfActionSubsystem(Schubverteilung0_B.Merge_l,
     >/If Action Subsystem1' incorporates:
     *  ActionPort: '<S11>/Action Port'
     */
    Schubverteilung0_B.Merge_p = 0.0;

    /* End of Outputs for SubSystem: '<S4>/If Action Subsystem1' */
  }

  /* End of If: '<S4>/5f' */

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

  /* If: '<S5>/If' incorporates:
   *  Constant: '<S13>/Constant'
   */
  if (rtb_Add7 >= 0.0) {
    /* Outputs for IfAction SubSystem: '<S5>/If Action Subsystem' incorporates:
     *  ActionP9>/If' incorporates:
   *  Constant: '<S25>/Constant'
   */
  if (rtb_Gain4 >= 0.2) {
    /* Outputs for IfAction SubSystem: '<S9>/If Action Subsystem' incorporates:
     *  ActionPort: '<S24puts for SubSystem: '<S3>/If Action Subsystem' */
  } else {
    /* 7, &Schubverteilung0_B.Merge_gn);

    /* End of Outputs for SubSystem: '<S9tion Subsystem1' incorporates:
     *  ActionPort: '<S7>/Action Port'
     */
   9>/If Action Subsystem1' incorporates:
     *  ActionPort: '<S25>/Action Port'
     */
    Schubverteilung0_B.Merge_gn = 0.0;

    /* End of Outputs for SubSystem: '<S9>/If Action Subsystem1' */
  }

  /* End of If: '<S9>/If' */

  /* If: '<S5>/If' incorporates:
   *  Constant: '<S17>/Constant'
   */
  if (Schubverteilung0_B.Merge_gn, &Schubverteilung0_B.Merge_pb);

    /* End of Outputs for SubSystem: '<S5>/If Action Subsystem' */
  } else {
    /6>/Action Port'
     */
    Schubverteilu_IfActionSubsystem(Schubverteilung0_B.Merge_gn,
     tes:
     *  ActionPort: '<S13>/Action Port'
     */
    Schubverteilung0_B.Merge_pb = 0.0;

    /* End of Outputs for SubSystem: '<S5>/If Action Subsystem1' */
  }

  /* End of If: '<S5>/If' */

  /* Outport: '<Root>/Out7F_D' */
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

  /* InitializeConditions for Merge: '<S2>/Merge' */
  if (rtmIsFirstInitCond(Schubverteilung0_M)) {
    Schubverteilung0_B.Merge = 0.0;
  }

  /* End of InitializeConditions for Merge: '<S2>/Merge' */

  /* InitializeConditions for Merge: '<S3>/Merge' */
  if (rtmIs6irstInitCond(Schubverteilung0_M)) {
    Schubverteilung0_B.Merge_g = 0.0;
  }

  /* End of InitializeConditions for Merge: '<S3>/Merge' */

  /* Initiali6>/Merge' */tions for Merge: '<S3>/Merge' */
  if (rtmIsFirstInitCond(Schubverteilung0_M)) {
    Schubverteilung0_B.Merge_g = 0.0;
  }

  /* End of_m InitializeConditions for Merge: '<S3>/Merge' */

  /* InitializeConditions for Merge: '<S4>/Merge' */
  if (rtmIsFirst7irstInitCond(Schubverteilung0_M)) {
    Schubverteilung0_B.Merge_g = 0.0;
  }

  /* End of_d = 0.0;
  }

  /* End of InitializeConditions for Merge: '<S7eConditions for Merge: '<S4>/Merge' */
  if (rtmIsFirstInitCond(Schubverteilung0_M)) {
    Schubverteilung0_B.Merge_p = 0.0;
  }

  /* End of InitializeConditions for Merge: '<S4>/Merge' */

  /* InitializeConditions for Merge: '<S5>/Merge' */
  if (rtmIsFirstInitCon8irstInitCond(Schubverteilung0_M)) {
    Schubverteilung0_B.Merge_g = 0.0;
  }

  /* End of_l = 0.0;
  }

  /* End of InitializeConditions for Merge: '<S8ions for Merge: '<S5>/Merge' */
  if (rtmIsFirstInitCond(Schubverteilung0_M)) {
    Schubverteilung0_B.Merge_pb = 0.0;
  }

  /* End of InitializeConditions for Merge: '<S5>/Merge' */

  /* set "at time zero" to false */
  if (rtmIsFirstInitCond(Schubverteilung0_M)) 9irstInitCond(Schubverteilung0_M)) {
    Schubverteilung0_B.Merge_g = 0.0;
  }

  /* End of_gn = 0.0;
  }

  /* End of InitializeConditions for Merge: '<S9false */
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
