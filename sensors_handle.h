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
#ifndef SENSORS_HANDLE_H
#define SENSORS_HANDLE_H

#include <cstdint>
#include "mbed.h"
#include "sensor_states.h"
// #include "accelerometer_handle.h"
// #include "colour_sensor_handle.h"
#include "temp_hum_handle.h"
#include "gps_handle.h"
// #include "soil_moisture_handle.h"
// #include "brightness_handle.h"

#define MEASUREMENTS_MAX_LENGHT 200

#define OUT_RANGE_TEMP_MEA  0b000001
#define OUT_RANGE_HUM_MEA   0b000010
#define OUT_RANGE_SOIL_MEA  0b000100
#define OUT_RANGE_BRIGH_MEA 0b001000
#define OUT_RANGE_ACCEL_MEA 0b010000
#define OUT_RANGE_COLOR_MEA 0b100000


// class Sensor{
//     Sensor_Status
// };


class Sensors_interface{

    public:
    Sensors_interface(); // contructor with sensor address
    char* sensors_interface_get_last_meassurements();
    void sensors_interface_get_stats(); //MAx min and mean in corresponding sensors 
    I2C *_p_i2c_interface;
    BufferedSerial* _serial_port;
    bool start_messurement_cycle;
    bool messurements_finished;
    int counter;

    //Pointer to sensors handles

    Temp_Hum_handle* p_temp_hum;
    // Soil_Moisture_handle* p_soil_moisture;
    GPS_handle* p_gps;
    // Colour_sensor_handle* p_colour_sensor;
    // Brightness_Sensor_handle* p_brightness;
    // Accelerometer_handle* p_accel;



    private:
    char measurements[MEASUREMENTS_MAX_LENGHT];





};

void sensors_interface_init(I2C *p_i2c_interface, BufferedSerial* serial_port);

Sensors_interface* sensors_interface_get();
void Thread_Sensor_meassurement();
#endif // SENSORS_HANDLE_H