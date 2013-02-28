#include "ch.h"
#include "hal.h"

bool setup_barometer();
float baro_get_temperatur();
float baro_get_altitude();
float baro_get_pressure();
float baro_get_standardized_pressure();
bool test_connect();
void baro_para_altitude(float altitude);
void baro_read_temperatur();
void baro_read_all();
