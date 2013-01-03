/*
 * File: Input_Kennlinie.h
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

#ifndef RTW_HEADER_Input_Kennlinie_h_
#define RTW_HEADER_Input_Kennlinie_h_
#ifndef Input_Kennlinie_COMMON_INCLUDES_
# define Input_Kennlinie_COMMON_INCLUDES_
#include <string.h>
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#endif                                 /* Input_Kennlinie_COMMON_INCLUDES_ */

#include "Input_Kennlinie_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

#ifndef rtmGetT
# define rtmGetT(rtm)                  (rtmGetTPtr((rtm))[0])
#endif

/* External outputs (root outports fed by signals with auto storage) */
typedef struct {
  real_T Winkel;                       /* '<Root>/Winkel' */
  real_T Throttle;                     /* '<Root>/Throttle' */
} ExternalOutputs_Input_Kennlinie;

/* Real-time Model Data Structure */
struct RT_MODEL_Input_Kennlinie {
  const char_T * volatile errorStatus;
  RTWSolverInfo solverInfo;

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
    time_T *t;
    time_T tArray[2];
  } Timing;
};

/* External outputs (root outports fed by signals with auto storage) */
#ifdef __cplusplus

extern "C" {

#endif

  extern ExternalOutputs_Input_Kennlinie Input_Kennlinie_Y;

#ifdef __cplusplus

}
#endif

#ifdef __cplusplus

extern "C" {

#endif

  /* Model entry point functions */
  extern void Input_Kennlinie_initialize(void);
  extern void Input_Kennlinie_step(void);
  extern void Input_Kennlinie_terminate(void);

#ifdef __cplusplus

}
#endif

/* Real-time Model object */
#ifdef __cplusplus

extern "C" {

#endif

  extern struct RT_MODEL_Input_Kennlinie *const Input_Kennlinie_M;

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
 * hilite_system('regler/Input_Kennlinie')    - opens subsystem regler/Input_Kennlinie
 * hilite_system('regler/Input_Kennlinie/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'regler'
 * '<S1>'   : 'regler/Input_Kennlinie'
 * '<S2>'   : 'regler/Input_Kennlinie/Ramp1'
 * '<S3>'   : 'regler/Input_Kennlinie/Ramp2'
 * '<S4>'   : 'regler/Input_Kennlinie/Ramp3'
 * '<S5>'   : 'regler/Input_Kennlinie/Ramp4'
 */
#endif                                 /* RTW_HEADER_Input_Kennlinie_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
