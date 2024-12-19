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


// Global instance
Accelerometer_handle *accelerometer_handle;


uint8_t getWhoAmI() {
    Sensors_interface *p_comm_handle = sensors_interface_get();

    uint8_t data[2] = { REG_CTRL_REG, 0x01 };

    // Write the data array to the accelerometer
    i2c.write(MMA8451_I2C_ADDRESS, (char *)data, 2);
    uint8_t who_am_i[1];
    char cmd;

    cmd = (char)REG_WHO_AM_I;

    
    // Read the WHO_AM_I register value (unique identifier for MMA8451)
    p_comm_handle->_p_i2c_interface->.write(ACCEL_I2C_ADDR, &cmd, 1, true); // true = keep the connection open
    // Read the specified number of bytes from the accelerometer into data buffer
    p_comm_handle->_p_i2c_interface->.read(ACCEL_I2C_ADDR, (char*)who_am_i, 1);
    //readRegs(REG_WHO_AM_I, &who_am_i, 1);
    return *who_am_i;
}

// Function to initialize the MMA8451 accelerometer and check if it’s connected
bool initMMA8451() {
  Sensors_interface *p_comm_handle = sensors_interface_get();
  char cmd[2];
  volatile char who_am_i = 0;
  char data[1] = {0};
//    char data = 0x01; // Command to set accelerometer to active mode

  // Send command to activate the accelerometer
  cmd[0]=REG_CTRL_REG;
  cmd[1]=0x01;
int valid=p_comm_handle->_p_i2c_interface->write(ACCEL_I2C_ADDR, cmd, 2);

//Test the device
// cmd[0]=REG_WHO_AM_I;
//   p_comm_handle->_p_i2c_interface->write(ACCEL_ADDR << 1, cmd, 1, true);
//   p_comm_handle->_p_i2c_interface->read(ACCEL_ADDR << 1, data, 1, false);
  // Read the WHO_AM_I register value (unique identifier for MMA8451)



  if (valid==0) {
    // return who_am_i == 0x1A; // 0x1A is the device ID for MMA8451
    return true;
  }
  return false;
}

// Class methods

// Constructor
Accelerometer_handle::Accelerometer_handle(uint8_t address)
    : address(address), is_sensor_available(false), acc_x_value(0),
      acc_y_value(0), acc_z_value(0) {}

// Format accelerometer data for display
char *Accelerometer_handle::get_data_to_print() {
  snprintf(formated_data, sizeof(formated_data),
           "ACC: X: %.5f Y: %.5f Z: %.5f \n", acc_x_value_m_s, acc_y_value_m_s,
           acc_z_value_m_s);
  return formated_data;
}

// Checks if the sensor is available by performing a measurement
bool Accelerometer_handle::make_meassurement() {
  Sensors_interface *p_comm_handle = sensors_interface_get();

  char data = 0x01; // Command to set accelerometer to active mode

  // Send command to activate the accelerometer
   char cmd[2];
  
//   cmd[0]=REG_CTRL_REG;
//   cmd[1]=0x01;
// p_comm_handle->_p_i2c_interface->write(ACCEL_I2C_ADDR, cmd, 2);

  


  if (p_comm_handle->_p_i2c_interface->write(ACCEL_I2C_ADDR, cmd, 2) == 0) {
    // is_sensor_available = true;

    cmd[0]=MMA8451_XYZ_DATA_CFG;
  cmd[1]=0x00;
  p_comm_handle->_p_i2c_interface->write(ACCEL_I2C_ADDR, cmd, 2);
    return true;

  }
  // is_sensor_available = false;
  return false;
}

// Read raw data from the accelerometer for each axis
void Accelerometer_handle::read_meassurement() {
  Sensors_interface *p_comm_handle = sensors_interface_get();
  char data[6];
  char cmd;

  // Read X-axis data
  cmd = REG_OUT_X_MSB;
  p_comm_handle->_p_i2c_interface->write(ACCEL_I2C_ADDR, &cmd, 1, true);
  p_comm_handle->_p_i2c_interface->read(ACCEL_I2C_ADDR, data, 6);
   raw_x_value = (data[0] << 8) | (data[1] );
  raw_y_value = (data[2] << 8) | (data[3] );
  raw_z_value = (data[4] << 8) | (data[5] );


  if (raw_x_value & 0x8000) {
        acc_x_value = -(~(raw_x_value & 0x7FFF));
    }

if (raw_y_value & 0x8000) {
        acc_y_value = -(~raw_y_value & 0x7FFF);
    }

    if (raw_z_value & 0x8000) {
        acc_z_value = -(~raw_z_value & 0x7FFF);
    }
    // ... similar for y_raw and z_raw

    // Convert to g-force (assuming 2g full-scale range)
    acc_x_value = acc_x_value * 2.0f / 4096.0f;
    acc_y_value = acc_y_value * 2.0f / 4096.0f;
    acc_z_value = acc_z_value * 2.0f / 4096.0f;

  acc_x_value_m_s = acc_x_value * G_COVERSION_FACTOR;
  acc_y_value_m_s = acc_y_value * G_COVERSION_FACTOR;
  acc_z_value_m_s = acc_z_value * G_COVERSION_FACTOR;



}

// Get individual axis values
float Accelerometer_handle::get_acc_x_value() { return acc_x_value; }
float Accelerometer_handle::get_acc_y_value() { return acc_y_value; }
float Accelerometer_handle::get_acc_z_value() { return acc_z_value; }
uint16_t Accelerometer_handle::get_raw_x_value(){return raw_x_value;}
uint16_t Accelerometer_handle::get_raw_y_value(){return raw_y_value;}
uint16_t Accelerometer_handle::get_raw_z_value(){return raw_z_value;}



bool Accelerometer_handle::is_measurement_out_of_range() {
  if (acc_x_value_m_s > ACCEL_X_MAX_VALUE_M_S2 ||
      acc_x_value_m_s < ACCEL_X_MIN_VALUE_M_S2 ||
      acc_y_value_m_s > ACCEL_Y_MAX_VALUE_M_S2 ||
      acc_y_value_m_s < ACCEL_Y_MIN_VALUE_M_S2 ||
      acc_z_value_m_s > ACCEL_Z_MAX_VALUE_M_S2 ||
      acc_z_value_m_s < ACCEL_Z_MIN_VALUE_M_S2) {
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
