// ===========================================================================
// sensors_handle.h
// ---------------------------------------------------------------------------
// Description:
//   Header file for the sensors_handle class adn sesors interface funtion, it will work as the Control Center to get the correct  I2C communication interface
//
// Author: Mariana Arvelaiz 
// Date: Nov. 2024
// Version: 1.0
// ===========================================================================
#ifndef SENSORS_STATE_H
#define SENSORS_STATE_H

#include <cstdint>


typedef enum {
  IDDLE = 0,
  DOING_MEAS = 1,
  MEAS_AVAILABLE = 2,
}Sensor_Control_Status;

typedef enum {
  MEA_OK = 0,
  MEA_OUT_RANGE = 1,
}Measurement_Validation;






#endif // SENSORS_STATE_H