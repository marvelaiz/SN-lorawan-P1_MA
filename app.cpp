

#include "app.h"
#include "sensors_handle.h"
#include "user_interface.h"
#include "mbed.h"

App *p_app;

Ticker tkr_system_cycle;
DigitalOut temperature_sensor_VCC(PB_6);

// inline void Sleep(void) {
//   // Ensure Flash memory stays on
//   FLASH->ACR &= ~FLASH_ACR_SLEEP_PD;

//   // Configure low-power mode
//   SCB->SCR &= ~(SCB_SCR_SLEEPDEEP_Msk); // low-power mode = sleep mode
//   SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk; // reenter low-power mode after ISR   


//   // Enter low-power mode
//   __WFI();
// }

// Comunications Ports
static I2C i2c(PB_9, PB_8);
BufferedSerial serial_port_A(PA_9, PA_10,
                             9600); // To communicate with gps senspr
BufferedSerial serial_port_B(USBTX, USBRX,
                             115200); // To communicate with pc (User Interface)

// Internal Funtions declaration


//-------------------------CLASS Methods & Contructor

App::App()
    : _status(IDLE), _mode(TEST_MODE)
       {}


App_mode App::get_mode() { return p_app->_mode; }


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
  
  temperature_sensor_VCC=1;
}

