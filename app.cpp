

#include "app.h"
#include "sensors_handle.h"
#include "user_interface.h"
#include "mbed.h"

App *p_app;

Ticker tkr_system_cycle;
DigitalOut temperature_sensor_VCC(PB_6);

inline void Sleep(void) {
  // Ensure Flash memory stays on
  FLASH->ACR &= ~FLASH_ACR_SLEEP_PD;

  // Configure low-power mode
  SCB->SCR &= ~(SCB_SCR_SLEEPDEEP_Msk); // low-power mode = sleep mode
  SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk; // reenter low-power mode after ISR   


  // Enter low-power mode
  __WFI();
}

// Comunications Ports
static I2C i2c(PB_9, PB_8);
BufferedSerial serial_port_A(PA_9, PA_10,
                             9600); // To communicate with gps senspr
BufferedSerial serial_port_B(USBTX, USBRX,
                             115200); // To communicate with pc (User Interface)

// Internal Funtions declaration

void activate_meassurement_reading_cb();

//-------------------------CLASS Methods & Contructor

App::App()
    : flag_change_mode(false), _status(IDLE), _mode(TEST_MODE),
      flag_ready_to_sleep(false), show_stats(false) {}

App_cycle_state App::get_status() { return p_app->_status; }
void App::set_status(App_cycle_state new_status) {
  p_app->_status = new_status;
}
App_mode App::get_mode() { return p_app->_mode; }
void App::change_to_next_mode() {
//     user_interface_handle *p_ui = user_interface_get();
//   Sensors_interface *p_sensor_handle = sensors_interface_get();
//   p_sensor_handle->counter = 0;
//   if (p_app->_mode == TEST_MODE) {
//       temperature_sensor_VCC=1;
//     p_app->_mode = NORMAL_MODE;
    
//     snprintf(p_ui->buffer, sizeof(p_ui->buffer), "----NORMAL MODE----\n");

    
//     tkr_system_cycle.attach_us(activate_meassurement_reading_cb,
//                                NORMAL_MODE_UPDATE_TIME_US);
//   } else if (p_app->_mode == NORMAL_MODE) {

// #ifndef ADVANCE_MODE_ON
//     p_app->_mode = TEST_MODE;
//     snprintf(p_ui->buffer, sizeof(p_ui->buffer), "----TEST MODE----\n");
//     tkr_system_cycle.attach_us(activate_meassurement_reading_cb,
//                                TEST_MODE_UPDATE_TIME_US);
// #else
// temperature_sensor_VCC=1;
//     p_app->_mode = ADVANCE_MODE;
//     snprintf(p_ui->buffer, sizeof(p_ui->buffer), "----ADVANCED MODE----\n");
//     tkr_system_cycle.attach_us(activate_meassurement_reading_cb,
//                                TEST_MODE_UPDATE_TIME_US);
// #endif
//   } else {
// #ifdef ADVANCE_MODE_ON
//     p_app->_mode = TEST_MODE;
//     snprintf(p_ui->buffer, sizeof(p_ui->buffer), "----TEST MODE----\n");
//     tkr_system_cycle.attach_us(activate_meassurement_reading_cb,
//                                TEST_MODE_UPDATE_TIME_US);
// // #endif
//   }

//   p_ui->print_msg(p_ui->buffer);

  //
}

App *app_get() { return p_app; }

//----------------------------------------Internal Funtions

void activate_meassurement_reading_cb() {
  Sensors_interface *p_sensor_handle = sensors_interface_get();
  p_sensor_handle->start_messurement_cycle = true;
  temperature_sensor_VCC=1;
  //   to_check_meassurements.attach_us(activate_meassurement_reading,
  //                                    TEST_MODE_UPDATE_TIME_MS);
}

//---------------------------------External Funtions (can be called from another
// module)

void app_init() {
  p_app = new App();

  user_interface_init(&serial_port_B);
  user_interface_handle *p_handle = user_interface_get();

  sensors_interface_init(&i2c, &serial_port_A);
  Sensors_interface *p_sensor_handle = sensors_interface_get();
  tkr_system_cycle.attach_us(activate_meassurement_reading_cb,
                             TEST_MODE_UPDATE_TIME_US);
  //   to_check_meassurements.attach_us(activate_meassurement_reading,
  //                                    TEST_MODE_UPDATE_TIME_MS);
  // tmr_system_cycle.start();
  temperature_sensor_VCC=1;
}

void app_state_control() {
  user_interface_handle *p_handle = user_interface_get();

//   if (p_app->show_stats) {
//     p_app->show_stats = false;
//     p_handle->print_stats();
//   }
  if (p_handle->display_new_update == true) {
    p_handle->print_new_update();
    // p_handle->set_RGB_led_color();
  }
  

  // Check if the app state need to be updated
//   if (p_app->flag_change_mode) {
//     tkr_system_cycle.detach();
//     p_app->flag_change_mode = false;
//     p_app->change_to_next_mode(); // Change Mode and also update the ticker
//   }

//   if(p_app->get_mode()==ADVANCE_MODE){
//       Sensors_interface *p_sensor_handle = sensors_interface_get();
//       temperature_sensor_VCC=0;
//       //p_sensor_handle->p_temp_hum->power_off();
      
//         sleep();// Go to sleep while waiting for interruption

//   }


}