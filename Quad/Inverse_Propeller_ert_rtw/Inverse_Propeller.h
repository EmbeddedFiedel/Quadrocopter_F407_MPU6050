/*
 * File: Inverse_Propeller.h
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

#ifndef RTW_HEADER_Inverse_Propeller_h_
#define RTW_HEADER_Inverse_Propeller_h_
#ifndef Inverse_Propeller_COMMON_INCLUDES_
# define Inverse_Propeller_COMMON_INCLUDES_
#include <math.h>
#include <stddef.h>
#include <string.h>
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#include "rt_nonfinite.h"
#include "rtGetInf.h"
#include "rtGetNaN.h"
#endif                                 /* Inverse_Propeller_COMMON_INCLUDES_ */

#include "Inverse_Propeller_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

/* External inputs (root inport signals with auto storage) */
typedef struct {
  real_T In_F_A;                       /* '<Root>/In_F_A' */
  real_T In_F_B;                       /* '<Root>/In_F_B' */
  real_T In_F_C;                       /* '<Root>/In_F_C' */
  real_T In_F_D;                       /* '<Root>/In_F_D' */
} ExternalInputs_Inverse_Propelle;

/* External outputs (root outports fed by signals with auto storage) */
typedef struct {
  real_T Out_n_A;                      /* '<Root>/Out_n_A' */
  real_T Out_n_B;                      /* '<Root>/Out_n_B' */
  real_T Out_n_C;                      /* '<Root>/Out_n_C' */
  real_T Out_n_D;                      /* '<Root>/Out_n_D' */
} ExternalOutputs_Inverse_Propell;

/* Real-time Model Data Structure */
struct RT_MODEL_Inverse_Propeller {
  const char_T * volatile errorStatus;
};

/* External inputs (root inport signals with auto storage) */
#ifdef __cplusplus

extern "C" {

#endif

  extern ExternalInputs_Inverse_Propelle Inverse_Propeller_U;

#ifdef __cplusplus

}
#endif

/* External outputs (root outports fed by signals with auto storage) */
#ifdef __cplusplus

extern "C" {

#endif

  extern ExternalOutputs_Inverse_Propell Inverse_Propeller_Y;

#ifdef __cplusplus

}
#endif

/*
 * Exported Global Parameters
 *
 * Note: Exported global parameters are tunable parameters with an exported
 * global storage class designation.  Code generation will declare the memory for
 * these parameters and exports their symbols.
 *
 */
extern real_T exponent;                /* Variable: exponent
                                        * Referenced by:
                                        *   '<S2>/Constant3'
                                        *   '<S3>/Constant3'
                                        *   '<S4>/Constant3'
                                        *   '<S5>/Constant3'
                                        */
extern real_T faktor;                  /* Variable: faktor
                                        * Referenced by:
                                        *   '<S2>/Constant2'
                                        *   '<S3>/Constant2'
                                        *   '<S4>/Constant2'
                                        *   '<S5>/Constant2'
                                        */
extern real_T n_max;                   /* Variable: n_max
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

#ifdef __cplusplus

extern "C" {

#endif

  /* Model entry point functions */
  extern void Inverse_Propeller_initialize(void);
  extern void Inverse_Propeller_step(void);
  extern void Inverse_Propeller_terminate(void);

#ifdef __cplusplus

}
#endif

/* Real-time Model object */
#ifdef __cplusplus

extern "C" {

#endif

  extern struct RT_MODEL_Inverse_Propeller *const Inverse_Propeller_M;

#ifdef __cplusplus

}
#endif

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Note that this particular code originates from a subsystem build,
 * and has its own system numbers different from the parent model.
 * Refer to the system hierarchy for this subsystem below, and use the
 * MATLAB hilite_system command to trace the generated code back
 * to the parent model.  For example,
 *
 * hilite_system('regler/Regelkreis/Inverse_Propeller')    - opens subsystem regler/Regelkreis/Inverse_Propeller
 * hilite_system('regler/Regelkreis/Inverse_Propeller/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'regler/Regelkreis'
 * '<S1>'   : 'regler/Regelkreis/Inverse_Propeller'
 * '<S2>'   : 'regler/Regelkreis/Inverse_Propeller/ inverse Schubkarft A'
 * '<S3>'   : 'regler/Regelkreis/Inverse_Propeller/ inverse Schubkarft B'
 * '<S4>'   : 'regler/Regelkreis/Inverse_Propeller/ inverse Schubkarft C'
 * '<S5>'   : 'regler/Regelkreis/Inverse_Propeller/ inverse Schubkarft D'
 */
#endif                                 /* RTW_HEADER_Inverse_Propeller_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
