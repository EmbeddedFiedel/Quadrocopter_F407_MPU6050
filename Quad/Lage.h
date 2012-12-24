#include "ch.h"
#include "hal.h"
void dmpDataReady();
void setup_IMU(); 

void mpu_6050_interrupt(EXTDriver *extp, expchannel_t channel);
void update_IMU();
float get_euler_nick_ist();
float get_euler_roll_ist();
float get_euler_yaw_ist();
float get_rate_nick_ist();
float get_rate_roll_ist();
float get_rate_yaw_ist();