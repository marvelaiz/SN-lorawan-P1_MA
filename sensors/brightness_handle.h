#ifndef BRIGHTNESS_SENSOR_HANDLE_H
#define BRIGHTNESS_SENSOR_HANDLE_H

#include <cstdint>
#include "mbed.h"
#include "sensor_states.h"
//#include "user_interface.h"

#define BRIGHTNESS_SENSOR_DATA_MAX_LENGHT 40
#define BRIGHTNESS_SENSOR_RESOLUTION 1  //Supply voltage value (range 0.0V - Max Value)

#define BRIGHTNESS_SENSOR_MAX_VALUE 80
#define BRIGHTNESS_SENSOR_MIN_VALUE 5

//Class
class Brightness_Sensor_handle{

public:
Brightness_Sensor_handle(AnalogIn* pin);
bool make_meassurement();
void read_meassurement();
float get_value();
uint16_t get_raw_value();
char* get_data_to_print();

bool is_measurement_out_of_range();
Measurement_Validation meassurement_state;
bool is_sensor_available;
char formated_data[BRIGHTNESS_SENSOR_DATA_MAX_LENGHT];

private:
AnalogIn* Signal_PIN;
float value;
uint16_t raw_value;
 
};


//External funtions
Brightness_Sensor_handle* Brightness_Sensor_handle_get();
void Brightness_Sensor_handle_init(AnalogIn* pin); // Create and initialize the whole interface


#endif // BRIGHTNESS_SENSOR_HANDLE_H