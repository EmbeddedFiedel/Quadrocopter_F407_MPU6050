/*
 * File: Inverse_Propeller.cpp
 *
 * Code generated for Simulink model 'Inverse_Propeller'.
 *
 * Model version                  : 1.365
 * Simulink Coder version         : 8.2 (R2012a) 29-Dec-2011
 * TLC version                    : 8.2 (Dec 29 2011)
 * C/C++ source code generated on : Sat Dec 29 17:42:24 2012
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: 32-bit Generic
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 * Validation result: Passed (8), Warnings (4), Error (0)
 */

#include "Inverse_Propeller.h"
#include "Inverse_Propeller_private.h"

/* Exported block parameters */
real_T exponent = 2.015137;            /* Variable: exponent
                                        * Referenced by:
                                        *   '<S2>/Constant3'
                                        *   '<S3>/Constant3'
                                        *   '<S4>/Constant3'
                                        *   '<S5>/Constant3'
                                        */
real_T faktor = 0.8895305;             /* Variable: faktor
                                        * Referenced by:
                                        *   '<S2>/Constant2'
                                        *   '<S3>/Constant2'
                                        *   '<S4>/Constant2'
                                        *   '<S5>/Constant2'
                                        */
real_T n_max = 20000.0;                /* Variable: n_max
                                        * Referenced by:
                                        *   '<S1>/Gain'
                                        *   '<S1>/Gain1'
                                        *   '<S1>/Gain2'
                                        *   '<S1>/Gain3'
                                        *   '<S1>/Saturation'
                                        *   '<S1>/Saturation1'
                                        *   '<S1>/Saturation2'
                                        *   '<S1>/Saturation3'
                                        */

/* External inputs (root inport signals with auto storage) */
ExternalInputs_Inverse_Propelle Inverse_Propeller_U;

/* External outputs (root outports fed by signals with auto storage) */
ExternalOutputs_Inverse_Propell Inverse_Propeller_Y;

/* Real-time model */
RT_MODEL_Inverse_Propeller Inverse_Propeller_M_;
RT_MODEL_Inverse_Propeller *const Inverse_Propeller_M = &Inverse_Propeller_M_;
real_T rt_powd_snf(real_T u0, real_T u1)
{
  real_T y;
  real_T tmp;
  real_T tmp_0;
  if (rtIsNaN(u0) || rtIsNaN(u1)) {
    y = (rtNaN);
  } else {
    tmp = fabs(u0);
    tmp_0 = fabs(u1);
    if (rtIsInf(u1)) {
      if (tmp == 1.0) {
        y = (rtNaN);
      } else if (tmp > 1.0) {
        if (u1 > 0.0) {
          y = (rtInf);
        } else {
          y = 0.0;
        }
      } else if (u1 > 0.0) {
        y = 0.0;
      } else {
        y = (rtInf);
      }
    } else if (tmp_0 == 0.0) {
      y = 1.0;
    } else if (tmp_0 == 1.0) {
      if (u1 > 0.0) {
        y = u0;
      } else {
        y = 1.0 / u0;
      }
    } else if (u1 == 2.0) {
      y = u0 * u0;
    } else if ((u1 == 0.5) && (u0 >= 0.0)) {
      y = sqrt(u0);
    } else if ((u0 < 0.0) && (u1 > floor(u1))) {
      y = (rtNaN);
    } else {
      y = pow(u0, u1);
    }
  }

  return y;
}

/* Model step function */
void Inverse_Propeller_step(void)
{
  real_T rtb_Gain_d;

  /* Product: '<S2>/Divide1' incorporates:
   *  Constant: '<S2>/Constant2'
   *  Inport: '<Root>/In_F_A'
   */
  rtb_Gain_d = Inverse_Propeller_U.In_F_A / (faktor * 9.81 / 1000.0);

  /* Math: '<S2>/Math Function1' incorporates:
   *  Constant: '<S2>/Constant3'
   */
  if ((rtb_Gain_d < 0.0) && (1.0 / exponent > floor(1.0 / exponent))) {
    rtb_Gain_d = -rt_powd_snf(-rtb_Gain_d, 1.0 / exponent);
  } else {
    rtb_Gain_d = rt_powd_snf(rtb_Gain_d, 1.0 / exponent);
  }

  /* End of Math: '<S2>/Math Function1' */

  /* Gain: '<S2>/Gain' */
  rtb_Gain_d *= 1000.0;

  /* Saturate: '<S1>/Saturation' */
  if (rtb_Gain_d >= n_max) {
    rtb_Gain_d = n_max;
  } else {
    if (rtb_Gain_d <= 0.0) {
      rtb_Gain_d = 0.0;
    }
  }

  /* Outport: '<Root>/Out_n_A' incorporates:
   *  Gain: '<S1>/Gain'
   *  Saturate: '<S1>/Saturation'
   */
  Inverse_Propeller_Y.Out_n_A = 1.0 / n_max * rtb_Gain_d;

  /* Product: '<S3>/Divide1' incorporates:
   *  Constant: '<S3>/Constant2'
   *  Inport: '<Root>/In_F_B'
   */
  rtb_Gain_d = Inverse_Propeller_U.In_F_B / (faktor * 9.81 / 1000.0);

  /* Math: '<S3>/Math Function1' incorporates:
   *  Constant: '<S3>/Constant3'
   */
  if ((rtb_Gain_d < 0.0) && (1.0 / exponent > floor(1.0 / exponent))) {
    rtb_Gain_d = -rt_powd_snf(-rtb_Gain_d, 1.0 / exponent);
  } else {
    rtb_Gain_d = rt_powd_snf(rtb_Gain_d, 1.0 / exponent);
  }

  /* End of Math: '<S3>/Math Function1' */

  /* Gain: '<S3>/Gain' */
  rtb_Gain_d *= 1000.0;

  /* Saturate: '<S1>/Saturation1' */
  if (rtb_Gain_d >= n_max) {
    rtb_Gain_d = n_max;
  } else {
    if (rtb_Gain_d <= 0.0) {
      rtb_Gain_d = 0.0;
    }
  }

  /* Outport: '<Root>/Out_n_B' incorporates:
   *  Gain: '<S1>/Gain1'
   *  Saturate: '<S1>/Saturation1'
   */
  Inverse_Propeller_Y.Out_n_B = 1.0 / n_max * rtb_Gain_d;

  /* Product: '<S4>/Divide1' incorporates:
   *  Constant: '<S4>/Constant2'
   *  Inport: '<Root>/In_F_C'
   */
  rtb_Gain_d = Inverse_Propeller_U.In_F_C / (faktor * 9.81 / 1000.0);

  /* Math: '<S4>/Math Function1' incorporates:
   *  Constant: '<S4>/Constant3'
   */
  if ((rtb_Gain_d < 0.0) && (1.0 / exponent > floor(1.0 / exponent))) {
    rtb_Gain_d = -rt_powd_snf(-rtb_Gain_d, 1.0 / exponent);
  } else {
    rtb_Gain_d = rt_powd_snf(rtb_Gain_d, 1.0 / exponent);
  }

  /* End of Math: '<S4>/Math Function1' */

  /* Gain: '<S4>/Gain' */
  rtb_Gain_d *= 1000.0;

  /* Saturate: '<S1>/Saturation2' */
  if (rtb_Gain_d >= n_max) {
    rtb_Gain_d = n_max;
  } else {
    if (rtb_Gain_d <= 0.0) {
      rtb_Gain_d = 0.0;
    }
  }

  /* Outport: '<Root>/Out_n_C' incorporates:
   *  Gain: '<S1>/Gain2'
   *  Saturate: '<S1>/Saturation2'
   */
  Inverse_Propeller_Y.Out_n_C = 1.0 / n_max * rtb_Gain_d;

  /* Product: '<S5>/Divide1' incorporates:
   *  Constant: '<S5>/Constant2'
   *  Inport: '<Root>/In_F_D'
   */
  rtb_Gain_d = Inverse_Propeller_U.In_F_D / (faktor * 9.81 / 1000.0);

  /* Math: '<S5>/Math Function1' incorporates:
   *  Constant: '<S5>/Constant3'
   */
  if ((rtb_Gain_d < 0.0) && (1.0 / exponent > floor(1.0 / exponent))) {
    rtb_Gain_d = -rt_powd_snf(-rtb_Gain_d, 1.0 / exponent);
  } else {
    rtb_Gain_d = rt_powd_snf(rtb_Gain_d, 1.0 / exponent);
  }

  /* End of Math: '<S5>/Math Function1' */

  /* Gain: '<S5>/Gain' */
  rtb_Gain_d *= 1000.0;

  /* Saturate: '<S1>/Saturation3' */
  if (rtb_Gain_d >= n_max) {
    rtb_Gain_d = n_max;
  } else {
    if (rtb_Gain_d <= 0.0) {
      rtb_Gain_d = 0.0;
    }
  }

  /* Outport: '<Root>/Out_n_D' incorporates:
   *  Gain: '<S1>/Gain3'
   *  Saturate: '<S1>/Saturation3'
   */
  Inverse_Propeller_Y.Out_n_D = 1.0 / n_max * rtb_Gain_d;
}

/* Model initialize function */
void Inverse_Propeller_initialize(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

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
