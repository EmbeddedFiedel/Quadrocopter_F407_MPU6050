#include "ch.h"
#include "hal.h"
#include "mavlink.h"

void setup_Mavlink(void);
void send_statustext(uint8_t severity, const char *text);