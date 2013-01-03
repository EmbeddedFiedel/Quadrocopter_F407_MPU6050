/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

#include "ch.h"
#include "hal.h"
#include "test.h"
#include "lis302dl.h"
#include "chprintf.h"
#include "Lage.h"
#include "Fernsteuerung.h"
#include "Motoren.h"
#include "Regelung.h"
#include "Datalogger.h"
#include "ExternalInterrupt.h"
#include "mavlink.h"
#include "common.h"


mavlink_system_t mavlink_system;

void send_heartbeat(void)
{
	// Define the system type, in this case an airplane
	uint8_t system_type = MAV_TYPE_QUADROTOR;
	uint8_t autopilot_type = MAV_AUTOPILOT_GENERIC;
	 
	uint8_t system_mode = MAV_MODE_STABILIZE_ARMED; ///< Booting up
	uint32_t custom_mode = 0;                 ///< Custom mode, can be defined by user/adopter
	uint8_t system_state = MAV_STATE_ACTIVE; ///< System ready for flight
	 
	// Initialize the required buffers
	mavlink_message_t msg;
	uint8_t buf[MAVLINK_MAX_PACKET_LEN];
	 
	// Pack the message
	mavlink_msg_heartbeat_pack(mavlink_system.sysid, mavlink_system.compid, &msg, system_type, autopilot_type, system_mode, custom_mode, system_state);
	 
	// Copy the message to the send buffer
	uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
	// Send the message with the standard UART send function
	// uart0_send might be named differently depending on
	// the individual microcontroller / library in use.
	sdAsynchronousWrite(&SD2, buf, len);
}

/*
 * Working area for MavlinkHeartbeatThread
 */
static WORKING_AREA(MavlinkHeartbeatThreadWorkingArea, 2048);
/*
 * MavlinkHeartbeatThread
 */

static msg_t MavlinkHeartbeatThread(void *arg) 
{
  while (TRUE) 
  {
		send_heartbeat();	 
		chThdSleepMilliseconds(1000);
  }
}


void send_attitude(void)
{
    // Initialize the required buffers
	mavlink_message_t msg;
	uint8_t buf[MAVLINK_MAX_PACKET_LEN];
    
    uint32_t time_boot_ms = chTimeNow();
    float roll = get_euler_roll_ist();
    float pitch = get_euler_nick_ist();
    float yaw = get_euler_yaw_ist();
    float rollspeed = get_rate_roll_ist();
    float pitchspeed = get_rate_nick_ist();
    float yawspeed = get_rate_yaw_ist();

	// Pack the message
    mavlink_msg_attitude_pack(mavlink_system.sysid, mavlink_system.compid, &msg, time_boot_ms, roll, pitch, yaw, rollspeed, pitchspeed, yawspeed);
    
    // Copy the message to the send buffer
	uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
	// Send the message with the standard UART send function
	// uart0_send might be named differently depending on
	// the individual microcontroller / library in use.
	//sdAsynchronousWrite(&SD2, buf, len);
	sdWrite(&SD2, buf, len);
}
/*
 * Working area for MavlinkHeartbeatThread
 */
static WORKING_AREA(MavlinkAttitudeThreadWorkingArea, 2048);
/*
 * MavlinkHeartbeatThread
 */

static msg_t MavlinkAttitudeThread(void *arg)
{
    while (TRUE)
    {
		send_attitude();
		chThdSleepMilliseconds(1000);
    }
}

void setup_Mavlink()
{
    /*
     * Activates the serial driver 2 using the driver default configuration.
     * PA2(TX) and PA3(RX) are routed to USART2.
     */
	sdStart(&SD2, NULL);
	palSetPadMode(GPIOD, 5, PAL_MODE_ALTERNATE(7));
	palSetPadMode(GPIOD, 6, PAL_MODE_ALTERNATE(7));
    
    mavlink_system.sysid = 1;                   ///< ID 1 for this airplane
	mavlink_system.compid = MAV_COMP_ID_ALL;     ///< The component sending the message is the IMU, it could be also a Linux process
    
	chThdCreateStatic(MavlinkHeartbeatThreadWorkingArea, sizeof(MavlinkHeartbeatThreadWorkingArea), NORMALPRIO, MavlinkHeartbeatThread, NULL);
    chThdSleepMilliseconds(400);
	chThdCreateStatic(MavlinkAttitudeThreadWorkingArea, sizeof(MavlinkAttitudeThreadWorkingArea), NORMALPRIO, MavlinkAttitudeThread, NULL);
}



/*
 * Application entry point.
 */
int main(void) 
{
	/*
	* System initializations.
	* - HAL initialization, this also initializes the configured device drivers
	*   and performs the board-specific initializations.
	* - Kernel initialization, the main() function becomes a thread and the
	*   RTOS is active.
	*/
	halInit();	   
	chSysInit();
	
	setup_IMU();
	setup_ExternalInterrupt();
	setup_Fernsteuerung();
	setup_Motoren();
	setup_Regelung();
	setup_Datalogger(); 
	setup_Mavlink();
	/*
	* Normal main() thread activity, in this demo it does nothing except
	* sleeping in a loop and check the button state, when the button is
	* pressed the test procedure is launched with output on the serial
	* driver 2.
	*/
	while (TRUE) 
	{
		update_IMU();
    }
}
