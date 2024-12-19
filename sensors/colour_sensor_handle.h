#ifndef COLOUR_SENSOR_HANDLE_H
#define COLOUR_SENSOR_HANDLE_H

#include <cstdint>





//RGB Sensor Adress and registers 
#define TCS3472_ADDRESS 0x29
#define TCS3472_ENABLE 0x00

#define TCS3472_ATIME 0X01      //RGBC Timing Register (0x01)


#define TCS3472_CONTROL 0x0F
#define TCS3472_ID 0x12
#define TCS3472_STATUS 0x13
#define TCS3472_CDATAL 0X14
#define TCS3472_CDATAH 0X15
#define TCS3472_RDATAL 0X16
#define TCS3472_RDATAH 0X17
#define TCS3472_GDATAL 0X18
#define TCS3472_GDATAH 0X19
#define TCS3472_BDATAL 0X1A
#define TCS3472_BDATAH 0X1B

//Enable Register
#define TCS3472_ENABLE_CMD_PON      0b00000001
#define TCS3472_ENABLE_CMD_AEN      0b00000010
#define TCS3472_ENABLE_CMD_AIEN     0b00010000

//RGBC Timing Register
#define TCS3472_ATIME_CMD_1CYCLES 0xFF  //INTEG_CYCLES: 1  TIME: 2.4 ms  MAAAX COUNT: 1024
#define TCS3472_ATIME_CMD_256CYCLES 0x00  //INTEG_CYCLES: 256  TIME: 700 ms  MAX COUNT: 65535

//Control Register values
#define TCS3472_CONTROL_CMD_1xGAIN  0x00
#define TCS3472_CONTROL_CMD_16xGAIN 0x03
#define TCS3472_CONTROL_CMD_60xGAIN 0x04


#define TCS3472_COMMAND_MASK 0x80    


//MACROS
#define COLOUR_ADDR TCS3472_ADDRESS
#define COLOUR_DATA_MAX_LENGHT 60

#define DOMINANT_COLOUR_RED "Red is Dominant"
#define DOMINANT_COLOUR_GREEN "Green is Dominant"
#define DOMINANT_COLOUR_BLUE "Blue is Dominant"
#define DOMINANT_COLOUR_NONE "None is Dominant"

//IMPORTANT CMD OF THE SENSOR 
//(missing)Here we need to create a macros with the cmds of the sensor used and NOT to use them directly inside the funtions 

typedef enum {
  NONE=0,
  RED = 1,
  GREEN = 2,
  BLUE = 3,
}Colours;

//Class
class Colour_sensor_handle{

public:
Colour_sensor_handle(uint8_t address); // contructor with sensor address
bool make_meassurement();
void read_meassurement();
void* _Sensor_Led;
char* get_data_to_print();
Colours get_dominant_colour();
uint16_t get_clear();
uint16_t get_red();
uint16_t get_green();
uint16_t get_blue();


bool is_sensor_available;
char formated_data[COLOUR_DATA_MAX_LENGHT];
Colours dominant_colour;

private:


uint8_t address;
uint16_t red;
uint16_t blue;
uint16_t green;
uint16_t clear;


};


//External funtions
Colour_sensor_handle* Colour_sensor_handle_get();
void Colour_sensor_handle_init(uint8_t address); // Create and initialize the whole interface



#endif // COLOUR_SENSOR_HANDLE_H