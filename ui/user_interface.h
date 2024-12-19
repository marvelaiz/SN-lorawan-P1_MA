// ===========================================================================
// Temp_Hum_handle.h
// ---------------------------------------------------------------------------
// Description:
//   Header file for the UI class, which provides an interface to interact with the User
//
// Author: Mariana Arvelaiz 
// Date: Nov. 2024
// Version: 1.0
// ===========================================================================

#ifndef UI_H
#define UI_H

#include <cstdint>
#include "mbed.h"


typedef enum{
RGB_White=0b000,
RGB_Red=0b110,
RGB_Green=0b101,
RGB_Cyan=0b001, //Cyan
RGB_Yellow=0b100,   //Yellow
RGB_Magenta=0b010,
RGB_Blue=0b011,
RGB_None=0b111,
}RGB_Color; // The logic used here is based on common cathode

class user_interface_handle{

public:
bool btn_pressed;
user_interface_handle(BufferedSerial* serial_port); // contructor with sensor address
void print_new_update();
void print_stats();
void print_msg(char* msg);
void set_RGB_colour(RGB_Color colour);
bool display_new_update;
BufferedSerial* _serial_port;

InterruptIn* User_Btn; 


char buffer[350];
};

void user_interface_init(BufferedSerial* serial_port); // Create and initialize the whole interface
user_interface_handle* user_interface_get();

#endif // TEMP_HUM_HANDLE_H