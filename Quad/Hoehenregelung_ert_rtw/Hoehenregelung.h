/*
 * File: Hoehenregelung.h
 *
 * Code generated for Simulink model 'Hoehenregelung'.
 *
 * Model version                  : 1.505
 * Simulink Coder version         : 8.2 (R2012a) 29-Dec-2011
 * TLC version                    : 8.2 (Dec 29 2011)
 * C/C++ source code generated on : Wed Mar 06 15:59:46 2013
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 * Validation result: Passed (7), Warnings (5), Error (0)
 */

#ifndef RTW_HEADER_Hoehenregelung_h_
#define RTW_HEADER_Hoehenregelung_h_
#ifndef Hoehenregelung_COMMON_INCLUDES_
# define Hoehenregelung_COMMON_INCLUDES_
#include <string.h>
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#endif                                 /* Hoehenregelung_COMMON_INCLUDES_ */

#include "Hoehenregelung_types.h"

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
  real_T DeadZone;                     /* '<S1>/Dead Zone' */
} BlockIO_Hoehenregelung;

/* Block states (auto storage) for system '<Root>' */
typedef struct {
  struct {
    int_T IcNeedsLoading;
  } IntegratorLimited_IWORK;           /* '<S1>/Integrator Limited' */
} D_Work_Hoehenregelung;

/* Continuous states (auto storage) */
typedef struct {
  real_T Integrator_CSTATE;            /* '<S1>/Integrator' */
  real_T IntegratorLimited_CSTATE;     /* '<S1>/Integrator Limited' */
} ContinuousStates_Hoehenregelung;

/* State derivatives (auto storage) */
typedef struct {
  real_T Integrator_CSTATE;            /* '<S1>/Integrator' */
  real_T IntegratorLimited_CSTATE;     /* '<S1>/Integrator Limited' */
} StateDerivatives_Hoehenregelung;

/* State disabled  */
typedef struct {
  boolean_T Integrator_CSTATE;         /* '<S1>/Integrator' */
  boolean_T IntegratorLimited_CSTATE;  /* '<S1>/Integrator Limited' */
} StateDisabled_Hoehenregelung;

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
  real_T Throttle;                     /* '<Root>/Throttle' */
  real_T Hoehe;                        /* '<Root>/Hoehe' */
  real_T Beschleunigung_Z;             /* '<Root>/Beschleunigung_Z' */
} ExternalInputs_Hoehenregelung;

/* External outputs (root outports fed by signals with auto storage) */
typedef struct {
  real_T Schubkraft;                   /* '<Root>/Schubkraft' */
} ExternalOutputs_Hoehenregelung;

/* Real-time Model Data Structure */
struct RT_MODEL_Hoehenregelung {
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
extern BlockIO_Hoehenregelung Hoehenregelung_B;

/* Continuous states (auto storage) */
extern ContinuousStates_Hoehenregelung Hoehenregelung_X;

/* Block states (auto storage) */
extern D_Work_Hoehenregelung Hoehenregelung_DWork;

/* External inputs (root inport signals with auto storage) */
#ifdef __cplusplus

extern "C" {

#endif

  extern ExternalInputs_Hoehenregelung Hoehenregelung_U;

#ifdef __cplusplus

}
#endif

/* External outputs (root outports fed by signals with auto storage) */
#ifdef __cplusplus

extern "C" {

#endif

  extern ExternalOutputs_Hoehenregelung Hoehenregelung_Y;

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
extern real_T v_z;                     /* '<S1>/Integrator' */
extern real_T h_z;                     /* '<S1>/Integrator Limited' */

#ifdef __cplusplus

extern "C" {

#endif

  /* Model entry point functions */
  extern void Hoehenregelung_initialize(void);
  extern void Hoehenregelung_step(void);
  extern void Hoehenregelung_terminate(void);

#ifdef __cplusplus

}
#endif

/* Real-time Model object */
#ifdef __cplusplus

extern "C" {

#endif

  extern struct RT_MODEL_Hoehenregelung *const Hoehenregelung_M;

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
 * hilite_system('regler/Regelkreis/Hoehenregelung')    - opens subsystem regler/Regelkreis/Hoehenregelung
 * hilite_system('regler/Regelkreis/Hoehenregelung/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'regler/Regelkreis'
 * '<S1>'   : 'regler/Regelkreis/Hoehenregelung'
 */
#endif                                 /* RTW_HEADER_Hoehenregelung_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
