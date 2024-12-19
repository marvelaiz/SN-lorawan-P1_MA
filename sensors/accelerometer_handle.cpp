// ===========================================================================
// accel-handle.cpp
// ---------------------------------------------------------------------------
// Description:
// Implementation of the Accelerometer_handle class for interfacing with the
// Adafruit MMA8451 accelerometer via I2C.
// Author: Borzou Ghorbani
// Date: Nov. 2024
// Version: 1.0
// ===========================================================================

#include "accelerometer_handle.h"
#include "sensors_handle.h"
#include "user_interface.h"
#include <cstdio>
#include <cstring>
#include "app.h"

#define UINT14_MAX     16383  // Maximum value for 14-bit unsigned integer (2^14 - 1)
// Global instance
Accelerometer_handle *accelerometer_handle;


// uint8_t getWhoAmI() {
//     Sensors_interface *p_comm_handle = sensors_interface_get();

//     // uint8_t data[2] = { REG_CTRL_REG, 0x01 };

//     // // Write the data array to the accelerometer
//     // i2c.write(ACCEL_I2C_ADDR, (char *)data, 2);
    
// }

// Function to initialize the MMA8451 accelerometer and check if it’s connected
bool initMMA8451() {
  Sensors_interface *p_comm_handle = sensors_interface_get();
  // REG_CTRL_REG1 with value 0x01 sets the device to active mode for continuous data reading
  uint8_t data[2] = { REG_CTRL_REG, 0x01 };

    // Write the data array to the accelerometer
    volatile bool valid=p_comm_handle->_p_i2c_interface->write(ACCEL_I2C_ADDR, (char *)data, 2);
//    char data = 0x01; // Command to set accelerometer to active mode

uint8_t who_am_i[1];
    char cmd;

    cmd = (char)REG_WHO_AM_I;

    
    // Read the WHO_AM_I register value (unique identifier for MMA8451)
    p_comm_handle->_p_i2c_interface->write(ACCEL_I2C_ADDR, &cmd, 1, true); // true = keep the connection open
    // Read the specified number of bytes from the accelerometer into data buffer
    p_comm_handle->_p_i2c_interface->read(ACCEL_I2C_ADDR, (char*)who_am_i, 1);
    //readRegs(REG_WHO_AM_I, &who_am_i, 1);
    

//   if (who_am_i[0]==26) {
    // return who_am_i == 0x1A; // 0x1A is the device ID for MMA8451
    return true;
//   }
//   return false;
}

// Class methods

// Constructor
Accelerometer_handle::Accelerometer_handle(uint8_t address)
    : address(address), is_sensor_available(false), acc_x_value(0),
      acc_y_value(0), acc_z_value(0) {}

// Format accelerometer data for display
char *Accelerometer_handle::get_data_to_print() {
  snprintf(formated_data, sizeof(formated_data),
           "ACC: X: %.5f Y: %.5f Z: %.5f \n", acc_x_value, acc_y_value,
           acc_z_value);
  return formated_data;
}

// Checks if the sensor is available by performing a measurement
bool Accelerometer_handle::make_meassurement() {
  
    return true;

}

// Read raw data from the accelerometer for each axis
void Accelerometer_handle::read_meassurement() {
  Sensors_interface *p_comm_handle = sensors_interface_get();
  uint8_t data[6];
  char cmd;

  cmd = (char)REG_OUT_X_MSB;
  p_comm_handle->_p_i2c_interface->write(ACCEL_I2C_ADDR, &cmd, 1, true);
  p_comm_handle->_p_i2c_interface->read(ACCEL_I2C_ADDR, (char*)data, 2);
  int16_t raw_x_value = (data[0] << 6) | (data[1] >>2);
  cmd = (char)REG_OUT_Y_MSB;
  p_comm_handle->_p_i2c_interface->write(ACCEL_I2C_ADDR, &cmd, 1, true);
  p_comm_handle->_p_i2c_interface->read(ACCEL_I2C_ADDR, (char*)data, 2);
  int16_t raw_y_value = (data[0] << 6) | (data[1] >>2);
  cmd = (char)REG_OUT_Z_MSB;
  p_comm_handle->_p_i2c_interface->write(ACCEL_I2C_ADDR, &cmd, 1, true);
  p_comm_handle->_p_i2c_interface->read(ACCEL_I2C_ADDR, (char*)data, 2);
  int16_t raw_z_value = (data[0] << 6) | (data[1] >>2);

   if (raw_x_value > UINT14_MAX / 2) raw_x_value -= UINT14_MAX; // Convert to signed by adjusting for overflow
   if (raw_y_value > UINT14_MAX / 2) raw_y_value -= UINT14_MAX; // Convert to signed by adjusting for overflow
   if (raw_z_value > UINT14_MAX / 2) raw_z_value -= UINT14_MAX; // Convert to signed by adjusting for overflow

//Obtain the acceleration in units of g
   acc_x_value=float(raw_x_value) / 4096.0;
   acc_y_value=float(raw_y_value) / 4096.0;
   acc_z_value=float(raw_z_value) / 4096.0;

//Transform the acceleration in units of m/s^2
    acc_x_value=acc_x_value*G_COVERSION_FACTOR;
    acc_y_value=acc_y_value*G_COVERSION_FACTOR;
    acc_z_value=acc_z_value*G_COVERSION_FACTOR;


}

// Get individual axis values
float Accelerometer_handle::get_acc_x_value() { return acc_x_value; }
float Accelerometer_handle::get_acc_y_value() { return acc_y_value; }
float Accelerometer_handle::get_acc_z_value() { return acc_z_value; }
int16_t Accelerometer_handle::get_raw_x_value(){return raw_x_value;}
int16_t Accelerometer_handle::get_raw_y_value(){return raw_y_value;}
int16_t Accelerometer_handle::get_raw_z_value(){return raw_z_value;}



bool Accelerometer_handle::is_measurement_out_of_range() {
  if (acc_x_value > ACCEL_X_MAX_VALUE_M_S2 ||
      acc_x_value < ACCEL_X_MIN_VALUE_M_S2 ||
      acc_y_value > ACCEL_Y_MAX_VALUE_M_S2 ||
      acc_y_value < ACCEL_Y_MIN_VALUE_M_S2 ||
      acc_z_value > ACCEL_Z_MAX_VALUE_M_S2 ||
      acc_z_value < ACCEL_Z_MIN_VALUE_M_S2) {
    meassurement_state = MEA_OUT_RANGE;
    return true;
  } else {
    meassurement_state = MEA_OK;
    return false;
  }
}

// External functions

// Returns a pointer to the global accelerometer handle instance
Accelerometer_handle *Accelerometer_handle_get() {
  return accelerometer_handle;
}

// Initialize the accelerometer handle
void Accelerometer_handle_init(uint8_t address) {
  // Allocate memory for the accelerometer handle
  accelerometer_handle = new Accelerometer_handle(address);
  memset(accelerometer_handle->formated_data, '\0',
         sizeof(accelerometer_handle->formated_data));

  // Check if sensor is detected
  if (initMMA8451()) {
    snprintf(accelerometer_handle->formated_data,
             sizeof(accelerometer_handle->formated_data),
             "MMA8451 sensor detected.\n");
    accelerometer_handle->is_sensor_available = true;
  } else {
    snprintf(accelerometer_handle->formated_data,
             sizeof(accelerometer_handle->formated_data),
             "MMA8451 sensor not detected.\n");
    accelerometer_handle->is_sensor_available = false;
  }

  // Print sensor status to the user interface
  user_interface_handle *p_handle = user_interface_get();
  p_handle->print_msg(accelerometer_handle->formated_data);
}
