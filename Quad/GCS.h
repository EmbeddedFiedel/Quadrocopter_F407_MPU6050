#include "ch.h"
#include "hal.h"
#include "mavlink.h"
					  

#define ONBOARD_PARAM_COUNT 42
#define ONBOARD_PARAM_NAME_LENGTH 15
					  
struct global_struct
{
	float param[ONBOARD_PARAM_COUNT];
	char param_name[ONBOARD_PARAM_COUNT][MAVLINK_MSG_PARAM_SET_FIELD_PARAM_ID_LEN];
};
 

void setup_Mavlink(void);
void send_statustext(uint8_t severity, const char *text);
