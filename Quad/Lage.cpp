/* ============================================
I2Cdev device library code is placed under the MIT license
Copyright (c) 2012 Jeff Rowberg

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
===============================================
*/

#include "I2Cdev.h"
#include "Lage.h"
#include "MPU6050_9Axis_MotionApps41.cpp"
#include "ch.h"
#include "hal.h"

MPU6050 mpu;

// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
float euler[3];         // [psi, theta, phi]    Euler angle container
int32_t gyroRate[3];
float gyro_rate_float[3];

// ================================================================
// ===               INTERRUPT DETECTION ROUTINE                ===
// ================================================================

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() 
{
	mpuInterrupt = true;
}


// ================================================================
// ===                      INITIAL SETUP                       ===
// ================================================================

static const I2CConfig i2cfg1 = {
    OPMODE_I2C,
    100000,
    STD_DUTY_CYCLE,
};

void I2CInitialize(void)
{
  i2cInit();
	i2cStart(&I2CD1, &i2cfg1);
	// Link PB6 and PB6 to I2C1 function
	palSetPadMode(GPIOB, 6,  PAL_MODE_ALTERNATE(4)); 
	palSetPadMode(GPIOB, 7,  PAL_MODE_ALTERNATE(4)); 

	// startups. Pauses added just to be safe
	chThdSleepMilliseconds(100);
}

void setup_IMU() 
{
	I2CInitialize();
	mpu.initialize();
	devStatus = mpu.dmpInitialize();
	if (devStatus == 0) 
	{
		mpu.setDMPEnabled(true);
		mpuIntStatus = mpu.getIntStatus();
		dmpReady = true;
		packetSize = mpu.dmpGetFIFOPacketSize();
	} 
}
void update_IMU()
{
	mpuIntStatus = mpu.getIntStatus();
	// get current FIFO count
	fifoCount = mpu.getFIFOCount();
	// check for overflow (this should never happen unless our code is too inefficient)
	if ((mpuIntStatus & 0x10) || fifoCount == 1024)	mpu.resetFIFO();
	// otherwise, check for DMP data ready interrupt (this should happen frequently)
	else if (mpuIntStatus & 0x02) 
	{
		// wait for correct available data length, should be a VERY short wait
		while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();
		// read a packet from FIFO
		mpu.getFIFOBytes(fifoBuffer, packetSize);
		// track FIFO count here in case there is > 1 packet available
		// (this lets us immediately read more without waiting for an interrupt)
		fifoCount -= packetSize;
		mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetEuler(euler, &q);
		mpu.dmpGetGyro(gyroRate,fifoBuffer);
		gyro_rate_float[0] = (float)gyroRate[0]/2147483648*2000*0.41;
		gyro_rate_float[1] = (float)gyroRate[1]/2147483648*2000*0.41;
		gyro_rate_float[2] = (float)gyroRate[2]/2147483648*2000*0.41;
	}
}

float getEuler_nick() {return euler[1];}
float getEuler_roll() {return euler[2];}
float getEuler_yaw() {return euler[0];}
float getRate_nick() {return gyro_rate_float[1];}
float getRate_roll() {return gyro_rate_float[0];}
float getRate_yaw() {return gyro_rate_float[2];}