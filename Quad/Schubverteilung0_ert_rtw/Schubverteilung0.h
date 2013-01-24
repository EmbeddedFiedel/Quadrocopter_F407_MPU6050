/*
 * File: Schubverteilung0.h
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

#ifndef RTW_HEADER_Schubverteilung0_h_
#define RTW_HEADER_Schubverteilung0_h_
#ifndef Schubverteilung0_COMMON_INCLUDES_
# define Schubverteilung0_COMMON_INCLUDES_
#include <string.h>
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#endif                                 /* Schubverteilung0_COMMON_INCLUDES_ */

#include "Schubverteilung0_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

/* Block signals (auto storage) */
typedef struct {
  real_T Merge;                        /* '<S6>/Merge' */
  real_T Merge_m;                      /* '<S2>/Merge' */
  real_T Merge_d;                      /* '<S7>/Merge' */
  real_T Merge_g;                      /* '<S3>/Merge' */
  real_T Merge_l;                      /* '<S8>/Merge' */
  real_T Merge_p;                      /* '<S4>/Merge' */
  real_T Merge_gn;                     /* '<S9>/Merge' */
  real_T Merge_pb;                     /* '<S5>/Merge' */
} BlockIO_Schubverteilung0;

/* External inputs (root inport signals with auto storage) */
typedef struct {
  real_T In_M_Roll;                    /* '<Root>/In_M_Roll' */
  real_T In_M_Nick;                    /* '<Root>/In_M_Nick' */
  real_T In_M_Gier;                    /* '<Root>/In_M_Gier' */
  real_T In_Throttle;                  /* '<Root>/In_Throttle' */
} ExternalInputs_Schubverteilung0;

/* External outputs (root outports fed by signals with auto storage) */
typedef struct {
  real_T Out_F_A;                      /* '<Root>/Out_F_A' */
  real_T Out_F_B;                      /* '<Root>/Out_F_B' */
  real_T Out_F_C;                      /* '<Root>/Out_F_C' */
  real_T Out_F_D;                      /* '<Root>/Out_F_D' */
} ExternalOutputs_Schubverteilung;

/* Real-time Model Data Structure */
struct RT_MODEL_Schubverteilung0 {
  const char_T * volatile errorStatus;

  /*
   * Timing:
   * The following substructure contains information regarding
   * the timing information for the model.
   */
  struct {
    boolean_T firstInitCondFlag;
  } Timing;
};

/* Block signals (auto storage) */
extern BlockIO_Schubverteilung0 Schubverteilung0_B;

/* External inputs (root inport signals with auto storage) */
#ifdef __cplusplus

extern "C" {

#endif

  extern ExternalInputs_Schubverteilung0 Schubverteilung0_U;

#ifdef __cplusplus

}
#endif

/* External outputs (root outports fed by signals with auto storage) */
#ifdef __cplusplus

extern "C" {

#endif

  extern ExternalOutputs_Schubverteilung Schubverteilung0_Y;

#ifdef __cplusplus

}
#endif

#ifdef __cplusplus

extern "C" {

#endif

  /* Model entry point functions */
  extern void Schubverteilung0_initialize(void);
  extern void Schubverteilung0_step(void);
  extern void Schubverteilung0_terminate(void);

#ifdef __cplusplus

}
#endif

/* Real-time Model object */
#ifdef __cplusplus

extern "C" {

#endif

  extern struct RT_MODEL_Schubverteilung0 *const Schubverteilung0_M;

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
 * hilite_system('regler/Regelkreis/Schubverteilung')    - opens subsystem regler/Regelkreis/Schubverteilung
 * hilite_system('regler/Regelkreis/Schubverteilung/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'regler/Regelkreis'
 * '<S1>'   : 'regler/Regelkreis/Schubverteilung'
 * '<S2>'   : 'regler/Regelkreis/Schubverteilung/Begrenzung groeﬂer Null'
 * '<S3>'   : 'regler/Regelkreis/Schubverteilung/Begrenzung groeﬂer Null1'
 * '<S4>'   : 'regler/Regelkreis/Schubverteilung/Begrenzung groeﬂer Null2'
 * '<S5>'   : 'regler/Regelkreis/Schubverteilung/Begrenzung groeﬂer Null3'
 * '<S6>'   : 'regler/Regelkreis/Schubverteilung/Throttlecutoff'
 * '<S7>'   : 'regler/Regelkreis/Schubverteilung/Throttlecutoff1'
 * '<S8>'   : 'regler/Regelkreis/Schubverteilung/Throttlecutoff2'
 * '<S9>'   : 'regler/Regelkreis/Schubverteilung/Throttlecutoff3'
 * '<S10>'  : 'regler/Regelkreis/Schubverteilung/Begrenzung groeﬂer Null/If Action Subsystem'
 * '<S11>'  : 'regler/Regelkreis/Schubverteilung/Begrenzung groeﬂer Null/If Action Subsystem1'
 * '<S12>'  : 'regler/Regelkreis/Schubverteilung/Begrenzung groeﬂer Null1/If Action Subsystem'
 * '<S13>'  : 'regler/Regelkreis/Schubverteilung/Begrenzung groeﬂer Null1/If Action Subsystem1'
 * '<S14>'  : 'regler/Regelkreis/Schubverteilung/Begrenzung groeﬂer Null2/If Action Subsystem'
 * '<S15>'  : 'regler/Regelkreis/Schubverteilung/Begrenzung groeﬂer Null2/If Action Subsystem1'
 * '<S16>'  : 'regler/Regelkreis/Schubverteilung/Begrenzung groeﬂer Null3/If Action Subsystem'
 * '<S17>'  : 'regler/Regelkreis/Schubverteilung/Begrenzung groeﬂer Null3/If Action Subsystem1'
 * '<S18>'  : 'regler/Regelkreis/Schubverteilung/Throttlecutoff/If Action Subsystem'
 * '<S19>'  : 'regler/Regelkreis/Schubverteilung/Throttlecutoff/If Action Subsystem1'
 * '<S20>'  : 'regler/Regelkreis/Schubverteilung/Throttlecutoff1/If Action Subsystem'
 * '<S21>'  : 'regler/Regelkreis/Schubverteilung/Throttlecutoff1/If Action Subsystem1'
 * '<S22>'  : 'regler/Regelkreis/Schubverteilung/Throttlecutoff2/If Action Subsystem'
 * '<S23>'  : 'regler/Regelkreis/Schubverteilung/Throttlecutoff2/If Action Subsystem1'
 * '<S24>'  : 'regler/Regelkreis/Schubverteilung/Throttlecutoff3/If Action Subsystem'
 * '<S25>'  : 'regler/Regelkreis/Schubverteilung/Throttlecutoff3/If Action Subsystem1'
 */
#endif                                 /* RTW_HEADER_Schubverteilung0_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
