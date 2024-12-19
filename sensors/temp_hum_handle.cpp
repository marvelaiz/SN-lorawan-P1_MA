// ===========================================================================
// Temp_Hum_handle.cpp
// ---------------------------------------------------------------------------
// Description:
// Implementation of the Temp_Hum_handle class.
// Author: Mariana Arvelaiz
// Date: Nov. 2024
// Version: 1.0
// ===========================================================================

#include "temp_hum_handle.h"
#include "app.h"
#include "sensors_handle.h"
#include "user_interface.h"
#include <cstdio>
#include <cstring>

Temp_Hum_handle *temp_hum_handle;



bool initSi7021() {
  Sensors_interface *p_comm_handle = sensors_interface_get();
  char cmd = 0xE3; // Command to read temperature from the Si7021.

  // Send the command to the sensor; if successful, mark it as available.
  if (p_comm_handle->_p_i2c_interface->write(0x40 << 1, &cmd, 1) == 0) {
    return true; // Initialization successful.
  }

  return false; // Initialization failed (sensor not available).
}

// Class methods

Temp_Hum_handle::Temp_Hum_handle(uint8_t address)
    : address(address), is_sensor_available(false),
      temp_meassurement_state(MEA_OK), hum_meassurement_state(MEA_OK),
      first_meassurement(true), min_temp_value(0), max_temp_value(0),
      mean_temp_value(0), min_hum_value(0), max_hum_value(0),
      mean_hum_value(0) {}

char *Temp_Hum_handle::get_data_to_print() {
  // This funtion configure the format in which the sensors reading is going to
  // be printed
  snprintf(formated_data, sizeof(formated_data),
           "TEMP/HUM: Temp: %.2f°C   Rel. Hum: %.2f . \n\n",
           temp_hum_handle->temp_value, temp_hum_handle->hum_value);
  return formated_data;
}



bool Temp_Hum_handle::make_meassurement() {

  Sensors_interface *p_comm_handle = sensors_interface_get();
  char cmd = 0xE3; // Command to read temperature from the Si7021.
  if (p_comm_handle->_p_i2c_interface->write(TEMP_HUM_ADDR << 1, &cmd, 1) ==
      0) {
    temp_hum_handle->is_sensor_available = true; // Sensor is available.
    return true;
  }
  temp_hum_handle->is_sensor_available = false;
  return false;

  
}

void Temp_Hum_handle::read_meassurement() {
  App *app = app_get();
  Sensors_interface *p_comm_handle = sensors_interface_get();
  char cmd = 0xE3; // Command to read temperature.
  char data[2];    // Array to hold the received data.

  // Send the command to the sensor to request temperature data.
  p_comm_handle->_p_i2c_interface->write(TEMP_HUM_ADDR << 1, &cmd, 1);
  // Read two bytes of data from the sensor.
  p_comm_handle->_p_i2c_interface->read(TEMP_HUM_ADDR << 1, data, 2);

  // Combine the two bytes into a single 16-bit value.
   rawTemperature = (data[0] << 8) | data[1];
  // Convert the raw temperature value to degrees Celsius.
  temp_value = ((175.72 * rawTemperature) / 65536.0) - 46.85;

  // Humidity

  cmd = 0xE5; // Command to read humidity.

  // Send the command to the sensor to request humidity data.
  p_comm_handle->_p_i2c_interface->write(0x40 << 1, &cmd, 1);
  // Read two bytes of data from the sensor.
  p_comm_handle->_p_i2c_interface->read(0x40 << 1, data, 2);

  // Combine the two bytes into a single 16-bit value.
  rawHumidity = (data[0] << 8) | (data[1]);
  // Convert the raw humidity value to a percentage.
  hum_value = ((125.0f * rawHumidity) / 65536.0) - 6.0;

  if (first_meassurement) {
    first_meassurement = false;
    max_hum_value = hum_value;
    min_hum_value = hum_value;
    max_temp_value = temp_value;
    min_temp_value = temp_value;
  }

  if (app->get_mode() == NORMAL_MODE ||app->get_mode() == ADVANCE_MODE) {
    if (p_comm_handle->counter == 0) {
    //   reset_min_max_mean();
    } else {
      // Update min, max and meaen values for temp
      min_temp_value = min(min_temp_value, temp_value);
      max_temp_value = max(max_temp_value, temp_value);
      mean_temp_value =
          mean_temp_value + (temp_value - mean_temp_value) / p_comm_handle->counter;
      // Update min, max and meaen values for temp
      min_hum_value = min(min_hum_value, hum_value);
      max_hum_value = max(max_hum_value, hum_value);
      mean_hum_value =
          mean_hum_value + (hum_value - mean_hum_value) / p_comm_handle->counter;

      
      
    }
  }
}
float Temp_Hum_handle::get_temp_value() { return temp_value; }
float Temp_Hum_handle::get_hum_value() { return hum_value; }

bool Temp_Hum_handle::is_temp_measurement_out_of_range() {

  if (temp_value > TEMP_MAX_VALUE || temp_value < TEMP_MIN_VALUE) {
    temp_meassurement_state = MEA_OUT_RANGE;
    return true;
  } else {
    temp_meassurement_state = MEA_OK;
    return false;
  }
}
bool Temp_Hum_handle::is_hum_measurement_out_of_range() {
  if (hum_value > HUM_MAX_VALUE || hum_value < HUM_MIN_VALUE) {
    hum_meassurement_state = MEA_OUT_RANGE;
    return true;
  } else {
    hum_meassurement_state = MEA_OK;
    return false;
  }
}

// External Funtions

void Temp_Hum_handle::power_up(){
    //sensor_vcc.write(1);

}
void Temp_Hum_handle::power_off(){
   //sensor_vcc.write(0);

}

int16_t Temp_Hum_handle::get_raw_temp_value(){
    return rawTemperature;
}
int16_t Temp_Hum_handle::get_raw_hum_value(){
    return rawHumidity;

}

Temp_Hum_handle *Temp_Hum_handle_get() { return temp_hum_handle; }

void Temp_Hum_handle_init(uint8_t address) {
  // Allocate the pointer
  temp_hum_handle = new Temp_Hum_handle(address);
  temp_hum_handle->power_up();
  
  memset(temp_hum_handle->formated_data, '\0',
         sizeof(temp_hum_handle->formated_data));

  if (initSi7021()) {

    snprintf(temp_hum_handle->formated_data,
             sizeof(temp_hum_handle->formated_data),
             "Si7021 sensor detected.\n");
    temp_hum_handle->is_sensor_available = true;
  } else {
    snprintf(temp_hum_handle->formated_data,
             sizeof(temp_hum_handle->formated_data),
             "Si7021 sensor not detected.\n");
    temp_hum_handle->is_sensor_available = false;
  }
  user_interface_handle *p_handle = user_interface_get();
  p_handle->print_msg(temp_hum_handle->formated_data);

  // Code to configure the sensor for first measurement if needed

  // Sensor init

  // Set up the timeout to make the meassurement
}