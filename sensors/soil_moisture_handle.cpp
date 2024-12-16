// ===========================================================================
// Temp_Hum_handle.cpp
// ---------------------------------------------------------------------------
// Description:
// Implementation of the Temp_Hum_handle class.
// Author: Mariana Arvelaiz
// Date: Nov. 2024
// Version: 1.0
// ===========================================================================
#include "soil_moisture_handle.h"
#include "sensors_handle.h"
#include "user_interface.h"
#include <cstdio>
#include <cstring>
#include "app.h"


Soil_Moisture_handle *soil_moisture_handle;

bool initSoil_Moisture() {
  // Sensors_interface *p_comm_handle = sensors_interface_get();
  // char cmd = 0xE3; // Command to read temperature from the Si7021.

  // // Send the command to the sensor; if successful, mark it as available.
  // if (p_comm_handle->_p_i2c_interface->write(0x40 << 1, &cmd, 1) == 0) {
  //     return true; // Initialization successful.
  // }

  return true; // Initialization failed (sensor not available).
}

// Class methods

Soil_Moisture_handle::Soil_Moisture_handle(AnalogIn pin)
    : Signal_PIN(pin), is_sensor_available(false) {}

char *Soil_Moisture_handle::get_data_to_print() {
  // This funtion configure the format in which the sensors reading is going to
  // be printed
  snprintf(formated_data, sizeof(formated_data),
           "Soil Moisture Level: %.2f%%\n", soil_moisture_handle->value);
  return formated_data;
}

bool Soil_Moisture_handle::make_meassurement() {

  //   Sensors_interface *p_comm_handle = sensors_interface_get();
  //   char cmd = 0xE3; // Command to read temperature from the Si7021.
  //   if (p_comm_handle->_p_i2c_interface->write(TEMP_HUM_ADDR << 1, &cmd, 1)
  //   ==
  //       0) {
  //     temp_hum_handle->is_sensor_available = true; // Sensor is available.
  //     return true;
  //   }
  return false;
}

void Soil_Moisture_handle::read_meassurement() {

  Sensors_interface *p_comm_handle = sensors_interface_get();
  float sensorReading = Signal_PIN.read();
  value = (sensorReading * 100) / SOIL_MOISTURE_RESOLUTION;

  if (value > 100) {
    value = 100;
  } else if (value < 0) {
    value = 0;
  }
  App *app=app_get();

  if (app->get_mode() == NORMAL_MODE ||app->get_mode() == ADVANCE_MODE) {
    if (p_comm_handle->counter == 0) {
      reset_min_max_mean();
    } else {
      // Update min, max and meaen values for temp
      // Update min and max values
            min_value = min(min_value, value);
            max_value = max(max_value, value);
            mean_value = mean_value + (value - mean_value) / p_comm_handle->counter; 

    }
  }
}
float Soil_Moisture_handle::get_soil_moisture_value() { return value; }

bool Soil_Moisture_handle::is_measurement_out_of_range() {
  if (value > SOIL_MOISTURE_MAX_VALUE || value < SOIL_MOISTURE_MIN_VALUE) {
    meassurement_state = MEA_OUT_RANGE;
    return true;
  } else {
    meassurement_state = MEA_OK;
    return false;
  }
}
void Soil_Moisture_handle::reset_min_max_mean() {
    min_value = value;
    max_value = value;
    mean_value = 0.0;
}
float Soil_Moisture_handle :: get_min_value() { return min_value; }
float Soil_Moisture_handle :: get_max_value() { return max_value; }
float Soil_Moisture_handle :: get_mean_value() { return mean_value; }

// External Funtions

Soil_Moisture_handle *Soil_Moisture_handle_get() {
  return soil_moisture_handle;
}

void Soil_Moisture_handle_init(AnalogIn pin) {
  // Allocate the pointer
  soil_moisture_handle = new Soil_Moisture_handle(pin);
  memset(soil_moisture_handle->formated_data, '\0',
         sizeof(soil_moisture_handle->formated_data));

  if (initSoil_Moisture()) {

    snprintf(soil_moisture_handle->formated_data,
             sizeof(soil_moisture_handle->formated_data),
             "Soil Moisture sensor detected.\n");

    soil_moisture_handle->is_sensor_available = true;
  } else {
    snprintf(soil_moisture_handle->formated_data,
             sizeof(soil_moisture_handle->formated_data),
             "Soil Moisture sensor not detected.\n");

    soil_moisture_handle->is_sensor_available = false;
  }
  user_interface_handle *p_handle = user_interface_get();
  p_handle->print_msg(soil_moisture_handle->formated_data);

  // Code to configure the sensor for first measurement if needed

  // Sensor init

  // Set up the timeout to make the meassurement
}