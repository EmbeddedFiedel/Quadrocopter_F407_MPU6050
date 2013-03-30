#include "I2Cdev.h"
#include "BMP085.h"
#include "barometer.h"
#include "Lage.h"

#include <math.h>

BMP085 barometer;
 
float temperatur=0;
float pressure=0;
float pressure_0=101325.0; //Pa sea level pressure standart
systime_t delta_t_baro;





//test the conncetion. returns true if barometer was successful initalized.
bool test_connect(){
return barometer.testConnection();
}

//read the temperatur. to get the value use baro_get_temperatur (5ms delay)
void baro_read_temperatur(){
    // request temperature
    barometer.setControl(BMP085_MODE_TEMPERATURE);
		// wait appropriate time for conversion (4.5ms delay)
		chThdSleepMilliseconds(barometer.getMeasureDelayMilliseconds());
		temperatur =barometer.getTemperatureC();
}

//read the pressure in Pa. (3x oversampling mode, high detail, 26ms delay)
void baro_read_pressure(){
    // request pressure (3x oversampling mode, high detail, 26ms delay)
    barometer.setControl(BMP085_MODE_PRESSURE_3);
		chThdSleepMilliseconds(barometer.getMeasureDelayMilliseconds());
	   // read pressure value in Pascals (Pa)
    pressure = barometer.getPressure();
	//return;

}

//additional function if you want to calibrate the altitude
void baro_para_altitude(float set_altitude){
	//baro_read_all, because of temperatur compensation 
	baro_read_all();
	pressure_0=pressure/pow(1-(set_altitude/44330),5.255f);
	
}
//reads pressure, temperatur, altitude. (28ms delay)
void baro_read_all(){
	baro_read_temperatur();
	baro_read_pressure();
	
}

float baro_get_temperatur(){
	return temperatur;
}
float baro_get_pressure(){
	return pressure;
}
//returns the standardized pressure for weather calculation. based on a reduction formula of the "Deutschem Wetterdienst" see 
//http://de.wikipedia.org/wiki/Barometrische_H%C3%B6henformel#Reduktion_auf_Meeresh.C3.B6he
float baro_get_standardized_pressure(){
	float altitude;
	altitude=baro_get_altitude();
	return pressure*exp((9.80665*altitude)/(287.05*(273.15+temperatur+(0.12*18.2193*(1.0463-exp(-0.0666*temperatur)))+((0.0065*altitude)/2))));
}

//returns the calculated altitude. don't forget to calibrate the altitude!
float baro_get_altitude(){
    return barometer.getAltitude(pressure,pressure_0);
}

static WORKING_AREA(BarometerReadThreadWorkingArea, 2048);

static msg_t BarometerReadThread(void *arg) {
		static systime_t delta_t_baro;
		systime_t time;
    // request pressure (3x oversampling mode, high detail, 26ms delay)
    barometer.setControl(BMP085_MODE_PRESSURE_3);
		chThdSleepMilliseconds(barometer.getMeasureDelayMilliseconds());
		time = chTimeNow();     // Tnow
		while(true){
				delta_t_baro = MS2ST(31);
				time += delta_t_baro;            // Next deadline
				if(get_updateing_imu()==0){
				pressure = barometer.getPressure();
				}
				if(chTimeNow() < time) chThdSleepUntil(time);
			}
}
//init barometer. returns true if barometer was successful initialized.
bool setup_barometer(){	
	 barometer.initialize();
	 baro_para_altitude(310);
	 baro_read_all();
	 chThdCreateStatic(BarometerReadThreadWorkingArea, sizeof(BarometerReadThreadWorkingArea), HIGHPRIO, BarometerReadThread, NULL);
	
	 return test_connect();
}