

#include "user_interface.h"
// #include "app.h"
#include "sensors_handle.h"
#include "string.h"
#include <cstdio>
#include <cstring>

user_interface_handle *user_interface;

BusOut RGB_Led(D11, D12, D13);


#define LED1_HIGH 1
#define LED1_LOW 0

// void user_interface_check_sensors_display_RGB();

user_interface_handle::user_interface_handle(BufferedSerial *serial_port)
    : _serial_port(serial_port), display_new_update(false) {
  memset(buffer, '\0', sizeof(buffer));
}
void user_interface_handle::print_new_update() {
    memset(buffer, '\0', sizeof(buffer));
  Sensors_interface *p_handle = sensors_interface_get();
  char *p_data = p_handle->sensors_interface_get_last_meassurements();
  sprintf(buffer + (strlen(buffer)), "%s \n", p_data);

  if (_serial_port->writable()) {
    _serial_port->write(buffer, sizeof(buffer));
  }
  display_new_update = false;
}

void user_interface_handle::print_stats() {
   

  if (_serial_port->writable()) {
    _serial_port->write(buffer, sizeof(buffer));
  }
  
}

void user_interface_handle::print_msg(char *msg) {
  snprintf(buffer, sizeof(buffer), "%s", msg);
  _serial_port->write(buffer, strlen(buffer));
}



user_interface_handle *user_interface_get() { return user_interface; }


void user_interface_handle::set_RGB_colour(RGB_Color colour){
    RGB_Led.write(colour);
}



void user_interface_init(BufferedSerial *serial_port) {
  user_interface = new user_interface_handle(serial_port);
  RGB_Led.write(RGB_None);

}