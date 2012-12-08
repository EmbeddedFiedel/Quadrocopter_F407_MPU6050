
#include "ch.h"
#include "hal.h"
void setup_Fernsteuerung(void); 

float getNick(void);
float getRoll(void);
float getSchub(void);
float getYaw(void);

void rx_channel1_interrupt(EXTDriver *extp, expchannel_t channel);
void rx_channel2_interrupt(EXTDriver *extp, expchannel_t channel);
void rx_channel3_interrupt(EXTDriver *extp, expchannel_t channel);
void rx_channel4_interrupt(EXTDriver *extp, expchannel_t channel);
