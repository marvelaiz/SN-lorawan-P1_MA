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

// void readRegs(int addr, uint8_t *data, int len) {
//     // Write the address we want to read from
//     char t[1] = { (char)addr };
//     i2c.write(MMA8451_I2C_ADDRESS, t, 1, true); // true = keep the connection
//     open

//     // Read the specified number of bytes from the accelerometer into data
//     buffer i2c.read(MMA8451_I2C_ADDRESS, (char *)data, len);
// }

// // Function to write data to a register (e.g., for configuration)
// void writeRegs(uint8_t *data, int len) {
//     // Write the data array to the accelerometer
//     i2c.write(MMA8451_I2C_ADDRESS, (char *)data, len);
// }

// Function to get the value of the WHO_AM_I register, which identifies the
// sensor char getWhoAmI() {
//     char who_am_i = 0;
//     // Read the WHO_AM_I register value (unique identifier for MMA8451)
//     char reg[1] = { (char)REG_WHO_AM_I };
//     Sensors_interface *p_comm_handle = sensors_interface_get();
//     p_comm_handle->_p_i2c_interface->write(ACCEL_I2C_ADDR, reg, 1, true); //
//     true = keep the connection open

//     // Read the specified number of bytes from the accelerometer into data
//     buffer p_comm_handle->_p_i2c_interface->read(ACCEL_I2C_ADDR, &who_am_i,
//     1);

//     return who_am_i;
// }
// Function to initialize the MMA8451 accelerometer and check if it’s connected
bool initMMA8451() {
  Sensors_interface *p_comm_handle = sensors_interface_get();
  char cmd = REG_WHO_AM_I;
  char who_am_i = 0;

  // Read the WHO_AM_I register value (unique identifier for MMA8451)
  char reg[1] = {(char)REG_WHO_AM_I};

  p_comm_handle->_p_i2c_interface->write(
      ACCEL_I2C_ADDR, &cmd, 1, true); // true = keep the connection open
  // Read the specified number of bytes from the accelerometer into data buffer
  p_comm_handle->_p_i2c_interface->read(ACCEL_I2C_ADDR, &who_am_i, 1);

//   char timing_register[2] = {MMA8451_XYZ_DATA_CFG,
//                              0x02};

  

//   p_comm_handle->_p_i2c_interface->write(ACCEL_I2C_ADDR, timing_register, 2, true);


  // p_comm_handle->_p_i2c_interface->write(ACCEL_ADDR << 1, &cmd, 1);
  // p_comm_handle->_p_i2c_interface->read(ACCEL_ADDR << 1, &who_am_i, 1);

  // Send WHO_AM_I command and read response to verify the device ID
  // if (p_comm_handle->_p_i2c_interface->write(0x1D << 1, &cmd, 1) == 0 &&
  //     p_comm_handle->_p_i2c_interface->read(0x1D << 1, &who_am_i, 1) == 0) {
  if (who_am_i == 0x1A) {
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
   char cmd[2] = {MMA8451_XYZ_DATA_CFG, 0x00};
  p_comm_handle->_p_i2c_interface->write(ACCEL_I2C_ADDR, cmd, 2);
  cmd[0]=REG_CTRL_REG;
  cmd[1]=0x01;

  if (p_comm_handle->_p_i2c_interface->write(ACCEL_I2C_ADDR, cmd, 2) == 0) {
    // is_sensor_available = true;
    return true;
  }
  // is_sensor_available = false;
  return false;
}

// Read raw data from the accelerometer for each axis
void Accelerometer_handle::read_meassurement() {
  Sensors_interface *p_comm_handle = sensors_interface_get();
  char data[2];
  char cmd;

  // Read X-axis data
  cmd = REG_OUT_X_MSB;
  p_comm_handle->_p_i2c_interface->write(ACCEL_I2C_ADDR, &cmd, 1, true);
  p_comm_handle->_p_i2c_interface->read(ACCEL_I2C_ADDR, data, 2);
  int16_t raw_x = (data[0] << 8) | (data[1] );
//   if (raw_x > UINT14_MAX / 2)
//     raw_x -= UINT14_MAX;
raw_x>>=2;
  //acc_x_value = ((float)raw_x) / 4096.0f;
  acc_x_value_m_s = acc_x_value * G_COVERSION_FACTOR;

  // Read Y-axis data
  cmd = REG_OUT_Y_MSB;
  p_comm_handle->_p_i2c_interface->write(ACCEL_I2C_ADDR, &cmd, 1);
  p_comm_handle->_p_i2c_interface->read(ACCEL_I2C_ADDR, data, 2);
  volatile int16_t raw_y = (data[0] << 8) | (data[1] );
//   if (raw_y > UINT14_MAX / 2)
//     raw_y -= UINT14_MAX;
raw_y>>=2;
  //acc_y_value = static_cast<float>(raw_y) / 4096.0f;
  acc_y_value_m_s = acc_y_value * G_COVERSION_FACTOR;

  // Read Z-axis data
  cmd = REG_OUT_Z_MSB;
  p_comm_handle->_p_i2c_interface->write(ACCEL_I2C_ADDR, &cmd, 1);
  p_comm_handle->_p_i2c_interface->read(ACCEL_I2C_ADDR, data, 2);
  int16_t raw_z = (data[0] << 8) | (data[1] );
  raw_z>>=2;
//   if (raw_z > UINT14_MAX / 2)
//     raw_z -= UINT14_MAX;
  //acc_z_value = static_cast<float>(raw_z) / 4096.0f;
  acc_z_value_m_s = acc_z_value * G_COVERSION_FACTOR;


  App* app=app_get();


  if(app->get_mode() == NORMAL_MODE ||app->get_mode() == ADVANCE_MODE){
        if (app->counter == 0) {
        reset_min_max_mean();
        }else {
            // Update min, max and maean values of x
            min_acc_x_value = min(min_acc_x_value, acc_x_value_m_s);
            max_acc_x_value = max(min_acc_x_value, acc_x_value_m_s);
            mean_acc_x_value = mean_acc_x_value + (acc_x_value_m_s - mean_acc_x_value) / p_comm_handle->counter;

            // Update min, max and maean values of y
            min_acc_y_value = min(min_acc_y_value, acc_y_value_m_s);
            max_acc_y_value = max(min_acc_y_value, acc_y_value_m_s);
            mean_acc_y_value = mean_acc_y_value + (acc_y_value_m_s - mean_acc_y_value) / p_comm_handle->counter;

            // Update min, max and maean values of z
            min_acc_z_value = min(min_acc_z_value, acc_z_value_m_s);
            max_acc_z_value = max(min_acc_z_value, acc_z_value_m_s);
            mean_acc_z_value = mean_acc_z_value + (acc_z_value_m_s - mean_acc_z_value) / p_comm_handle->counter;

            
        }
    }
}

// Get individual axis values
float Accelerometer_handle::get_acc_x_value() { return acc_x_value; }
float Accelerometer_handle::get_acc_y_value() { return acc_y_value; }
float Accelerometer_handle::get_acc_z_value() { return acc_z_value; }


float Accelerometer_handle::get_min_acc_x_value() { return min_acc_x_value; }
float Accelerometer_handle::get_min_acc_y_value() { return min_acc_y_value; }
float Accelerometer_handle::get_min_acc_z_value() { return min_acc_z_value; }
float Accelerometer_handle::get_max_acc_x_value() { return max_acc_x_value; }
float Accelerometer_handle::get_max_acc_y_value() { return max_acc_y_value; }
float Accelerometer_handle::get_max_acc_z_value() { return max_acc_z_value; }
float Accelerometer_handle::get_mean_acc_x_value() { return mean_acc_x_value; }
float Accelerometer_handle::get_mean_acc_y_value() { return mean_acc_y_value; }
float Accelerometer_handle::get_mean_acc_z_value() { return mean_acc_z_value; }

void Accelerometer_handle::reset_min_max_mean() {
 min_acc_x_value = acc_x_value;
 min_acc_y_value = acc_y_value;
 min_acc_z_value = acc_z_value;
 max_acc_x_value = acc_x_value;
 max_acc_y_value = acc_y_value;
 max_acc_z_value = acc_z_value;
 mean_acc_x_value = 0.0;
 mean_acc_y_value = 0.0;
 mean_acc_z_value = 0.0;

}

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
