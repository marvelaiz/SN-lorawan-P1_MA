// ===========================================================================
// Aaccelerometer_handle
// ---------------------------------------------------------------------------
// Description:
//   Header file for the Aaccelerometer_handle class, which provides an interface for
//   reading Three different Acces (x-y-z) values from a Adafruit MMA8451 sensor with I2C communication interface
//
// Author: Mariana Arvelaiz
// Date: Nov. 2024
// Version: 1.0
// ===========================================================================
#ifndef ACELEROMETER_HANDLE_H
#define ACELEROMETER_HANDLE_H

#include <cstdint>
#include "sensor_states.h"
//MACROS
#define ACC_DATA_MAX_LENGHT 45
#define ACCEL_ADDR 0x1D
#define ACCEL_I2C_ADDR (0x1D << 1)

// Register addresses for the MMA8451 accelerometer
#define REG_WHO_AM_I   0x0D   // WHO_AM_I register to identify the device
#define REG_CTRL_REG  0x2A   // Control register for configuring the device
#define MMA8451_XYZ_DATA_CFG 0x0E
#define REG_OUT_X_MSB  0x01   // X-axis MSB output register
#define REG_OUT_Y_MSB  0x03   // Y-axis MSB output register
#define REG_OUT_Z_MSB  0x05   // Z-axis MSB output register
#define G_COVERSION_FACTOR 9.81


//Range
#define ACCEL_X_MAX_VALUE_M_S2 10
#define ACCEL_X_MIN_VALUE_M_S2 -1
#define ACCEL_Y_MAX_VALUE_M_S2 10
#define ACCEL_Y_MIN_VALUE_M_S2 -1
#define ACCEL_Z_MAX_VALUE_M_S2 15
#define ACCEL_Z_MIN_VALUE_M_S2 0.5



//Class
class Accelerometer_handle{

public:
Accelerometer_handle(uint8_t address); // contructor with sensor address
bool make_meassurement();
void read_meassurement();
float get_acc_x_value();
uint16_t get_raw_x_value();
float get_acc_y_value();
uint16_t get_raw_y_value();
float get_acc_z_value();
uint16_t get_raw_z_value();
char* get_data_to_print();

Measurement_Validation meassurement_state;
bool is_measurement_out_of_range();

bool is_sensor_available;
char formated_data[ACC_DATA_MAX_LENGHT];

private:
uint8_t address;
float acc_x_value;
float acc_y_value;
float acc_z_value;
uint16_t raw_x_value;
uint16_t raw_y_value;
uint16_t raw_z_value;

float acc_x_value_m_s;  //Value in m/s
float acc_y_value_m_s;  //Value in m/s
float acc_z_value_m_s;  //Value in m/s



};


//External funtions
Accelerometer_handle* Accelerometer_handle_get();
void Accelerometer_handle_init(uint8_t address); // Create and initialize the whole interface








#endif // ACELEROMETER_HANDLE_H