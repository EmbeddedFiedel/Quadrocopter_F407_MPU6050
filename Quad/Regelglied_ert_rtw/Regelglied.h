/*
 * File: Regelglied.h
 *
 * Code generated for Simulink model 'Regelglied'.
 *
 * Model version                  : 1.365
 403imulink Coder version         : 8.2 (R2012a) 29-Dec-2011
 * TLC version                    : 8.2 (Dec 29 2011)
 * C/C++ source code generated on : Sat Dec 2un Dec 30 18:04:57
 * Target selection: ert.tlc
 * Embedded hardware selection: 32-bit Generic
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
#include "rt_nonfinite.h"
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
  real_T Add;                          /* '<S1>/Add' */
  real_T Add3;lglied;

/* Block states (auto storage) for system '<Root>' */
typedef struct {
  struct {
    real_T TimeStampA;
    real_T LastUAtTimeA;
    real_T TimeStampB;
    real_T LastUAtTimeB;
  } Derivative_RWORK;                  /* '<S1>/Derivative' */

  struct {
    real_T TimeStampA;
    real_T LastUAtTimeA;
    real_T TimeStampB;
    real_T LastUAtTimeB;
  } Derivative1_RWORK;                 /* '<S1>/Derivative1' */

  struct {
    real_T Tim4_RWORK;                 /* '<S1>/Derivative4TE;               /* '<S7>/I-Anteil' */
  real_T DAnteil_CSTATE;               /* '<S7>/D-Anteil' */
  real_T IAnteil_CSTATE_h;             /* '<S6>/I-Anteil' */
  real_T DAnteil_CSTATE_f;             /* '<S6>/D-Anteil' *IAnteil_CSTATE_h;             /* '<S6>/I-Anteil' */
  real_T DAnteil_CSTATE_f;             /* '<S6>/Deil_CSTATE;               /* '<S7>/I-Anteil' */
  real_T DAnteil_CSTATE;               /* '<S7>/D-Anteil' */
  real_T IAnteil_CSTATE_h;             /* '<S6>/I-Anteil' */
  real_T DAnteil_CSTATE_f;             /* '<S6>/D-Anteil' */
} StatDIAnteil_CSTATE_h;             /* '<S6>/I-Anteil' */
  real_T DAnteil_CSTATE_f;             /* '<S6>/D/* '<S7>/I-Anteil' */
  boolean_T DAnteil_CSTATE;            /* '<S7>/D-Anteil' */
  boolean_T IAnteil_CSTATE_h;          /* '<S6>/I-Anteil' */
  boolean_T DAnteil_CSTATE_f;          /* '<S6>/D-Anteil' */
} StateDisabled_RDAIAnteil_CSTATE_h;          /* '<S6>/I-Anteil' */
  boolean_T DAnteil_CSTATE_f;          /* '<S6>/D *y;                           /* output */
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
    real_T odeY[4];
    real_T odeF[3][4];
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
 * Exported Global Parameters
 *
 * Note: Exported global parameters are tunable parameters with an exported
 * global storage class designation.  Code generation will declare the memory for
 * these parameters and exports their symbols.
 *
 */
extern real_T kp_a_nicSignals
 *
 * Note: Exported global signals are block signals with an exported global
 * storage class designation.  Code generation will declare the memory for
 * these signals and export their symbols.
 *
 */
extern real_T xd_roll;                 /* '<S1>/Add' */
extern real_T p_anteil;                /* '<S7>/Gain' */
extern real_T i_anteil;                /* '<S7>/I-Anteil' */
extern real_T d_anteil;                /* '<S7>/D-Anteil' */
extern real_T y_roll;                  /* '<S7>/Add' */
extern real_T Xd_V_Roll;               /* '<S1>/Add1' */
extern real_T Xf_a_Roll;               /* '<S1>/Add2' */
extern real_T Soll_V_Roll;             /* '<S1>/Derivative' */
extern real_T Ist_a_Roll;              /* '<S1>/Derivative4' */
 */
extern real_T kp_a_nick;               /* Variable: kp_a_nick
                                        * Referenced by: '<S2>/Gain'
                                        */
extern real_T kp_a_roll;               /* Variable: kp_a_roll
                                      eferenced by: '<S6nick;               /* Variable: kp_a_nick
                                        * Referenced by: '<S2>/Gain'
                                             eferenced by: '<S6>/Gain'
                                        */
extern real_T kp_roll;                 /* Variable: kp_roll
                                        * Refeenced by: '<S3>/Ganick;                 /* Variable: kp_nick
                                        * Referenced by: '<S6_roll
                                        * Refeenced by: '<S3>/Gain'
                                        */
extern real_T kp_v_roll;               /* Variable: kp_v_roll
                                        * Referencvoid);
  extern vonick;               /* Variable: kp_v_nick
                                        * Referenced by: '<S3oll
                                        * Referencvoid);
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
 * hilite_system('regler/Regelkreis/Regelglied')    - opens subsystem regler/Regelkreis/Regelglied
 * hilite_system('regler/Regelkreis/Regelglied/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'regler/Regelkreis'
 * '<S1>'   : 'regler/Regelkreis/Regelglied'
 * '<S2>'   : 'regler/Regelkreis/Regelglied/P Regler Nickwinkelbeschleunigung'
 * '<S3>'   : 'regler/Regelkreis/Regelglied/P Regler Nickwinkelgeschwindigkeit'
 * '<S4>'   : 'regler/Regelkreis/Regelglied/P Regler Rollwinkelbeschleunigung'
 * '<S5>'   : 'regler/Regelkreis/Regelglied/P Regler Rollwinkelgeschwindigkeit'
 * '<S6>'   : 'regler/Regelkreis/Regelglied/PID Regler Nick'
 * '<S7>'   : 'regler/Regelkreis/Regelglied/PID Regler Roll'
 */
#endif                                 /* RTW_HEADER_Regelglied_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
