

#include "user_interface.h"
// #include "app.h"
#include "sensors_handle.h"
#include "string.h"
#include <cstdio>
#include <cstring>

user_interface_handle *user_interface;

BusOut RGB_Led(D11, D12, D13);
DigitalOut led1(LED1);
DigitalOut led2(LED2);
InterruptIn User_Button(PB_2); //  User btn it is going to be user input to the
                               //  system. Implemented with interruptions
#define LED1_HIGH 1
#define LED1_LOW 0

// void user_interface_check_sensors_display_RGB();

user_interface_handle::user_interface_handle(BufferedSerial *serial_port)
    : _serial_port(serial_port), display_new_update(false) {
  memset(buffer, '\0', sizeof(buffer));
}
void user_interface_handle::print_new_update() {
    memset(buffer, '\0', sizeof(buffer));
  sprintf(buffer, "\n\nNew  Update \n");
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

void user_interface_handle::set_RGB_led_color() {
//   App *p_handle = app_get();

//   if (p_handle->get_mode() == TEST_MODE) {
    // led1 = LED1_HIGH;
    // led2 = LED1_LOW;
    // Colour_sensor_handle *p_colour_sensor = Colour_sensor_handle_get();
    // Colours dominant = p_colour_sensor->get_dominant_colour();

    // if (dominant == RED) {
    //   RGB_Led.write(RGB_Red);

    // } else if (dominant == BLUE) {
    //   RGB_Led.write(RGB_Blue);

    // } else if (dominant == GREEN) {
    //   RGB_Led.write(RGB_Green);

    // } else {
    //   RGB_Led.write(RGB_None);
    // }

//   } else if (p_handle->get_mode() == NORMAL_MODE ||p_handle->get_mode() == ADVANCE_MODE) {
//     led1 = LED1_LOW;
//     led2 = LED1_HIGH;
//     RGB_Led.write(RGB_None);
//     // void user_interface_check_sensors_display_RGB();
//     // Temperature & HUM
//     // Temp_Hum_handle *p_temp_hum = Temp_Hum_handle_get();

//     // if (p_temp_hum->is_temp_measurement_out_of_range()) {
//     //   RGB_Led.write(RGB_Magenta);
//     // } else if (p_temp_hum->is_hum_measurement_out_of_range()) {
//     //   RGB_Led.write(RGB_Cyan);
//     // }

//     Sensors_interface *p_handle = sensors_interface_get();
//     if (p_handle->p_temp_hum->is_temp_measurement_out_of_range()) {
//       RGB_Led.write(RGB_Magenta);
//     } else if (p_handle->p_temp_hum->is_hum_measurement_out_of_range()) {
//       RGB_Led.write(RGB_Cyan);
//     } else if (p_handle->p_soil_moisture->is_measurement_out_of_range()) {
//       RGB_Led.write(RGB_Yellow);
//     } else if (p_handle->p_brightness->is_measurement_out_of_range()) {
//       RGB_Led.write(RGB_Blue);
//     } else if (p_handle->p_accel->is_measurement_out_of_range()) {
//       RGB_Led.write(RGB_Red);
//     }
//   }
}

user_interface_handle *user_interface_get() { return user_interface; }

// void user_interface_thread() {

//   if (user_interface->display_new_update == true) {
//     user_interface->print_new_update();
//   }

// } // Thread display meassuments on the terminal

// void user_interface_check_sensors_display_RGB(){
//     Sensors_interface* p_handle=sensors_interface_get();
//     if (p_handle->p_temp_hum->is_temp_measurement_out_of_range()) {
//       RGB_Led.write(RGB_Magenta);
//     } else if (p_handle->p_temp_hum->is_hum_measurement_out_of_range()) {
//       RGB_Led.write(RGB_Cyan);
//     }

// }

void user_interface_btn_pressed_cb() {

  // Change to next mode when its needed
//   App *p_handle = app_get();
//   p_handle->flag_change_mode = true;
//   p_handle->flag_ready_to_sleep = false;
}

void user_interface_init(BufferedSerial *serial_port) {
  user_interface = new user_interface_handle(serial_port);
  RGB_Led.write(RGB_White);

  // Setup interrupt funtions to track user interactions with the system
  user_interface->User_Btn = &User_Button;

  User_Button.mode(PullUp); // declare the pin mode
  User_Button.fall(
      user_interface_btn_pressed_cb); // setup the Btn press cb to be executed
                                      // when PB_2 signal fall

  // Int the BTN and atach the needed funtions
}