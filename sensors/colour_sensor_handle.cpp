// ===========================================================================
// Colour_sensor_handle.cpp
// ---------------------------------------------------------------------------
// Description:
// Implementation of the Colour_sensor_handle class.
// Author: Mariana Arvelaiz
// Date: Nov. 2024
// Version: 1.0
// ===========================================================================

#include "colour_sensor_handle.h"
#include "app.h"
#include "sensors_handle.h"
#include "user_interface.h"
#include <cstdint>
#include <cstdio>
#include <cstring>


#define LED_HIGH 1
#define LED_LOW 0
Colour_sensor_handle *colour_sensor_handle;
DigitalOut Sensor_Led(LED4); // PB_7

bool initTCS34725() {
  Sensors_interface *p_comm_handle = sensors_interface_get();

  // To validtae the sensor read the Part Id in  0x12 Register (ID Register)
  // FOR TCS34725 should be 0x44
  char cmd = TCS3472_COMMAND_MASK | TCS3472_ID;
  char data[1] = {0};
  p_comm_handle->_p_i2c_interface->write(COLOUR_ADDR << 1, &cmd, 1, true);
  p_comm_handle->_p_i2c_interface->read(COLOUR_ADDR << 1, data, 1, false);

  if (data[0] == 0x44) {
    return true; // Device detected
  } else {
    return false; // Device not detected (sensor not available).
  }

  // // Send the command to the sensor; if successful, mark it as available.
  // if (p_comm_handle->_p_i2c_interface->write(0x40 << 1, &cmd, 1) == 0) {
  //     return true; // Initialization successful.
  // }
}

// Class methods

Colour_sensor_handle::Colour_sensor_handle(uint8_t address)
    : address(address), is_sensor_available(false), red(0), green(0), blue(0),
      counter_blue(0), counter_clear(0), counter_green(0), counter_red(0) {}

char *Colour_sensor_handle::get_data_to_print() {
  // This funtion configure the format in which the sensors reading is going to
  // be printed
  evaluate_dominant_colour();
  if(dominant_colour==RED){
      snprintf(formated_data, sizeof(formated_data),
           "COLOUR: Red:%d Green: %d Blue: %d, Dominant color:%s\n", red, green, blue, DOMINANT_COLOUR_RED);

  }else if(dominant_colour==BLUE){
      snprintf(formated_data, sizeof(formated_data),
           "COLOUR: Red:%d Green: %d Blue: %d, Dominant color:%s\n", red, green, blue, DOMINANT_COLOUR_BLUE);

  }else if(dominant_colour==GREEN){
      snprintf(formated_data, sizeof(formated_data),
           "COLOUR: Red:%d Green: %d Blue: %d, Dominant color:%s\n", red, green, blue, DOMINANT_COLOUR_GREEN);

  }
  
  return formated_data;
}

void Colour_sensor_handle::evaluate_dominant_colour() {
  if (green > blue && green > red) {
    dominant_colour = GREEN;
  } else if (blue > green & blue > red) {
    dominant_colour = BLUE;
  } else if (red > green & red > blue) {
    dominant_colour = RED;
  } else {
    dominant_colour = NONE;
  }
}

void Colour_sensor_handle::evaluate_dominant_colour_in_period() {

  if (counter_green >= counter_blue && counter_green >= counter_red) {
    dominant_colour = GREEN;

  } else if (counter_blue >= counter_green && counter_blue >= counter_red) {
    dominant_colour = BLUE;
  } else if (counter_red >= counter_green && counter_red >= counter_blue) {
    dominant_colour = RED;
  } else {
    dominant_colour = NONE;
  }

  reset_mean_dominant_colour();
}

void Colour_sensor_handle::reset_mean_dominant_colour() {
  counter_clear = 0;
  counter_blue = 0;
  counter_green = 0;
  counter_red = 0;
}

Colours Colour_sensor_handle::get_dominant_colour() { return dominant_colour; }

bool Colour_sensor_handle::make_meassurement() {

  Sensors_interface *p_comm_handle = sensors_interface_get();

  char cmd;
  char data[2] = {0, 0};
  Sensor_Led = (int)LED_HIGH;

  // Read Clean

  cmd = TCS3472_COMMAND_MASK |
        TCS3472_CDATAL; // Preperare to read TCS3472_CDATAL register
  p_comm_handle->_p_i2c_interface->write(COLOUR_ADDR << 1, &cmd, 1, true);
  p_comm_handle->_p_i2c_interface->read(COLOUR_ADDR << 1, data, 2, false);

  clear = (data[1] << 8) | data[0];

  // Read Read

  memset(data, 0,
         sizeof(data)); // Init the data buffer to receve another meassurement;
  cmd = TCS3472_COMMAND_MASK |
        TCS3472_RDATAL; // Preperare to read TCS3472_RDATAL register

  p_comm_handle->_p_i2c_interface->write(COLOUR_ADDR << 1, &cmd, 1, true);
  p_comm_handle->_p_i2c_interface->read(COLOUR_ADDR << 1, data, 2, false);

  red = (data[1] << 8) | data[0];

  // Read Green

  memset(data, 0,
         sizeof(data)); // Init the data buffer to receve another meassurement;
  cmd = TCS3472_COMMAND_MASK |
        TCS3472_GDATAL; // Preperare to read TCS3472_GDATAL register
  p_comm_handle->_p_i2c_interface->write(COLOUR_ADDR << 1, &cmd, 1, true);
  p_comm_handle->_p_i2c_interface->read(COLOUR_ADDR << 1, data, 2, false);

  green = (data[1] << 8) | data[0];

  // Read Blue

  memset(data, 0,
         sizeof(data)); // Init the data buffer to receve another meassurement;
  cmd = TCS3472_COMMAND_MASK |
        TCS3472_BDATAL; // Preperare to read TCS3472_CDATAL register
  p_comm_handle->_p_i2c_interface->write(COLOUR_ADDR << 1, &cmd, 1, true);
  p_comm_handle->_p_i2c_interface->read(COLOUR_ADDR << 1, data, 2, false);

  blue = (data[1] << 8) | data[0];

  App *app = app_get();

  if (app->get_mode() == NORMAL_MODE ||app->get_mode() == ADVANCE_MODE) {

    if (p_comm_handle->counter == 0) {
      
    } else {
      // Update counter values
      if (green > blue && green > red) {
        // dominant_colour = GREEN;
        counter_green++;
      } else if (blue > green & blue > red) {
        // dominant_colour = BLUE;
        counter_blue++;
      } else if (red > green & red > blue) {
        dominant_colour = RED;
        counter_red++;
      } else {
        counter_clear++;
      }
      
    }
  }

  return true;
}

uint16_t Colour_sensor_handle::get_clear(){return clear;}
uint16_t Colour_sensor_handle::get_red(){return red;}
uint16_t Colour_sensor_handle::get_green(){return green;}
uint16_t Colour_sensor_handle::get_blue(){return blue;}

void Colour_sensor_handle::read_meassurement() {

  Sensors_interface *p_comm_handle = sensors_interface_get();

  Sensor_Led = LED_HIGH;

  char cmd;
  char data[2] = {0, 0};

  // Read Clean

  cmd = TCS3472_COMMAND_MASK |
        TCS3472_CDATAL; // Preperare to read TCS3472_CDATAL register
  p_comm_handle->_p_i2c_interface->write(COLOUR_ADDR << 1, &cmd, 1, true);
  p_comm_handle->_p_i2c_interface->read(COLOUR_ADDR << 1, data, 2, false);

  clear = ((int)data[1] << 8) | data[0];

  // Read Read

  memset(data, 0,
         sizeof(data)); // Init the data buffer to receve another meassurement;
  cmd = TCS3472_COMMAND_MASK |
        TCS3472_RDATAL; // Preperare to read TCS3472_RDATAL register

  p_comm_handle->_p_i2c_interface->write(COLOUR_ADDR << 1, &cmd, 1, true);
  p_comm_handle->_p_i2c_interface->read(COLOUR_ADDR << 1, data, 2, false);

  red = ((int)data[1] << 8) | data[0];

  // Read Green

  memset(data, 0,
         sizeof(data)); // Init the data buffer to receve another meassurement;
  cmd = TCS3472_COMMAND_MASK |
        TCS3472_GDATAL; // Preperare to read TCS3472_GDATAL register
  p_comm_handle->_p_i2c_interface->write(COLOUR_ADDR << 1, &cmd, 1, true);
  p_comm_handle->_p_i2c_interface->read(COLOUR_ADDR << 1, data, 2, false);

  green = ((int)data[1] << 8) | data[0];

  // Read Blue

  memset(data, 0,
         sizeof(data)); // Init the data buffer to receve another meassurement;
  cmd = TCS3472_COMMAND_MASK |
        TCS3472_BDATAL; // Preperare to read TCS3472_CDATAL register
  p_comm_handle->_p_i2c_interface->write(COLOUR_ADDR << 1, &cmd, 1, true);
  p_comm_handle->_p_i2c_interface->read(COLOUR_ADDR << 1, data, 2, false);

  blue = ((int)data[1] << 8) | data[0];

  Sensor_Led = LED_LOW;

  evaluate_dominant_colour();
}

// External Funtions

Colour_sensor_handle *Colour_sensor_handle_get() {
  return colour_sensor_handle;
}

void Colour_sensor_handle_init(uint8_t address) {
  // Allocate the pointer
  colour_sensor_handle = new Colour_sensor_handle(address);
  memset(colour_sensor_handle->formated_data, '\0',
         sizeof(colour_sensor_handle->formated_data));

  colour_sensor_handle->_Sensor_Led = &Sensor_Led;
  Sensor_Led = LED_LOW;

  if (initTCS34725()) {

    snprintf(colour_sensor_handle->formated_data,
             sizeof(colour_sensor_handle->formated_data),
             "TCS34725 sensor detected.\n");
    colour_sensor_handle->is_sensor_available = true;
  } else {
    snprintf(colour_sensor_handle->formated_data,
             sizeof(colour_sensor_handle->formated_data),
             "TCS34725 sensor not detected.\n");
    colour_sensor_handle->is_sensor_available = false;
  }
  user_interface_handle *p_handle = user_interface_get();
  p_handle->print_msg(colour_sensor_handle->formated_data);

  // Code to configure the sensor for first measurement if needed

  // Sensor init
  Sensors_interface *p_comm_handle = sensors_interface_get();

  // Set ATIME of each meassurement (0x00->256 integrationa ~700ms each new
  // meassurement)
  char timing_register[2] = {TCS3472_COMMAND_MASK | TCS3472_ATIME,
                             TCS3472_ATIME_CMD_256CYCLES};
  p_comm_handle->_p_i2c_interface->write(COLOUR_ADDR << 1, timing_register, 2,
                                         false);

  char control_register[2] = {TCS3472_COMMAND_MASK | TCS3472_CONTROL,
                              TCS3472_CONTROL_CMD_1xGAIN};
  p_comm_handle->_p_i2c_interface->write(COLOUR_ADDR << 1, control_register, 2,
                                         false);

  char enable_register[2] = {TCS3472_COMMAND_MASK | TCS3472_ENABLE,
                             TCS3472_ENABLE_CMD_PON | TCS3472_ENABLE_CMD_AEN};
  p_comm_handle->_p_i2c_interface->write(COLOUR_ADDR << 1, enable_register, 2,
                                         false);
}