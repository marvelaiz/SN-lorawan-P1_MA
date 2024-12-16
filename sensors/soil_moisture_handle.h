#ifndef SOIL_MOISTURE_HANDLE_H
#define SOIL_MOISTURE_HANDLE_H

#include <cstdint>
#include "mbed.h"
#include "sensor_states.h"
//#include "user_interface.h"

#define SOIL_MOISTURE_DATA_MAX_LENGHT 40
#define SOIL_MOISTURE_RESOLUTION 3.3  //Supply voltage value (range 0.0V - Max Value)

#define SOIL_MOISTURE_MAX_VALUE 80
#define SOIL_MOISTURE_MIN_VALUE 10

//Class
class Soil_Moisture_handle{

public:
Soil_Moisture_handle(AnalogIn pin);
bool make_meassurement();
void read_meassurement();
float get_soil_moisture_value();
char* get_data_to_print();
void  reset_min_max_mean();
float get_min_value();
float get_max_value();
float get_mean_value();

bool is_measurement_out_of_range();
Measurement_Validation meassurement_state;

bool is_sensor_available;
char formated_data[SOIL_MOISTURE_DATA_MAX_LENGHT];

private:
AnalogIn Signal_PIN;
float value;
float min_value;
float max_value;
float mean_value;

};


//External funtions
Soil_Moisture_handle* Soil_Moisture_handle_get();
void Soil_Moisture_handle_init(AnalogIn pin); // Create and initialize the whole interface


#endif // SOIL_MOISTURE_HANDLE_H