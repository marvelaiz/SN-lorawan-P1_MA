/**
 * Copyright (c) 2017, Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <cstdint>
#include <cstdio>

#include "mbed_version.h"

#include "lorawan/LoRaWANInterface.h"
#include "lorawan/system/lorawan_data_structures.h"
#include "events/EventQueue.h"

// Application helpers
#include "DummySensor.h"
#include "trace_helper.h"
#include "lora_radio_helper.h"

// Custom Libraries 
#include "app.h"
#include "sensors_handle.h"
#include "user_interface.h"


//Custom variables 
//Thread ui_thread;
Thread sensors_thread;

using namespace events;
using namespace std::chrono_literals;

// Max payload size can be LORAMAC_PHY_MAXPAYLOAD.
// This example only communicates with much shorter messages (<30 bytes).
// If longer messages are used, these buffers must be changed accordingly.
uint8_t tx_buffer[30];
uint8_t rx_buffer[30];

/*
 * Sets up an application dependent transmission timer in ms. Used only when Duty Cycling is off for testing
 */
#define TX_TIMER                        10s

/**
 * Maximum number of events for the event queue.
 * 10 is the safe number for the stack events, however, if application
 * also uses the queue for whatever purposes, this number should be increased.
 */
#define MAX_NUMBER_OF_EVENTS            10

/**
 * Maximum number of retries for CONFIRMED messages before giving up
 */
#define CONFIRMED_MSG_RETRY_COUNTER     3

/**
 * Dummy pin for dummy sensor
 */
#define PC_9                            0

/**
 * Dummy sensor class object
 */
DS1820  ds1820(PC_9);

/**
* This event queue is the global event queue for both the
* application and stack. To conserve memory, the stack is designed to run
* in the same thread as the application and the application is responsible for
* providing an event queue to the stack that will be used for ISR deferment as
* well as application information event queuing.
*/
static EventQueue ev_queue(MAX_NUMBER_OF_EVENTS *EVENTS_EVENT_SIZE);

/**
 * Event handler.
 *
 * This will be passed to the LoRaWAN stack to queue events for the
 * application which in turn drive the application.
 */
static void lora_event_handler(lorawan_event_t event);

/**
 * Constructing Mbed LoRaWANInterface and passing it the radio object from lora_radio_helper.
 */
static LoRaWANInterface lorawan(radio);

/**
 * Application specific callbacks
 */
static lorawan_app_callbacks_t callbacks;


/**
 * testing functions
 */

size_t payload_packaging(){

    /*Testing the payload  packaging process*/

    //Retrive meassurements
    Sensors_interface *sensors_interface = sensors_interface_get();
    sensors_interface=sensors_interface_get();

    if (sensors_interface->p_temp_hum->is_sensor_available) {
        sensors_interface->p_temp_hum->read_meassurement();
    }
if (sensors_interface->p_gps->is_sensor_available) {
    sensors_interface->p_gps->read_meassurement();

    }
    // Read Colour Sensor Measurements
      
      if (sensors_interface->p_colour_sensor->is_sensor_available) {
        sensors_interface->p_colour_sensor->read_meassurement();
      }

            // Read Soil Moisture Measurements
      // Soil_Moisture_handle *p_soil_moisture = Soil_Moisture_handle_get();
      if (sensors_interface->p_soil_moisture->is_sensor_available) {
        sensors_interface->p_soil_moisture->read_meassurement();
      }

      // Read brightness Measurements
      if (sensors_interface->p_brightness->is_sensor_available) {
        sensors_interface->p_brightness->read_meassurement();
      }

     
    // volatile uint16_t raw_ay=(uint16_t)y;
    // volatile uint16_t raw_az=(uint16_t)z;

      if (sensors_interface->p_accel->is_sensor_available) {
        sensors_interface->p_accel->read_meassurement();
      }

    // Trigger UI update
      user_interface_handle *p_ui_handle = user_interface_get();
      p_ui_handle->display_new_update = true;


      if (p_ui_handle->display_new_update == true) {
    p_ui_handle->print_new_update();
   
  }
  uint16_t packet_len;

  packet_len=6;
//   volatile float temperature=sensors_interface->p_temp_hum->get_temp_value();
//   volatile float hum=sensors_interface->p_temp_hum->get_hum_value();
  

  volatile uint16_t light=0x24EB;

  volatile int16_t raw16_data;



// //   volatile uint16_t x=sensors_interface->p_accel->get_acc_x_value()*1000.0;
// //   volatile int16_t xx=(int16_t)x;
//      float y=0.0024;
//     y=y*1000;
//      float z =9.81345;
//     z=z*1000;

    
 

  size_t pos = 0;
  //temperatwure
raw16_data=sensors_interface->p_temp_hum->get_raw_temp_value();
tx_buffer[pos++] = (raw16_data>>8) & 0xFF;
tx_buffer[pos++] = (raw16_data) & 0xFF;
//humidity
raw16_data=sensors_interface->p_temp_hum->get_raw_hum_value();
tx_buffer[pos++] = (raw16_data>>8) & 0xFF;
tx_buffer[pos++] = (raw16_data) & 0xFF;
//Colour
//clear
raw16_data=sensors_interface->p_colour_sensor->get_clear();
tx_buffer[pos++] = (raw16_data>>8) & 0xFF;
tx_buffer[pos++] = (raw16_data) & 0xFF;

//red
raw16_data=sensors_interface->p_colour_sensor->get_red();
tx_buffer[pos++] = (raw16_data>>8) & 0xFF;
tx_buffer[pos++] = (raw16_data) & 0xFF;

//green
raw16_data=sensors_interface->p_colour_sensor->get_green();
tx_buffer[pos++] = (raw16_data>>8) & 0xFF;
tx_buffer[pos++] = (raw16_data) & 0xFF;

//blue
raw16_data=sensors_interface->p_colour_sensor->get_blue();
tx_buffer[pos++] = (raw16_data>>8) & 0xFF;
tx_buffer[pos++] = (raw16_data) & 0xFF;

//Brightness
raw16_data=sensors_interface->p_brightness->get_raw_value();
tx_buffer[pos++] = (raw16_data>>8) & 0xFF;
tx_buffer[pos++] = (raw16_data) & 0xFF;

//Soil Moisture
//raw16_data=sensors_interface->p_soil_moisture->get_raw_value();
raw16_data=548;
tx_buffer[pos++] = (raw16_data>>8) & 0xFF;
tx_buffer[pos++] = (raw16_data) & 0xFF;

//ax
raw16_data=sensors_interface->p_accel->get_acc_x_value()*1000.0;
tx_buffer[pos++] = (raw16_data>>8) & 0xFF;
tx_buffer[pos++] = (raw16_data) & 0xFF;

//ay
raw16_data=sensors_interface->p_accel->get_acc_y_value()*1000.0;
tx_buffer[pos++] = (raw16_data>>8) & 0xFF;
tx_buffer[pos++] = (raw16_data) & 0xFF;

//az
raw16_data=sensors_interface->p_accel->get_acc_z_value()*1000.0;
tx_buffer[pos++] = (raw16_data>>8) & 0xFF;
tx_buffer[pos++] = (raw16_data) & 0xFF;



//Latitude
volatile float latitude=40.40120;
volatile float longitud=-3.63330;

//validate if gps has gotten a valid measure. If not send a fixed location
if(sensors_interface->p_gps->is_sensor_available && sensors_interface->p_gps->is_measurement_valid()){
longitud=sensors_interface->p_gps->get_longitude();
latitude=sensors_interface->p_gps->get_latitude();
}else{
latitude=40.40120;
longitud=-3.63330;
}


  
tx_buffer[pos++] = ((*(uint32_t *) &latitude) >> 24) & 0xff;
tx_buffer[pos++] = ((*(uint32_t *) &latitude) >> 16) & 0xff;
tx_buffer[pos++] = ((*(uint32_t *) &latitude) >> 8) & 0xff;
tx_buffer[pos++] = (*(uint32_t *) &latitude) & 0xff;
//Longitude


tx_buffer[pos++] = ((*(uint32_t *) &longitud) >> 24) & 0xff;
tx_buffer[pos++] = ((*(uint32_t *) &longitud) >> 16) & 0xff;
tx_buffer[pos++] = ((*(uint32_t *) &longitud) >> 8) & 0xff;
tx_buffer[pos++] = (*(uint32_t *) &longitud) & 0xff;
  
return pos;

}


/**
 * Default and configured device EUI, application EUI and application key
 */
static const uint8_t DEFAULT_DEV_EUI[] = {0x40, 0x39, 0x32, 0x35, 0x59, 0x37, 0x91, 0x94};
static uint8_t DEV_EUI[] = {0x73, 0x39, 0x32, 0x35, 0x59, 0x37, 0x91, 0x94};
static uint8_t APP_EUI[] = {0x70, 0xb3, 0xd5, 0x7e, 0xd0, 0x00, 0xfc, 0x4d};
static uint8_t APP_KEY[] = {0xf3, 0x1c, 0x2e, 0x8b, 0xc6, 0x71, 0x28, 0x1d,
                            0x51, 0x16, 0xf0, 0x8f, 0xf0, 0xb7, 0x92, 0x8f};

/**
 * Entry point for application
 */

int main(void)
{


    //User custom Code
                                                     // for app funtionality
    app_init(); // Int all app classes an interfaces

    // sensors_thread.start(Thread_Sensor_meassurement); // Decleare another threat

    payload_packaging();
    

    

    //
    printf("\r\n*** Sensor Networks @ ETSIST, UPM ***\r\n"
           "   Mbed (v%d.%d.%d) LoRaWAN Project\r\n",
           MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);

    printf("\r\n DEV_EUI: ");
    for (int i = 0; i < sizeof(DEV_EUI); ++i) printf("%02x", DEV_EUI[i]);
    printf("\r\n APP_EUI: ");
    for (int i = 0; i < sizeof(APP_EUI); ++i) printf("%02x", APP_EUI[i]);
    printf("\r\n APP_KEY: ");
    for (int i = 0; i < sizeof(APP_KEY); ++i) printf("%02x", APP_KEY[i]);
    printf("\r\n");

    if (!memcmp(DEV_EUI, DEFAULT_DEV_EUI, sizeof(DEV_EUI))) {
        printf("\r\n *** You are using the default device EUI value!!! *** \r\n");
        printf("Please, change it to ensure that the device EUI is unique \r\n");
        return -1;
    }

    // setup tracing
    setup_trace();

    // stores the status of a call to LoRaWAN protocol
    lorawan_status_t retcode;

    // Initialize LoRaWAN stack
    if (lorawan.initialize(&ev_queue) != LORAWAN_STATUS_OK) {
        printf("\r\n LoRa initialization failed! \r\n");
        return -1;
    }

    printf("\r\n Mbed LoRaWANStack initialized \r\n");

    // prepare application callbacks
    callbacks.events = mbed::callback(lora_event_handler);
    lorawan.add_app_callbacks(&callbacks);

    // Set number of retries in case of CONFIRMED messages
    if (lorawan.set_confirmed_msg_retries(CONFIRMED_MSG_RETRY_COUNTER)
            != LORAWAN_STATUS_OK) {
        printf("\r\n set_confirmed_msg_retries failed! \r\n\r\n");
        return -1;
    }

    printf("\r\n CONFIRMED message retries : %d \r\n",
           CONFIRMED_MSG_RETRY_COUNTER);

    // Enable adaptive data rate
    if (lorawan.enable_adaptive_datarate() != LORAWAN_STATUS_OK) {
        printf("\r\n enable_adaptive_datarate failed! \r\n");
        return -1;
    }

    printf("\r\n Adaptive data  rate (ADR) - Enabled \r\n");

    lorawan_connect_t connect_params;
    connect_params.connect_type = LORAWAN_CONNECTION_OTAA;
    connect_params.connection_u.otaa.dev_eui = DEV_EUI;
    connect_params.connection_u.otaa.app_eui = APP_EUI;
    connect_params.connection_u.otaa.app_key = APP_KEY;
    connect_params.connection_u.otaa.nb_trials = 3;

    retcode = lorawan.connect(connect_params);

    if (retcode == LORAWAN_STATUS_OK ||
            retcode == LORAWAN_STATUS_CONNECT_IN_PROGRESS) {
    } else {
        printf("\r\n Connection error, code = %d \r\n", retcode);
        return -1;
    }

    printf("\r\n Connection - In Progress ...\r\n");

    // while (true) {
    //   app_state_control();
    // }

    // make your event queue dispatching events forever
    ev_queue.dispatch_forever();

// MBED_ASSERT("Start meassuring");

    

    return 0;
}

/**
 * Sends a message to the Network Server
 */
static void send_message()
{
    uint16_t packet_len;
    int16_t retcode;
    int32_t sensor_value;

    // if (ds1820.begin()) {
    //     ds1820.startConversion();
    //     sensor_value = ds1820.read();
    //     printf("\r\n Dummy Sensor Value = %d \r\n", sensor_value);
    //     ds1820.startConversion();
    // } else {
    //     printf("\r\n No sensor found \r\n");
    //     return;
    // }

//     //Retrive meassurements
//     Sensors_interface *sensors_interface = sensors_interface_get();
//     sensors_interface=sensors_interface_get();

//     if (sensors_interface->p_temp_hum->is_sensor_available) {
//         sensors_interface->p_temp_hum->read_meassurement();
//     }
// if (sensors_interface->p_gps->is_sensor_available) {
//     sensors_interface->p_gps->read_meassurement();

//     }

//     // Trigger UI update
//       user_interface_handle *p_ui_handle = user_interface_get();
//       p_ui_handle->display_new_update = true;


//       if (p_ui_handle->display_new_update == true) {
//     p_ui_handle->print_new_update();
   
//   }

  packet_len=(uint16_t)payload_packaging();
//   volatile float temperature=sensors_interface->p_temp_hum->get_raw_temp_value();
//   temperature= sensors_interface->p_temp_hum->get_temp_value()*10000;
//   volatile int16_t raw16_data;
//   raw16_data=sensors_interface->p_temp_hum->get_raw_temp_value();
//  *(tx_buffer)=(raw16_data>>8) & 0xFF;
//  *(tx_buffer+1)=(raw16_data) & 0xFF;


    // packet_len = snprintf((char *) tx_buffer, sizeof(tx_buffer),
    //                       "Dummy Sensor Value is %d", (int32_t)sensors_interface->p_temp_hum->get_temp_value());

    retcode = lorawan.send(MBED_CONF_LORA_APP_PORT, tx_buffer, packet_len,
                           MSG_UNCONFIRMED_FLAG);

    if (retcode < 0) {
        retcode == LORAWAN_STATUS_WOULD_BLOCK ? printf("send - WOULD BLOCK\r\n")
        : printf("\r\n send() - Error code %d \r\n", retcode);

        if (retcode == LORAWAN_STATUS_WOULD_BLOCK) {
            //retry in 3 seconds
            if (MBED_CONF_LORA_DUTY_CYCLE_ON) {
                ev_queue.call_in(3s, send_message);
            }
        }
        return;
    }

    printf("\r\n %d bytes scheduled for transmission \r\n", retcode);
    memset(tx_buffer, 0, sizeof(tx_buffer));
}

/**
 * Receive a message from the Network Server
 */
static void receive_message()
{
    uint8_t port;
    int flags;
    int16_t retcode = lorawan.receive(rx_buffer, sizeof(rx_buffer), port, flags);

    if (retcode < 0) {
        printf("\r\n receive() - Error code %d \r\n", retcode);
        return;
    }

    printf(" RX Data on port %u (%d bytes): ", port, retcode);
    for (uint8_t i = 0; i < retcode; i++) {
        printf("%02x ", rx_buffer[i]);
    }
    printf("\r\n");


    user_interface_handle *p_ui_handle = user_interface_get();

    if(rx_buffer[0]==0x52 && rx_buffer[1]==0x65 && rx_buffer[2]==0x64){
//Red CMD
p_ui_handle->set_RGB_colour(RGB_Red);

    }else if(rx_buffer[0]==0x47 && rx_buffer[1]==0x72 && rx_buffer[2]==0x65 && rx_buffer[3]==0x65 && rx_buffer[4]==0x6E){
//Green CMD
p_ui_handle->set_RGB_colour(RGB_Green);
    }else if(rx_buffer[0]==0x4F && rx_buffer[1]==0x46 && rx_buffer[2]==0x46){
//OFF CMD

p_ui_handle->set_RGB_colour(RGB_None);
    }else{
      printf(" CMD not registered \r\n");  
    }
    
    memset(rx_buffer, 0, sizeof(rx_buffer));
}

/**
 * Event handler
 */
static void lora_event_handler(lorawan_event_t event)
{
    switch (event) {
        case CONNECTED:
            printf("\r\n Connection - Successful \r\n");
            if (MBED_CONF_LORA_DUTY_CYCLE_ON) {
                send_message();
            } else {
                ev_queue.call_every(TX_TIMER, send_message);
            }

            break;
        case DISCONNECTED:
            ev_queue.break_dispatch();
            printf("\r\n Disconnected Successfully \r\n");
            break;
        case TX_DONE:
            printf("\r\n Message Sent to Network Server \r\n");
            if (MBED_CONF_LORA_DUTY_CYCLE_ON) {
                send_message();
            }
            break;
        case TX_TIMEOUT:
        case TX_ERROR:
        case TX_CRYPTO_ERROR:
        case TX_SCHEDULING_ERROR:
            printf("\r\n Transmission Error - EventCode = %d \r\n", event);
            // try again
            if (MBED_CONF_LORA_DUTY_CYCLE_ON) {
                send_message();
            }
            break;
        case RX_DONE:
            printf("\r\n Received message from Network Server \r\n");
            receive_message();
            break;
        case RX_TIMEOUT:
        case RX_ERROR:
            printf("\r\n Error in reception - Code = %d \r\n", event);
            break;
        case JOIN_FAILURE:
            printf("\r\n OTAA Failed - Check Keys \r\n");
            break;
        case UPLINK_REQUIRED:
            printf("\r\n Uplink required by NS \r\n");
            if (MBED_CONF_LORA_DUTY_CYCLE_ON) {
                send_message();
            }
            break;
        default:
            MBED_ASSERT("Unknown Event");
    }
}

// EOF
