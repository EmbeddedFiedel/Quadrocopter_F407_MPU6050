
#include "ch.h"
#include "hal.h"
void setup_Fernsteuerung(void); 

float get_schub_soll(void);
float get_euler_nick_soll(void);
float get_euler_roll_soll(void);
float get_euler_yaw_soll(void);

void rx_channel1_interrupt(EXTDriver *extp, expchannel_t channel);
void rx_channel2_interrupt(EXTDriver *extp, expchannel_t channel);
void rx_channel3_interrupt(EXTDriver *extp, expchannel_t channel);
void rx_channel4_interrupt(EXTDriver *extp, expchannel_t channel);
