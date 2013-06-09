/*
 * File: Regelglied.h
 *
 * Code generated for Simulink model 'Regelglied'.
 *
 * Model version                  : 1.553
 * Simulink Coder version         : 8.2 (R2012a) 29-Dec-2011
 * TLC version                    : 8.2 (Dec 29 2011)
 * C/C++ source code generated on : Sun Jun 09 11:48:47 2013
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 * Validation result: Passed (7), Warnings (5), Error (0)
 */

#ifndef RTW_HEADER_Regelglied_h_
#define RTW_HEADER_Regelglied_h_
#ifndef Regelglied_COMMON_INCLUDES_
# define Regelglied_COMMON_INCLUDES_
#include <string.h>
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#include "rt_zcfcn.h"
#endif                                 /* Regelglied_COMMON_INCLUDES_ */

#include "Regelglied_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

#ifndef rtmGetStopRequested
# define rtmGetStopRequested(rtm)      ((rtm)->Timing.stopRequestedFlag)
#endif

#ifndef rtmSetStopRequested
# define rtmSetStopRequested(rtm, val) ((rtm)->Timing.stopRequestedFlag = (val))
#endif

#ifndef rtmGetStopRequestedPtr
# define rtmGetStopRequestedPtr(rtm)   (&((rtm)->Timing.stopRequestedFlag))
#endif

#ifndef rtmGetT
# define rtmGetT(rtm)                  (rtmGetTPtr((rtm))[0])
#endif

/* Block signals (auto storage) */
typedef struct {
  real_T Merge;                        /* '<S2>/Merge' */
  real_T Gain1;                        /* '<S1>/Gain1' */
  real_T Gain4;                        /* '<S1>/Gain4' */
} BlockIO_Regelglied;

/* Block states (auto storage) for system '<Root>' */
typedef struct {
  int8_T If_ActiveSubsystem;           /* '<S2>/If' */
} D_Work_Regelglied;

/* Continuous states (auto storage) */
typedef struct {
  real_T Integrator_CSTATE;            /* '<S1>/Integrator' */
  real_T Integrator1_CSTATE;           /* '<S1>/Integrator1' */
} ContinuousStates_Regelglied;

/* State derivatives (auto storage) */
typedef struct {
  real_T Integrator_CSTATE;            /* '<S1>/Integrator' */
  real_T Integrator1_CSTATE;           /* '<S1>/Integrator1' */
} StateDerivatives_Regelglied;

/* State disabled  */
typedef struct {
  boolean_T Integrator_CSTATE;         /* '<S1>/Integrator' */
  boolean_T Integrator1_CSTATE;        /* '<S1>/Integrator1' */
} StateDisabled_Regelglied;

/* Zero-crossing (trigger) state */
typedef struct {
  ZCSigState Integrator_Reset_ZCE;     /* '<S1>/Integrator' */
  ZCSigState Integrator1_Reset_ZCE;    /* '<S1>/Integrator1' */
} PrevZCSigStates_Regelglied;

#ifndef ODE3_INTG
#define ODE3_INTG

/* ODE3 Integration Data */
typedef struct {
  real_T *y;                           /* output */
  real_T *f[3];                        /* derivatives */
} ODE3_IntgData;

#endif

/* External inputs (root inport signals with auto storage) */
typedef struct {
  real_T In_Soll_Roll;                 /* '<Root>/In_Soll_Roll' */
  real_T In_Soll_Nick;                 /* '<Root>/In_Soll_Nick' */
  real_T In_Soll_Gier;                 /* '<Root>/In_Soll_Gier' */
  real_T In_Ist_Roll;                  /* '<Root>/In_Ist_Roll' */
  real_T In_Ist_Nick;                  /* '<Root>/In_Ist_Nick' */
  real_T In_Ist_Gier;                  /* '<Root>/In_Ist_Gier' */
  real_T In_Ist_V_Roll;                /* '<Root>/In_Ist_V_Roll' */
  real_T In_Ist_V_Nick;                /* '<Root>/In_Ist_V_Nick' */
  real_T Throttle;                     /* '<Root>/Throttle' */
} ExternalInputs_Regelglied;

/* External outputs (root outports fed by signals with auto storage) */
typedef struct {
  real_T Out_M_Roll;                   /* '<Root>/Out_M_Roll' */
  real_T Out_M_Nick;                   /* '<Root>/Out_M_Nick' */
  real_T Out_M_Gier;                   /* '<Root>/Out_M_Gier' */
} ExternalOutputs_Regelglied;

/* Real-time Model Data Structure */
struct RT_MODEL_Regelglied {
  const char_T * volatile errorStatus;
  RTWSolverInfo solverInfo;

  /*
   * ModelData:
   * The following substructure contains information regarding
   * the data used in the model.
   */
  struct {
    real_T *contStates;
    real_T *derivs;
    boolean_T *contStateDisabled;
    boolean_T zCCacheNeedsReset;
    boolean_T derivCacheNeedsReset;
    boolean_T blkStateChange;
    real_T odeY[2];
    real_T odeF[3][2];
    ODE3_IntgData intgData;
  } ModelData;

  /*
   * Sizes:
   * The following substructure contains sizes information
   * for many of the model attributes such as inputs, outputs,
   * dwork, sample times, etc.
   */
  struct {
    int_T numContStates;
    int_T numSampTimes;
  } Sizes;

  /*
   * Timing:
   * The following substructure contains information regarding
   * the timing information for the model.
   */
  struct {
    uint32_T clockTick0;
    time_T stepSize0;
    uint32_T clockTick1;
    boolean_T firstInitCondFlag;
    SimTimeStep simTimeStep;
    boolean_T stopRequestedFlag;
    time_T *t;
    time_T tArray[2];
  } Timing;
};

/* Block signals (auto storage) */
extern BlockIO_Regelglied Regelglied_B;

/* Continuous states (auto storage) */
extern ContinuousStates_Regelglied Regelglied_X;

/* Block states (auto storage) */
extern D_Work_Regelglied Regelglied_DWork;

/* External inputs (root inport signals with auto storage) */
#ifdef __cplusplus

extern "C" {

#endif

  extern ExternalInputs_Regelglied Regelglied_U;

#ifdef __cplusplus

}
#endif

/* External outputs (root outports fed by signals with auto storage) */
#ifdef __cplusplus

extern "C" {

#endif

  extern ExternalOutputs_Regelglied Regelglied_Y;

#ifdef __cplusplus

}
#endif

/*
 * Exported Global Signals
 *
 * Note: Exported global signals are block signals with an exported global
 * storage class designation.  Code generation will declare the memory for
 * these signals and export their symbols.
 *
 */
extern real_T Xd_Roll;                 /* '<S1>/Add' */
extern real_T p_roll_m;                /* '<S1>/Gain' */
extern real_T i_roll_m;                /* '<S1>/Integrator' */
extern real_T d_roll_m;                /* '<S1>/Gain2' */
extern real_T p_nick_m;                /* '<S1>/Gain3' */
extern real_T i_nick_m;                /* '<S1>/Integrator1' */
extern real_T d_nick_m;                /* '<S1>/Gain5' */

/*
 * Exported Global Parameters
 *
 * Note: Exported global parameters are tunable parameters with an exported
 * global storage class designation.  Code generation will declare the memory for
 * these parameters and exports their symbols.
 *
 */
extern real_T d_nick;                  /* Variable: d_nick
                                        * Referenced by: '<S1>/Gain5'
                                        */
extern real_T d_roll;                  /* Variable: d_roll
                                        * Referenced by: '<S1>/Gain2'
                                        */
extern real_T gain_nick;               /* Variable: gain_nick
                                        * Referenced by: '<S1>/Gain7'
                                        */
extern real_T gain_roll;               /* Variable: gain_roll
                                        * Referenced by: '<S1>/Gain6'
                                        */
extern real_T i_nick;                  /* Variable: i_nick
                                        * Referenced by: '<S1>/Gain4'
                                        */
extern real_T i_roll;                  /* Variable: i_roll
                                        * Referenced by: '<S1>/Gain1'
                                        */
extern real_T kp_nick;                 /* Variable: kp_nick
                                        * Referenced by: '<S1>/Gain3'
                                        */
extern real_T kp_roll;                 /* Variable: kp_roll
                                        * Referenced by: '<S1>/Gain'
                                        */

#ifdef __cplusplus

extern "C" {

#endif

  /* Model entry point functions */
  extern void Regelglied_initialize(void);
  extern void Regelglied_step(void);
  extern void Regelglied_terminate(void);

#ifdef __cplusplus

}
#endif

/* Real-time Model object */
#ifdef __cplusplus

extern "C" {

#endif

  extern struct RT_MODEL_Regelglied *const Regelglied_M;

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
 * hilite_system('regler_new/Regelkreis/Regelglied')    - opens subsystem regler_new/Regelkreis/Regelglied
 * hilite_system('regler_new/Regelkreis/Regelglied/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'regler_new/Regelkreis'
 * '<S1>'   : 'regler_new/Regelkreis/Regelglied'
 * '<S2>'   : 'regler_new/Regelkreis/Regelglied/Throttlecutoff'
 * '<S3>'   : 'regler_new/Regelkreis/Regelglied/Throttlecutoff/If Action Subsystem'
 * '<S4>'   : 'regler_new/Regelkreis/Regelglied/Throttlecutoff/If Action Subsystem1'
 */
#endif                                 /* RTW_HEADER_Regelglied_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
