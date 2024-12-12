// ===========================================================================
// Temp_Hum_handle.h
// ---------------------------------------------------------------------------
// Description:
//   Header file for the Temp_Hum_handle class, which provides an interface for
//   reading temperature and humidity values from a Si7021 sensor with I2C communication interface
//
// Author: Mariana Arvelaiz 
// Date: Nov. 2024
// Version: 1.0
// ===========================================================================

#ifndef TEMP_HUM_HANDLE_H
#define TEMP_HUM_HANDLE_H

#include <cstdint>
#include "sensor_states.h"
#include "mbed.h"

//MACROS
#define TEMP_HUM_ADDR 0x40
#define TEMP_HUM_DATA_MAX_LENGHT 100
#define TEMP_MAX_VALUE 50
#define TEMP_MIN_VALUE -10
#define HUM_MAX_VALUE 75
#define HUM_MIN_VALUE 25

//IMPORTANT CMD OF THE SENSOR 
//(missing)Here we need to create a macros with the cmds of the sensor used and NOT to use them directly inside the funtions 



//Class
class Temp_Hum_handle{

public:
Temp_Hum_handle(uint8_t address); // contructor with sensor address
bool make_meassurement();
void read_meassurement();
float get_temp_value();
float get_hum_value();
char* get_data_to_print();
char *get_stats_to_print(char *buffer);
bool is_temp_measurement_out_of_range();
bool is_hum_measurement_out_of_range();
void power_up();
void power_off();
//DigitalOut sensor_vcc;

Measurement_Validation temp_meassurement_state;
Measurement_Validation hum_meassurement_state;

bool is_sensor_available;
char formated_data[TEMP_HUM_DATA_MAX_LENGHT];
void  reset_min_max_mean();
float get_min_temp_value();
float get_max_temp_value();
float get_mean_temp_value();
float get_min_hum_value();
float get_max_hum_value();
float get_mean_hum_value();

private:
uint8_t address;
float temp_value;
float hum_value;
bool first_meassurement;
float min_temp_value;
float max_temp_value;
float mean_temp_value;
float min_hum_value;
float max_hum_value;
float mean_hum_value;

};


//External funtions
Temp_Hum_handle* Temp_Hum_handle_get();
void Temp_Hum_handle_init(uint8_t address); // Create and initialize the whole interface


#endif // TEMP_HUM_HANDLE_H