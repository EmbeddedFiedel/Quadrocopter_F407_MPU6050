#include "Motoren.h"
#include "ch.h"
#include "hal.h"

/*
 * PWM configuration structure.
 * Cyclic callback enabled, channels 1 and 4 enabled without callbacks,
 * the active state is a logic one.
 */
static PWMConfig pwmcfg_esc = {
  1000000,                                    /* 10kHz PWM clock frequency.   */
  20000,                                    /* PWM period 20ms (in ticks).    */
  NULL,
  {
    {PWM_OUTPUT_ACTIVE_HIGH, NULL},
    {PWM_OUTPUT_ACTIVE_HIGH, NULL},
    {PWM_OUTPUT_ACTIVE_HIGH, NULL},
    {PWM_OUTPUT_ACTIVE_HIGH, NULL}
  },
  /* HW dependent part.*/
  0
};
static int width1 = 700, width2 = 700,width3 = 700,width4 = 700;

void setup_Motoren()
{
	/*
	* Initializes the PWM driver 5 for ESCs
	*/
	palSetPadMode(GPIOA, 0, PAL_MODE_ALTERNATE(2)); 
	palSetPadMode(GPIOA, 1, PAL_MODE_ALTERNATE(2));  
	palSetPadMode(GPIOA, 2, PAL_MODE_ALTERNATE(2));
	palSetPadMode(GPIOA, 3, PAL_MODE_ALTERNATE(2)); 
	pwmStart(&PWMD5, &pwmcfg_esc);

	width1 = 700; width2 = 700; width3 = 700;	width4 = 700;
  pwmEnableChannel(&PWMD5, 0, width1);
  pwmEnableChannel(&PWMD5, 1, width2);
  pwmEnableChannel(&PWMD5, 2, width3);
  pwmEnableChannel(&PWMD5, 3, width4);
}


void setMotor_1(float value)
{
	width1 = value*1000+1000;
	pwmEnableChannel(&PWMD5, 0, width1);
}
void setMotor_2(float value)
{
	width2 = value*1000+1000;
  pwmEnableChannel(&PWMD5, 1, width2);
}
void setMotor_3(float value)
{
	width3 = value*1000+1000;
  pwmEnableChannel(&PWMD5, 2, width3);
}
void setMotor_4(float value)
{
	width4 = value*1000+1000;
  pwmEnableChannel(&PWMD5, 3, width4);
}