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
#include "MPU6050_6Axis_MotionApps20.h"
#include "chprintf.h"
#include "Datalogger.h"
#include "tm.h"
#include "ff.h"
#include "barometer.h"

MPU6050 mpu;

// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus_mpu, devStatus_baro ;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
float euler[3];         // [psi, theta, phi]    Euler angle container
// VectorInt16 accel;					//3D acceleration vector
// VectorInt16 accel_world;		//3D acceleration vector rotated into world
// VectorInt16 accel_linear;		//acceleration without world acceleration
VectorInt16 aa; // [x, y, z] accel sensor measurements
VectorInt16 aa_offset_sum; //[x, y, z] accel sensor offeset sum
VectorInt16 aaReal; // [x, y, z] gravity-free accel sensor measurements
VectorInt16 aaWorld; // [x, y, z] world-frame accel sensor measurements
int32_t gyroRate[3];
float gyro_rate_float[3];
VectorFloat gravity; // [x, y, z] gravity vector
float ypr[3]; // [yaw, pitch, roll] yaw/pitch/roll container and gravity vector
float offset_test=0;

static FIL Fil_Lage;			/* File object */
FRESULT rc_lage;				/* Result code */


bool_t updating_imu=0;
bool_t datalog_lage_opened = 0;
bool_t datalog_lage_syncing = 0;
Thread *tp_mpu,*tp_baro;
/*
 * Working area for LageSync
 */
static WORKING_AREA(LageSyncThreadWorkingArea, 2048);
/*
 * LageSync
 */
static msg_t LageSyncthread(void *arg) {
 
  systime_t time = chTimeNow();     // Tnow
  while (TRUE) 
	{
		if(Datalogger_ready() && !datalog_lage_opened)
		{
				//rc_lage = f_mkfs(0,0,0);
				rc_lage = f_open(&Fil_Lage, ("QuadLage.TXT"), FA_WRITE | FA_CREATE_ALWAYS);
				if(rc_lage != FR_OK)
				{
					chprintf((BaseChannel *) &SD2, "SD QuadLage.TXT: f_open() failed %d\r\n", rc_lage);
				}	
				//rc = f_printf(&Fil, "moin\r\n");	 
				rc_lage = f_sync(&Fil_Lage);
				if(rc_lage != FR_OK)
				{
					chprintf((BaseChannel *) &SD2, "SD QuadLage.TXT: f_sync() failed %d\r\n", rc_lage);
				}	
				f_printf(&Fil_Lage, "Time_Lage; Nick_Lage; Roll_Lage; Yaw_Lage\r\n");
				rc_lage = f_sync(&Fil_Lage);
				if(rc_lage != FR_OK)
				{
					chprintf((BaseChannel *) &SD2, "SD QuadLage.TXT: f_sync() failed %d\r\n", rc_lage);
				}	
				else
				{
				datalog_lage_opened = TRUE;
				chprintf((BaseChannel *) &SD2, "SD QuadLage.TXT: opened successfull\r\n");
				}
				chThdSleepMilliseconds(10);

		}
		else if(Datalogger_ready() && datalog_lage_opened)
		{
		datalog_lage_syncing = 1;
		rc_lage = f_sync(&Fil_Lage);
		datalog_lage_syncing = 0;
		chThdSleepMilliseconds(700);
		}
  }
}





static TimeMeasurement lagedatalogsync_tmup;
void datalog_lage(void)
{
	uint32_t system_time;
	
		if(Datalogger_ready() && datalog_lage_opened && datalog_lage_syncing == 0)
		{
			//int worst, last, best;
			system_time = chTimeNow();
			f_printf(&Fil_Lage, "%d;%d;%d;%d\r\n",system_time,(int)(euler[1]*100),(int)(euler[2]*100),(int)(euler[0]*100));
			//tmStartMeasurement(&lagedatalogsync_tmup);
			//rc_lage = f_sync(&Fil_Lage);
			//tmStopMeasurement(&lagedatalogsync_tmup);
			//best = RTT2MS(lagedatalogsync_tmup.best);
			//last = RTT2MS(lagedatalogsync_tmup.last);
			//worst = RTT2MS(lagedatalogsync_tmup.worst);
			//chprintf((BaseChannel *) &SD2, "Lage SD Sync Best:%d Worst:%d Last:%d \r\n",best ,worst , last);
		}
}

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

void I2CInitialize()
{
  i2cInit();
	i2cStart(&I2CD1, &i2cfg1);
	// Link PB6 and PB6 to I2C1 function
	palSetPadMode(GPIOB, 6,  PAL_MODE_ALTERNATE(4)); 
	palSetPadMode(GPIOB, 7,  PAL_MODE_ALTERNATE(4)); 

	// startups. Pauses added just to be safe
	chThdSleepMilliseconds(100);
}


static WORKING_AREA(LageReadThreadWorkingArea, 1024);
/*
* LageRead
*/
static msg_t LageReadThread(void *arg)
{
	static int i=0;
	while(TRUE)
	{
		update_IMU();
		chThdSleepMilliseconds(1);
// 		
// 	i++;
// 		if ((i==100)||(i==0)){
// 		baro_read_all();
// 		i=1;
// 		}
// 		else
// 		{
// 		update_IMU();
// 		}
// 		chThdSleepMilliseconds(1);
	}
}


void setup_IMU() 
{
	tmObjectInit(&lagedatalogsync_tmup);
	I2CInitialize();
	mpu.initialize();
	devStatus_mpu = mpu.dmpInitialize();
	devStatus_baro=setup_barometer();
	if ((devStatus_mpu||(!devStatus_baro)) == 0) 
	{
		//chThdCreateStatic(LageSyncThreadWorkingArea, sizeof(LageSyncThreadWorkingArea), NORMALPRIO, LageSyncthread, NULL);
		mpu.setDMPEnabled(true);
		mpuIntStatus = mpu.getIntStatus();
		dmpReady = true;
		packetSize = mpu.dmpGetFIFOPacketSize();
		mpu.setFullScaleAccelRange(MPU6050_ACCEL_FS_4);
		
		tp_mpu=chThdCreateStatic(LageReadThreadWorkingArea, sizeof(LageReadThreadWorkingArea), HIGHPRIO, LageReadThread, NULL);
		
		
	} 
}



void mpu_6050_interrupt(EXTDriver *extp, expchannel_t channel) 
{
		(void)extp;
		(void)channel;
	
		chSysLockFromIsr();
		if (palReadPad(GPIOD, 7) == PAL_HIGH) 
		{
			mpuInterrupt = true;
		}
		chSysUnlockFromIsr();
}

int zaehler = 0;
void update_IMU()
{
	
	static int cali_accel=0;
	if(mpuInterrupt)
	{
		updating_imu=1;
		mpuInterrupt = FALSE;
		mpuIntStatus = mpu.getIntStatus();
		// get current FIFO count
		fifoCount = mpu.getFIFOCount();
		// check for overflow (this should never happen unless our code is too inefficient)
		if ((mpuIntStatus & 0x10) || fifoCount == 1024)
		{
			mpu.resetFIFO();
		}
		// otherwise, check for DMP data ready interrupt (this should happen frequently)
		else if (mpuIntStatus & 0x02) 
		{
			// wait for correct available data length, should be a VERY short wait
			while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();
			// read a packet from FIFO
			for(int i = 0; i<(fifoCount/packetSize); i++)
			{
				mpu.getFIFOBytes(fifoBuffer, packetSize);
				chThdSleepMilliseconds(1);
			}
				// track FIFO count here in case there is > 1 packet available
				// (this lets us immediately read more without waiting for an interrupt)
				//fifoCount -= packetSize;
				mpu.dmpGetQuaternion(&q, fifoBuffer);
				mpu.dmpGetGravity(&gravity, &q);
				mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
				mpu.dmpGetEuler(euler, &q);
			  mpu.dmpGetAccel(&aa, fifoBuffer);
				//calibration of the accerleration only the first 10 steps please hold the device in horizontal position 
				if((cali_accel<50)&&((aa.x!=0)||(aa.y!=0)||(aa.z!=0))){
					cali_accel++; // je nach dem noch verbesserungwürde bzgl. laufzeit
					aa_offset_sum.x=aa_offset_sum.x+0-aa.x;//0 because of the horizontal position 
					aa_offset_sum.y=aa_offset_sum.y+0-aa.y;//0 because of the horizontal position
					aa_offset_sum.z=aa_offset_sum.z+4096-aa.z;//4096 because of the horizontal position
					//könnte noch über den mpu offset gelöst werden.
				}
				aa.x=aa.x+(aa_offset_sum.x/cali_accel);
				aa.y=aa.y+(aa_offset_sum.y/cali_accel);
				aa.z=aa.z+(aa_offset_sum.z/cali_accel);
        mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
        mpu.dmpGetLinearAccelInWorld(&aaWorld, &aaReal, &q);
				mpu.dmpGetGyro(gyroRate,fifoBuffer);
				gyro_rate_float[0] = (float)gyroRate[0]/2147483648*2000*0.41;
				gyro_rate_float[1] = (float)gyroRate[1]/2147483648*2000*0.41;
				gyro_rate_float[2] = (float)gyroRate[2]/2147483648*2000*0.41;
				fifoCount = mpu.getFIFOCount();
				updating_imu=0;
		}
		else
		{
			zaehler++;
		}
	}
}

bool_t get_updateing_imu(){return updating_imu;}
float get_euler_nick_ist() {return euler[1];}
float get_euler_roll_ist() {return euler[2];}
float get_euler_yaw_ist() {return euler[0];}
float get_rate_nick_ist() {return gyro_rate_float[1];}
float get_rate_roll_ist() {return gyro_rate_float[0];}
float get_rate_yaw_ist() {return gyro_rate_float[2];}
float get_ypr_nick_ist() {return ypr[1];}
float get_ypr_roll_ist() {return ypr[2];}
float get_ypr_yaw_ist() {return ypr[0];}
float get_accel_x(){return (aa.x/4096.0)*9.80665;}
float get_accel_y(){return (aa.y/4096.0)*9.80665;}
float get_accel_z(){return (aa.z/4096.0)*9.80665;}
float get_accel_real_x(){return (aaReal.x/4096.0)*9.80665;}
float get_accel_real_y(){return (aaReal.y/4096.0)*9.80665;}
float get_accel_real_z(){return (aaReal.z/4096.0)*9.80665;}
float get_accel_world_x(){return (aaWorld.x/4096.0)*9.80665;}
float get_accel_world_y(){return (aaWorld.y/4096.0)*9.80665;}
float get_accel_world_z(){return (aaWorld.z/4096.0)*9.80665;}
uint16_t get_fifo_count() {return fifoCount;}
