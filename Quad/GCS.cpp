#include "GCS.h"
#include "Lage.h"
#include "Fernsteuerung.h"
#include "Motoren.h"


#define ONBOARD_PARAM_COUNT 5
#define ONBOARD_PARAM_NAME_LENGTH 10
 
mavlink_system_t mavlink_system;

struct global_struct
{
	float param[ONBOARD_PARAM_COUNT];
	char param_name[ONBOARD_PARAM_COUNT][MAVLINK_MSG_PARAM_SET_FIELD_PARAM_ID_LEN];
};
 
struct global_struct global_data;
 
/**
 * @brief reset all parameters to default
 * @warning DO NOT USE THIS IN FLIGHT!
 */
static inline void global_data_reset_param_defaults(void)
{
	global_data.param[0] = 0;
	strcpy(global_data.param_name[0], "KB0");
		global_data.param[1] = 1;
	strcpy(global_data.param_name[1], "KB1");
		global_data.param[2] = 2;
	strcpy(global_data.param_name[2], "KB2");
		global_data.param[3] = 3;
	strcpy(global_data.param_name[3], "KB3");
		global_data.param[4] = 4;
	strcpy(global_data.param_name[4], "KB4");
} 

uint16_t m_parameter_i = 0;

/**
* @brief Send low-priority messages at a maximum rate of xx Hertz
*
* This function sends messages at a lower rate to not exceed the wireless
* bandwidth. It sends one message each time it is called until the buffer is empty.
* Call this function with xx Hertz to increase/decrease the bandwidth.
*/
static void communication_queued_send(void)
{
	//send parameters one by one
	if (m_parameter_i < ONBOARD_PARAM_COUNT)
	{
   // Initialize the required buffers
		mavlink_message_t msg;
		uint8_t buf[MAVLINK_MAX_PACKET_LEN];
		mavlink_msg_param_value_pack(mavlink_system.sysid, mavlink_system.compid, &msg,(char*) global_data.param_name[m_parameter_i],
				         global_data.param[m_parameter_i], MAVLINK_TYPE_FLOAT,
                                         ONBOARD_PARAM_COUNT, m_parameter_i);
		// Copy the message to the send buffer
	uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
    
	// Send the message with the standard UART send function
	sdWrite(&SD2, buf, len);
	//uartStartSend(&UARTD2, len, buf);
		m_parameter_i++;
	}
}

/*
 * Working area for MavlinkHeartbeatThread
 */
static WORKING_AREA(MavlinkParamlistThreadWorkingArea, 2048);
/*
 * MavlinkHeartbeatThread
 */

static msg_t MavlinkParamlistThread(void *arg)
{
    while (TRUE)
    {
		communication_queued_send();
		chThdSleepMilliseconds(3);
    }
}

void send_heartbeat(void)
{
	// Define the system type, in this case an airplane
	uint8_t system_type = MAV_TYPE_QUADROTOR;
	uint8_t autopilot_type = MAV_AUTOPILOT_GENERIC;
	 
	uint8_t system_mode = MAV_MODE_PREFLIGHT; ///< Booting up
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
	sdWrite(&SD2, buf, len);
	//uartStartSend(&UARTD2, len, buf);
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
    float roll = get_ypr_roll_ist();
    float pitch = get_ypr_nick_ist();
    float yaw = get_ypr_yaw_ist();
    float rollspeed = get_rate_roll_ist();
    float pitchspeed = get_rate_nick_ist();
    float yawspeed = get_rate_yaw_ist();

	// Pack the message
    mavlink_msg_attitude_pack(mavlink_system.sysid, mavlink_system.compid, &msg, time_boot_ms, roll, pitch, yaw, rollspeed, pitchspeed, yawspeed);
    
    // Copy the message to the send buffer
	uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
    
	// Send the message with the standard UART send function
	sdWrite(&SD2, buf, len);
	//uartStartSend(&UARTD2, len, buf);
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
		chThdSleepMilliseconds(15);
    }
}


void send_rc_channels_scaled(void)
{
    // Initialize the required buffers
	mavlink_message_t msg;
	uint8_t buf[MAVLINK_MAX_PACKET_LEN];
    
    uint32_t time_boot_ms = chTimeNow();
    uint8_t port = 0; //????? Sinn?
    int16_t chan1_scaled = get_chan1_scaled();
    int16_t chan2_scaled = get_chan2_scaled();
    int16_t chan3_scaled = get_chan3_scaled();
    int16_t chan4_scaled = get_chan4_scaled();
    int16_t chan5_scaled = 32767;
    int16_t chan6_scaled = 32767;
    int16_t chan7_scaled = 32767;
    int16_t chan8_scaled = 32767;
    uint8_t rssi = 255;
    
    mavlink_msg_rc_channels_scaled_pack(mavlink_system.sysid, mavlink_system.compid, &msg, time_boot_ms, port, chan1_scaled, chan2_scaled, chan3_scaled, chan4_scaled, chan5_scaled, chan6_scaled, chan7_scaled, chan8_scaled, rssi);
    
    // Copy the message to the send buffer
	uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
    
	// Send the message with the standard UART send function
	sdWrite(&SD2, buf, len);
	//uartStartSend(&UARTD2, len, buf);
}
/*
 * Working area for MavlinkHeartbeatThread
 */
static WORKING_AREA(MavlinkRCChannelsScaledThreadWorkingArea, 2048);
/*
 * MavlinkHeartbeatThread
 */

static msg_t MavlinkRCChannelsScaledThread(void *arg)
{
    while (TRUE)
    {
		send_rc_channels_scaled();
		chThdSleepMilliseconds(15);
    }
}

void send_servo_output_raw(void)
{
    // Initialize the required buffers
	mavlink_message_t msg;
	uint8_t buf[MAVLINK_MAX_PACKET_LEN];
    
    uint32_t time_boot_ms = chTimeNow();
    uint8_t port = 0; //????? Sinn?
    uint16_t servo1_raw = getMotor_1();
    uint16_t servo2_raw = getMotor_2();
    uint16_t servo3_raw = getMotor_3();
    uint16_t servo4_raw = getMotor_4();
    uint16_t servo5_raw = 0;
    uint16_t servo6_raw = 0;
    uint16_t servo7_raw = 0;
    uint16_t servo8_raw = 0;
    
    mavlink_msg_servo_output_raw_pack(mavlink_system.sysid, mavlink_system.compid, &msg, time_boot_ms, port, servo1_raw, servo2_raw, servo3_raw, servo4_raw, servo5_raw, servo6_raw, servo7_raw, servo8_raw);
    
    // Copy the message to the send buffer
	uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
    
	// Send the message with the standard UART send function
	sdWrite(&SD2, buf, len);
	//uartStartSend(&UARTD2, len, buf);
}
/*
 * Working area for MavlinkHeartbeatThread
 */
static WORKING_AREA(MavlinkServoOutputThreadWorkingArea, 2048);
/*
 * MavlinkHeartbeatThread
 */

static msg_t MavlinkServoOutputThread(void *arg)
{
    while (TRUE)
    {
		send_servo_output_raw();
		chThdSleepMilliseconds(15);
    }
}

uint16_t c_global;
static WORKING_AREA(waThreadRadio, 4096);
static msg_t ThreadRadio(void *arg) 
{
    (void)arg;
    chRegSetThreadName("radio");
	
	
	mavlink_message_t msg;
	mavlink_status_t status;

    EventListener elRadio;
    chEvtRegister(chIOGetEventSource(&SD2), &elRadio, 1);
    while (TRUE) 
			{
        ioflags_t flags;
        chEvtWaitOneTimeout(EVENT_MASK(1), TIME_INFINITE);
        flags = chIOGetAndClearFlags(&SD2);
        if (flags & IO_INPUT_AVAILABLE) 
				{
          char c;
          c = chIOGetTimeout(&SD2, TIME_IMMEDIATE);
					if(mavlink_parse_char(MAVLINK_COMM_0, c, &msg, &status))
					{
							if (msg.msgid == MAVLINK_MSG_ID_HEARTBEAT)
							{
								//Heartbeat
							}
							
							else if (msg.msgid == MAVLINK_MSG_ID_COMMAND_LONG)
							{
								// Preflight Calibration
								if (mavlink_msg_command_long_get_command(&msg) == MAV_CMD_PREFLIGHT_CALIBRATION)
								{
										if (mavlink_msg_command_long_get_param4(&msg) == 1)
										{
											palSetPad(GPIOD, GPIOD_LED4);       /* Green.  */
										}
										else if (mavlink_msg_command_long_get_param4(&msg) == 0)
										{
											palClearPad(GPIOD, GPIOD_LED4);       /* Green.  */
										}
								}
							}
							
							else if (msg.msgid == MAVLINK_MSG_ID_PARAM_REQUEST_LIST)
							{
								// Start sending parameters
								m_parameter_i = 0;
							}
							else if (msg.msgid == MAVLINK_MSG_ID_PARAM_SET)
							{
								mavlink_param_set_t set;
								mavlink_msg_param_set_decode(&msg, &set);
								{
									char* key = (char*) set.param_id;
						 
									for (uint16_t i = 0; i < ONBOARD_PARAM_COUNT; i++)
									{
										bool match = true;
										for (uint16_t j = 0; j < ONBOARD_PARAM_NAME_LENGTH; j++)
										{
											// Compare
											if (((char) (global_data.param_name[i][j]))
													!= (char) (key[j]))
											{
												match = false;
											}
										}
						 
										// Check if matched
										if (match)
										{
											{
												global_data.param[i] = set.param_value;
												
												
												// Initialize the required buffers
												mavlink_message_t msg;
												uint8_t buf[MAVLINK_MAX_PACKET_LEN];
												mavlink_msg_param_value_pack(mavlink_system.sysid, mavlink_system.compid, &msg,(char*) global_data.param_name[i], global_data.param[i], MAVLINK_TYPE_FLOAT, ONBOARD_PARAM_COUNT, m_parameter_i);
												// Copy the message to the send buffer
												uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);

												// Send the message with the standard UART send function
												sdWrite(&SD2, buf, len);
												//uartStartSend(&UARTD2, len, buf);

											}
										}
									}
								}
							}
					}
					////////////////////////////ECHO//////////////////////////////
					//uint8_t buf[1];
					//buf[0] = c;
					//uint16_t len = 1;
					//sdWrite(&SD2, buf, len);
					/////////////////////////////////////////////////////////////////
        }
    }
}



void send_statustext(uint8_t severity, const char *text)
{
	mavlink_message_t msg;
	uint8_t buf[MAVLINK_MAX_PACKET_LEN];
	mavlink_msg_statustext_pack(mavlink_system.sysid, mavlink_system.compid, &msg, severity, text);
	// Copy the message to the send buffer
	uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
    
	// Send the message with the standard UART send function
	sdWrite(&SD2, buf, len);
	//uartStartSend(&UARTD2, len, buf);
}


void setup_Mavlink(void)
{
		global_data_reset_param_defaults();
	
		/*
		* Activates the serial driver 2 using the driver default configuration.
		* PA2(TX) and PA3(RX) are routed to USART2.
		*/
		sdStart(&SD2, NULL);
		//uartStart(&UARTD2, &uart_cfg_1);
		palSetPadMode(GPIOD, 5, PAL_MODE_ALTERNATE(7));
		palSetPadMode(GPIOD, 6, PAL_MODE_ALTERNATE(7));
	
		mavlink_system.sysid = 1;                   ///< ID 1 for this airplane
		mavlink_system.compid = MAV_COMP_ID_ALL;     ///< The component sending the message is the IMU, it could be also a Linux process

		chThdCreateStatic(waThreadRadio, sizeof(waThreadRadio), ABSPRIO, ThreadRadio, NULL);

		chThdSleepMilliseconds(20);
		chThdCreateStatic(MavlinkHeartbeatThreadWorkingArea, sizeof(MavlinkHeartbeatThreadWorkingArea), NORMALPRIO, MavlinkHeartbeatThread, NULL);
		
		chThdCreateStatic(MavlinkAttitudeThreadWorkingArea, sizeof(MavlinkAttitudeThreadWorkingArea), NORMALPRIO, MavlinkAttitudeThread, NULL);
		chThdSleepMilliseconds(20);
		chThdCreateStatic(MavlinkRCChannelsScaledThreadWorkingArea, sizeof(MavlinkRCChannelsScaledThreadWorkingArea), NORMALPRIO, MavlinkRCChannelsScaledThread, NULL);
		chThdSleepMilliseconds(20);
		chThdCreateStatic(MavlinkServoOutputThreadWorkingArea, sizeof(MavlinkServoOutputThreadWorkingArea), NORMALPRIO, MavlinkServoOutputThread, NULL); 
		chThdSleepMilliseconds(20);
		chThdCreateStatic(MavlinkParamlistThreadWorkingArea, sizeof(MavlinkParamlistThreadWorkingArea), NORMALPRIO, MavlinkParamlistThread, NULL);
}



