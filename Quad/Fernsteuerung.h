
#include "ch.h"
#include "hal.h"
void setup_Fernsteuerung(void); 

float get_schub_soll(void);
float get_euler_nick_soll(void);
float get_euler_roll_soll(void);
float get_euler_yaw_soll(void);

int16_t get_chan1_scaled();
int16_t get_chan2_scaled();
int16_t get_chan3_scaled();
int16_t get_chan4_scaled();

void rx_channel1_interrupt(EXTDriver *extp, expchannel_t channel);
void rx_channel2_interrupt(EXTDriver *extp, expchannel_t channel);
void rx_channel3_interrupt(EXTDriver *extp, expchannel_t channel);
void rx_channel4_interrupt(EXTDriver *extp, expchannel_t channel);

void calib_interrupt(EXTDriver *extp, expchannel_t channel);
void start_calib();
void stop_calib();
bool get_rc_calibration_ready();


static void gpt2cb(GPTDriver *gptp);
void timer_handler(void *arg);
