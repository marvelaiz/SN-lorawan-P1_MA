#ifndef GPS_HANDLE_H
#define GPS_HANDLE_H

#include <cstdint>


#define GPS_DATA_MAX_LENGHT 40
//Class
class GPS_handle{

public:
GPS_handle(); // contructor with sensor address
bool make_meassurement();
bool is_measurement_valid();
void read_meassurement();
float get_latitude();
float get_longitude();
void parseGPGGA(char* gpgga);


char* get_data_to_print();

bool is_sensor_available;
char formated_data[GPS_DATA_MAX_LENGHT];
char data[GPS_DATA_MAX_LENGHT];
private:
uint8_t address;
int num_satellites;
float latitude;
float longitude;
float altitude;
float hpop;
char latDirection, longDirection;
int hour;
int minutes;
int seconds;

};


//External funtions
GPS_handle* GPS_handle_get();
void GPS_handle_init(); // Create and initialize the whole interface



#endif // GPS_HANDLE_H