/*
 * File: rtGetInf.cpp
 *
 * Code generated for Simulink model 'Regelglied'.
 *
 * Model version                  : 1.365
 *91imulink Coder version         : 8.2 (R2012a) 29-Dec-2011
 * TLC version                    : 8.2 (Dec 29 2011)
 * C/C++ source code generated on : Sat Dec 2un Dec 30 12:54:15
 * Target selection: ert.tlc
 * Embedded hardware selection: 32-bit Generic
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 * Validation result: Passed (7), Warnings (5), Error (0)
 */

/*
 * Abstract:
 *      Function to intialize non-finite, Inf
 */
#include "rtGetInf.h"
#define NumBitsPerChar                 8U

extern "C" {
  /*
   * Initialize rtInf needed by the generated code.
   * Inf is initialized as non-signaling. Assumes IEEE.
   */
  real_T rtGetInf(void)
  {
    size_t bitsPerReal = sizeof(real_T) * (NumBitsPerChar);
    real_T inf = 0.0;
    if (bitsPerReal == 32U) {
      inf = rtGetInfF();
    } else {
      uint16_T one = 1U;
      enum {
        LittleEndian,
        BigEndian
      } machByteOrder = (*((uint8_T *) &one) == 1U) ? LittleEndian : BigEndian;
      switch (machByteOrder) {
       case LittleEndian:
        {
          union {
            LittleEndianIEEEDouble bitVal;
            real_T fltVal;
          } tmpVal;

          tmpVal.bitVal.words.wordH = 0x7FF00000U;
          tmpVal.bitVal.words.wordL = 0x00000000U;
          inf = tmpVal.fltVal;
          break;
        }

       case BigEndian:
        {
          union {
            BigEndianIEEEDouble bitVal;
            real_T fltVal;
          } tmpVal;

          tmpVal.bitVal.words.wordH = 0x7FF00000U;
          tmpVal.bitVal.words.wordL = 0x00000000U;
          inf = tmpVal.fltVal;
          break;
        }
      }
    }

    return inf;
  }

  /*
   * Initialize rtInfF needed by the generated code.
   * Inf is initialized as non-signaling. Assumes IEEE.
   */
  real32_T rtGetInfF(void)
  {
    IEEESingle infF;
    infF.wordL.wordLuint = 0x7F800000U;
    return infF.wordL.wordLreal;
  }

  /*
   * Initialize rtMinusInf needed by the generated code.
   * Inf is initialized as non-signaling. Assumes IEEE.
   */
  real_T rtGetMinusInf(void)
  {
    size_t bitsPerReal = sizeof(real_T) * (NumBitsPerChar);
    real_T minf = 0.0;
    if (bitsPerReal == 32U) {
      minf = rtGetMinusInfF();
    } else {
      uint16_T one = 1U;
      enum {
        LittleEndian,
        BigEndian
      } machByteOrder = (*((uint8_T *) &one) == 1U) ? LittleEndian : BigEndian;
      switch (machByteOrder) {
       case LittleEndian:
        {
          union {
            LittleEndianIEEEDouble bitVal;
            real_T fltVal;
          } tmpVal;

          tmpVal.bitVal.words.wordH = 0xFFF00000U;
          tmpVal.bitVal.words.wordL = 0x00000000U;
          minf = tmpVal.fltVal;
          break;
        }

       case BigEndian:
        {
          union {
            BigEndianIEEEDouble bitVal;
            real_T fltVal;
          } tmpVal;

          tmpVal.bitVal.words.wordH = 0xFFF00000U;
          tmpVal.bitVal.words.wordL = 0x00000000U;
          minf = tmpVal.fltVal;
          break;
        }
      }
    }

    return minf;
  }

  /*
   * Initialize rtMinusInfF needed by the generated code.
   * Inf is initialized as non-signaling. Assumes IEEE.
   */
  real32_T rtGetMinusInfF(void)
  {
    IEEESingle minfF;
    minfF.wordL.wordLuint = 0xFF800000U;
    return minfF.wordL.wordLreal;
  }
}
/*
 * File trailer for generated code.
 *
 * [EOF]
 */
