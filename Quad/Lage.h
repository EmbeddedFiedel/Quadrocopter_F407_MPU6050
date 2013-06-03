#include "ch.h"
#include "hal.h"

void dmpDataReady();
void setup_IMU(); 
void I2CInitialize();

void mpu_6050_interrupt(EXTDriver *extp, expchannel_t channel);
void update_IMU();
float get_euler_nick_ist();
float get_euler_roll_ist();
float get_euler_yaw_ist();
float get_rate_nick_ist();
float get_rate_roll_ist();
float get_rate_yaw_ist();
float get_ypr_nick_ist();
float get_ypr_roll_ist();
float get_ypr_yaw_ist();
float get_accel_world_x();
float get_accel_world_y();
float get_accel_world_z();
float get_accel_x();
float get_accel_y();
float get_accel_z();
float get_accel_real_x();
float get_accel_real_y();
float get_accel_real_z();
uint16_t get_fifo_count();
