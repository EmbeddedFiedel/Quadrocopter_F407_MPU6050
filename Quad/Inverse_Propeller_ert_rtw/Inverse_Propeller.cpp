/*
 * File: Inverse_Propeller.cpp
 *
 * Code generated for Simulink model 'Inverse_Propeller'.
 *
 * Model version                  : 1.548
 * Simulink Coder version         : 8.2 (R2012a) 29-Dec-2011
 * TLC version                    : 8.2 (Dec 29 2011)
 * C/C++ source code generated on : Sat Jun 08 13:49:15 2013
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 * Validation result: Passed (8), Warnings (4), Error (0)
 */

#include "Inverse_Propeller.h"
#include "Inverse_Propeller_private.h"

/* External inputs (root inport signals with auto storage) */
ExternalInputs_Inverse_Propelle Inverse_Propeller_U;

/* External outputs (root outports fed by signals with auto storage) */
ExternalOutputs_Inverse_Propell Inverse_Propeller_Y;

/* Real-time model */
RT_MODEL_Inverse_Propeller Inverse_Propeller_M_;
RT_MODEL_Inverse_Propeller *const Inverse_Propeller_M = &Inverse_Propeller_M_;

/* Model step function */
void Inverse_Propeller_step(void)
{
  real_T tmp;

  /* Saturate: '<S1>/Saturation' incorporates:
   *  Inport: '<Root>/In_F_A'
   */
  if (Inverse_Propeller_U.In_F_A >= n_max) {
    tmp = n_max;
  } else if (Inverse_Propeller_U.In_F_A <= 0.0) {
    tmp = 0.0;
  } else {
    tmp = Inverse_Propeller_U.In_F_A;
  }

  /* Outport: '<Root>/Out_n_A' incorporates:
   *  Gain: '<S1>/Gain'
   *  Saturate: '<S1>/Saturation'
   */
  Inverse_Propeller_Y.Out_n_A = 1.0 / n_max * tmp;

  /* Saturate: '<S1>/Saturation1' incorporates:
   *  Inport: '<Root>/In_F_B'
   */
  if (Inverse_Propeller_U.In_F_B >= n_max) {
    tmp = n_max;
  } else if (Inverse_Propeller_U.In_F_B <= 0.0) {
    tmp = 0.0;
  } else {
    tmp = Inverse_Propeller_U.In_F_B;
  }

  /* Outport: '<Root>/Out_n_B' incorporates:
   *  Gain: '<S1>/Gain1'
   *  Saturate: '<S1>/Saturation1'
   */
  Inverse_Propeller_Y.Out_n_B = 1.0 / n_max * tmp;

  /* Saturate: '<S1>/Saturation2' incorporates:
   *  Inport: '<Root>/In_F_C'
   */
  if (Inverse_Propeller_U.In_F_C >= n_max) {
    tmp = n_max;
  } else if (Inverse_Propeller_U.In_F_C <= 0.0) {
    tmp = 0.0;
  } else {
    tmp = Inverse_Propeller_U.In_F_C;
  }

  /* Outport: '<Root>/Out_n_C' incorporates:
   *  Gain: '<S1>/Gain2'
   *  Saturate: '<S1>/Saturation2'
   */
  Inverse_Propeller_Y.Out_n_C = 1.0 / n_max * tmp;

  /* Saturate: '<S1>/Saturation3' incorporates:
   *  Inport: '<Root>/In_F_D'
   */
  if (Inverse_Propeller_U.In_F_D >= n_max) {
    tmp = n_max;
  } else if (Inverse_Propeller_U.In_F_D <= 0.0) {
    tmp = 0.0;
  } else {
    tmp = Inverse_Propeller_U.In_F_D;
  }

  /* Outport: '<Root>/Out_n_D' incorporates:
   *  Gain: '<S1>/Gain3'
   *  Saturate: '<S1>/Saturation3'
   */
  Inverse_Propeller_Y.Out_n_D = 1.0 / n_max * tmp;
}

/* Model initialize function */
void Inverse_Propeller_initialize(void)
{
  /* Registration code */

  /* initialize error status */
  rtmSetErrorStatus(Inverse_Propeller_M, (NULL));

  /* external inputs */
  (void) memset((void *)&Inverse_Propeller_U, 0,
                sizeof(ExternalInputs_Inverse_Propelle));

  /* external outputs */
  (void) memset((void *)&Inverse_Propeller_Y, 0,
                sizeof(ExternalOutputs_Inverse_Propell));
}

/* Model terminate function */
void Inverse_Propeller_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
