#include "ch.h"
#include "hal.h"
void dmpDataReady();
void setup_IMU(); 

void mpu_6050_interrupt(EXTDriver *extp, expchannel_t channel);
void update_IMU();
float getEuler_nick();
float getEuler_roll();
float getEuler_yaw();
float getRate_nick();
float getRate_roll();
float getRate_yaw();