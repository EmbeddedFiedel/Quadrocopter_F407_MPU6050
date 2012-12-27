/*
 * Copyright 2009-2010 The MathWorks, Inc.
 *
 * File: sl_types_def.h
 *
 * Abstract:
 *   The embedded RTW code formats do not include simstruc.h, but
 *   needs these common types.
 *   Generated from sl_types_def.tpl.
 */

#ifndef __SL_TYPES_DEF_H__
#define __SL_TYPES_DEF_H__

#include "tmwtypes.h"

/* Enumeration of built-in data types */
typedef enum {
    SS_DOUBLE  =  0,    /* real_T    */
    SS_SINGLE  =  1,    /* real32_T  */
    SS_INT8    =  2,    /* int8_T    */
    SS_UINT8   =  3,    /* uint8_T   */
    SS_INT16   =  4,    /* int16_T   */
    SS_UINT16  =  5,    /* uint16_T  */
    SS_INT32   =  6,    /* int32_T   */
    SS_UINT32  =  7,    /* uint32_T  */
    SS_BOOLEAN =  8     /* boolean_T */
} BuiltInDTypeId;

#define SS_NUM_BUILT_IN_DTYPE ((int_T)SS_BOOLEAN+1)

#ifndef _DTYPEID
#  define _DTYPEID
   /* Enumeration for MAT-file logging code */
   typedef int_T DTypeId;
#endif

typedef enum {
    SS_FCN_CALL = 9, 
    SS_INTEGER = 10,
    SS_POINTER = 11,
    SS_INTERNAL_DTYPE2 = 12, 
    SS_TIMER_UINT32_PAIR = 13

    /* if last in list changes also update define below */

} PreDefinedDTypeId;

#define SS_DOUBLE_UINT32  SS_TIMER_UINT32_PAIR

#define SS_NUM_PREDEFINED_DTYPES  5

/*
 * DYNAMICALLY_TYPED - Specify for input/output port data types that can
 * accept a variety of data types.
 */
#define DYNAMICALLY_TYPED            (-1)

#endif /* __SL_TYPES_DEF_H__ */
