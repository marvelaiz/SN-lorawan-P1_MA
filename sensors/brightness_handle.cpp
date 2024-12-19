// ===========================================================================
// Temp_Hum_handle.cpp
// ---------------------------------------------------------------------------
// Description:
// Implementation of the Temp_Hum_handle class.
// Author: Mariana Arvelaiz
// Date: Nov. 2024
// Version: 1.0
// ===========================================================================
#include "brightness_handle.h"
#include "user_interface.h"
#include "sensors_handle.h"
#include <cstdint>
#include <cstdio>
#include <cstring>

Brightness_Sensor_handle *brightness_handle;

bool initBrightness() {
    // Sensors_interface *p_comm_handle = sensors_interface_get();
    // char cmd = 0xE3; // Command to read temperature from the Si7021.
    
    // // Send the command to the sensor; if successful, mark it as available.
    // if (p_comm_handle->_p_i2c_interface->write(0x40 << 1, &cmd, 1) == 0) {
    //     return true; // Initialization successful.
    // }
    
    return true; // Initialization failed (sensor not available).
}


// Class methods

Brightness_Sensor_handle::Brightness_Sensor_handle(AnalogIn* pin)
    : Signal_PIN(pin), is_sensor_available(false) {}

char *Brightness_Sensor_handle::get_data_to_print() {
  // This funtion configure the format in which the sensors reading is going to
  // be printed
  snprintf(formated_data, sizeof(formated_data), "Brightness Level: %.2f%%\n",
           brightness_handle->value);
  return formated_data;
}

bool Brightness_Sensor_handle::make_meassurement() {

//   Sensors_interface *p_comm_handle = sensors_interface_get();
//   char cmd = 0xE3; // Command to read temperature from the Si7021.
//   if (p_comm_handle->_p_i2c_interface->write(TEMP_HUM_ADDR << 1, &cmd, 1) ==
//       0) {
//     temp_hum_handle->is_sensor_available = true; // Sensor is available.
//     return true;
//   }
  return false;
}

uint16_t Brightness_Sensor_handle::get_raw_value(){
    return raw_value;
}

void Brightness_Sensor_handle::read_meassurement() {

    Sensors_interface *p_comm_handle = sensors_interface_get();
    float sensorReading = Signal_PIN->read();
    value = (sensorReading*100)/BRIGHTNESS_SENSOR_RESOLUTION;
    volatile float scaled= value*100;

    raw_value= (uint16_t)scaled;

    

    if (value > 100) {
        value = 100;
    } else if (value < 0) {
        value = 0;
    }
}
float Brightness_Sensor_handle::get_value() { return value; }

bool Brightness_Sensor_handle::is_measurement_out_of_range(){
    if (value > BRIGHTNESS_SENSOR_MAX_VALUE || value < BRIGHTNESS_SENSOR_MIN_VALUE) {
    meassurement_state = MEA_OUT_RANGE;
    return true;
  } else {
    meassurement_state = MEA_OK;
    return false;
  }
}

// External Funtions

Brightness_Sensor_handle* Brightness_Sensor_handle_get(){
    return brightness_handle;
}


void Brightness_Sensor_handle_init(AnalogIn* pin) {
  // Allocate the pointer
  brightness_handle = new Brightness_Sensor_handle(pin);
  memset(brightness_handle->formated_data, '\0',
         sizeof(brightness_handle->formated_data));

    if (initBrightness()) {

        snprintf(brightness_handle->formated_data, sizeof(brightness_handle->formated_data), "Brightness sensor detected.\n");

        brightness_handle->is_sensor_available=true;
    } else {
        snprintf(brightness_handle->formated_data, sizeof(brightness_handle->formated_data), "Brightness sensor not detected.\n");
        
        brightness_handle->is_sensor_available=false;
    }
    user_interface_handle *p_handle = user_interface_get();
    p_handle->print_msg(brightness_handle->formated_data);

  // Code to configure the sensor for first measurement if needed

  // Sensor init

  // Set up the timeout to make the meassurement
}