// ===========================================================================
// sensors_handle.cpp
// ---------------------------------------------------------------------------
// Description:
// Implementation of the Sensors Handle class.
// Author: Mariana Arvelaiz
// Date: Nov. 2024
// Version: 1.0
// ===========================================================================

#include "sensors_handle.h"
// #include "app.h"

#include <cstring>

#include "user_interface.h"

//#include "string.h"

AnalogIn soil_moisture_pin(A0);
AnalogIn brightness_sensor_signal_pin(A2);
//DigitalOut temperature_sensor_VCC(PB_6);
//DigitalOut temperature_sensor_VCC(D15);
Sensors_interface *sensors_interface;

Sensors_interface::Sensors_interface()
    : start_messurement_cycle(false), messurements_finished(false), counter(0) {
}

Sensors_interface *get_interface() { return sensors_interface; }

Sensors_interface *sensors_interface_get() { return sensors_interface; }

// void Sensors_interface::sensors_interface_get_stats() {

//   user_interface_handle *p_ui = user_interface_get();

//   memset(p_ui->buffer, '\0', sizeof(p_ui->buffer));

//   sprintf(p_ui->buffer, "\n--STATS-- \n");

//   if (sensors_interface->p_temp_hum->is_sensor_available) {

//     sensors_interface->p_temp_hum->get_stats_to_print(
//         sensors_interface->measurements);
//     sprintf(p_ui->buffer + strlen(p_ui->buffer),
//             "Temperature and Humidity Stats \n Temp Mean: %.2f°C | Temp "
//             "Min:%.2f°C | Temp Max: %.2f°C\nHumidity Mean: %.2f%% | "
//             "Humidity,Min: %.2f%% | Humidity Max: %.2f%%\n",
//             sensors_interface->p_temp_hum->get_mean_temp_value(),
//             sensors_interface->p_temp_hum->get_min_temp_value(),
//             sensors_interface->p_temp_hum->get_max_temp_value(),
//             sensors_interface->p_temp_hum->get_mean_hum_value(),
//             sensors_interface->p_temp_hum->get_min_hum_value(), sensors_interface->p_temp_hum->get_max_hum_value());
//     //    memcpy(p_ui->buffer +strlen(p_ui->buffer),
//     //        sensors_interface->measurements,
//     //        strlen(sensors_interface->measurements) + 1);

//   } else {
//     char msj[] = "TEMP& HUM STATS N/A \n";
//     // memcpy(measurements, msj, strlen(msj) + 1);
//     memcpy(p_ui->buffer + strlen(p_ui->buffer), msj, strlen(msj) + 1);
//   }


//   if (sensors_interface->p_soil_moisture->is_sensor_available) {
//     // memcpy(measurements + strlen(measurements),
//     //        sensors_interface->p_soil_moisture->get_data_to_print(),
//     //        strlen(sensors_interface->p_soil_moisture->get_data_to_print()) +
//     //        1);

//     sprintf(p_ui->buffer + strlen(p_ui->buffer),
//             "Soil Moisture Statistics - Mean: %.2f%% | Min: %.2f%% | Max: %.2f%%\n",
//             sensors_interface->p_soil_moisture->get_mean_value(),
//             p_soil_moisture->get_min_value(),
//             p_soil_moisture->get_max_value());
//   } else {
//     char msj[] = "Soil Moisture Stats N/A \n";
//     // memcpy(measurements + strlen(measurements) + 1, msj, strlen(msj) + 1);
//     memcpy(p_ui->buffer+ strlen(p_ui->buffer), msj, strlen(msj) + 1);
//   }


//   if (sensors_interface->p_colour_sensor->is_sensor_available) {

//       sensors_interface->p_colour_sensor->evaluate_dominant_colour_in_period();
//     // memcpy(measurements + strlen(measurements),
//     //        sensors_interface->p_soil_moisture->get_data_to_print(),
//     //        strlen(sensors_interface->p_soil_moisture->get_data_to_print()) +
//     //        1);

//     if(sensors_interface->p_colour_sensor->dominant_colour==GREEN){
//         sprintf(p_ui->buffer + strlen(p_ui->buffer),
//             "Dominant Colour Stats  %s\n",
//             DOMINANT_COLOUR_GREEN);

//     }else if(sensors_interface->p_colour_sensor->dominant_colour==BLUE){
//         sprintf(p_ui->buffer + strlen(p_ui->buffer),
//             "Dominant Colour Stats  %s\n",
//             DOMINANT_COLOUR_BLUE);

//     }else if (sensors_interface->p_colour_sensor->dominant_colour==RED){
//         sprintf(p_ui->buffer + strlen(p_ui->buffer),
//             "Dominant Colour Stats  %s\n",
//             DOMINANT_COLOUR_RED);

//     }else {
//     sprintf(p_ui->buffer + strlen(p_ui->buffer),
//             "Dominant Colour Stats  %s\n",
//             DOMINANT_COLOUR_NONE);
//     }
    

    
//   } 


//   if (sensors_interface->p_accel->is_sensor_available) {

//       sprintf(p_ui->buffer + strlen(p_ui->buffer),
//             "Accelerometer Stats- Min: X: %.5f, Y: %.5f, Z: %.5f | Max: X: %.5f, Y: %.5f, Z: %.5f\n",
        
//             sensors_interface->p_accel->get_min_acc_x_value(),
//             sensors_interface->p_accel->get_min_acc_y_value(),
//             sensors_interface->p_accel->get_min_acc_z_value(),
//             sensors_interface->p_accel->get_max_acc_x_value(),
//             sensors_interface->p_accel->get_max_acc_y_value(),
//             sensors_interface->p_accel->get_max_acc_z_value());

//   }

// //   }else {
// //   char msj[] = "ACCELEROMETER N/A \n";
// //     memcpy(p_ui->buffer+ strlen(p_ui->buffer), msj, strlen(msj) + 1);
// //   }



// }

char *Sensors_interface::sensors_interface_get_last_meassurements() {

//   if (sensors_interface->p_soil_moisture->is_sensor_available) {
    // memcpy(measurements + strlen(measurements),
    //        sensors_interface->p_soil_moisture->get_data_to_print(),
    //        strlen(sensors_interface->p_soil_moisture->get_data_to_print()) +
    //        1);

//     memcpy(measurements,
//            sensors_interface->p_soil_moisture->get_data_to_print(),
//            strlen(sensors_interface->p_temp_hum->get_data_to_print()) + 1);
//   } else {
//     char msj[] = "Soil Moisture N/A \n";
//     // memcpy(measurements + strlen(measurements) + 1, msj, strlen(msj) + 1);
//     memcpy(measurements, msj, strlen(msj) + 1);
//   }

  // Brightness Sensor
  Brightness_Sensor_handle *p_brightness = Brightness_Sensor_handle_get();
  if (sensors_interface->p_brightness->is_sensor_available) {
    memcpy(measurements,
           sensors_interface->p_brightness->get_data_to_print(),
           strlen(sensors_interface->p_brightness->get_data_to_print()) + 1);
  } else {
    char msj[] = "Brightness N/A \n";
    memcpy(measurements, msj, strlen(msj) + 1);
  }

  // GPS Sensor

//   GPS_handle *p_gps = GPS_handle_get();

//   if (sensors_interface->p_gps->is_sensor_available) {
//     memcpy(measurements + strlen(measurements),
//            sensors_interface->p_gps->get_data_to_print(),
//            strlen(sensors_interface->p_gps->get_data_to_print()) + 1);
//   }

  GPS_handle *p_gps = GPS_handle_get();

  if (sensors_interface->p_gps->is_sensor_available) {
    memcpy(measurements+ strlen(measurements),
           sensors_interface->p_gps->get_data_to_print(),
           strlen(sensors_interface->p_gps->get_data_to_print()) + 1);
  }

  // Colour Sensor
  Colour_sensor_handle *p_colour_sensor = Colour_sensor_handle_get();
  if (sensors_interface->p_colour_sensor->is_sensor_available) {
    memcpy(measurements + strlen(measurements),
           sensors_interface->p_colour_sensor->get_data_to_print(),
           strlen(sensors_interface->p_colour_sensor->get_data_to_print()) + 1);
  } else {
    char msj[] = "COLOUR N/A \n";
    memcpy(measurements + strlen(measurements) + 1, msj, strlen(msj) + 1);
  }

  // Accelerometer Sensor
//   Accelerometer_handle *p_accelerometer = Accelerometer_handle_get();
//   if (sensors_interface->p_accel->is_sensor_available) {
//     memcpy(measurements + strlen(measurements),
//            sensors_interface->p_accel->get_data_to_print(),
//            strlen(sensors_interface->p_accel->get_data_to_print()) + 1);
//   } else {
//     char msj[] = "ACCELEROMETER N/A \n";
//     memcpy(measurements + strlen(measurements) + 1, msj, strlen(msj) + 1);
//   }

    // Temperature & HUM
    Temp_Hum_handle *p_temp_hum = Temp_Hum_handle_get();
    if (sensors_interface->p_temp_hum->is_sensor_available) {
      // memcpy(measurements,
      sprintf(measurements + strlen(measurements), "TEMP/HUM: Temp: %.2f°C   Rel. Hum: %.2f %% \n\n", sensors_interface->p_temp_hum->get_temp_value(), sensors_interface->p_temp_hum->get_hum_value());
    } else {
      char msj[] = "TEMP& HUM N/A \n";
      // memcpy(measurements, msj, strlen(msj) + 1);
      memcpy(measurements + strlen(measurements) + 1, msj, strlen(msj) + 1);
    }

  return measurements;
}

void sensors_interface_init(I2C *p_i2c_interface, BufferedSerial *serial_port) {
  // Init the pointer

  sensors_interface = new Sensors_interface();
  sensors_interface->_p_i2c_interface = p_i2c_interface;
  sensors_interface->_serial_port = serial_port;

  // Code to configure the sensor for first measurement if needed and init the
  // class
  Temp_Hum_handle_init(TEMP_HUM_ADDR);
  sensors_interface->p_temp_hum = Temp_Hum_handle_get();
  sensors_interface->p_temp_hum->make_meassurement();
  //   Temp_Hum_handle *p_temp_hum = Temp_Hum_handle_get();
  //   bool correct_mea = p_temp_hum->make_meassurement();

  Colour_sensor_handle_init(COLOUR_ADDR);
  sensors_interface->p_colour_sensor = Colour_sensor_handle_get();
  sensors_interface->p_colour_sensor->make_meassurement();
 

//   // Initialize Accelerometer Sensor
//   Accelerometer_handle_init(ACCEL_ADDR);
//   sensors_interface->p_accel = Accelerometer_handle_get();
//   sensors_interface->p_accel->make_meassurement();

  //   Accelerometer_handle *p_accelerometer = Accelerometer_handle_get();
  //   correct_mea = p_accelerometer->make_meassurement();

  // Initialize GPS Sensor
  GPS_handle_init();
  sensors_interface->p_gps = GPS_handle_get();
  sensors_interface->p_gps->make_meassurement();

//   //   GPS_handle *p_gps = GPS_handle_get();
//   //   correct_mea = p_gps->make_meassurement();

//   // Init the Soil Moisture Sensor and class
//   Soil_Moisture_handle_init(soil_moisture_pin);
//   sensors_interface->p_soil_moisture = Soil_Moisture_handle_get();
//   sensors_interface->p_soil_moisture->make_meassurement();


  // Int Brightness_Sensor_handle
  Brightness_Sensor_handle_init(&brightness_sensor_signal_pin);
  sensors_interface->p_brightness = Brightness_Sensor_handle_get();
  sensors_interface->p_brightness->make_meassurement();

}

void Thread_Sensor_meassurement() {

  while (true) {


    if (sensors_interface->start_messurement_cycle) {

  
      // call funtion to make temperature meassurement

      // Temp_Hum_handle *p_temp_hum = Temp_Hum_handle_get();
      if (sensors_interface->p_temp_hum->is_sensor_available) {
        sensors_interface->p_temp_hum->read_meassurement();
      }
      
      // GPS
      // GPS_handle *p_gps = GPS_handle_get();
      sensors_interface->p_gps->read_meassurement();

     

      // Reset flags
      sensors_interface->start_messurement_cycle = false;
      sensors_interface->messurements_finished = true;
      // Trigger UI update
      user_interface_handle *p_ui_handle = user_interface_get();
      p_ui_handle->display_new_update = true;


      if (p_ui_handle->display_new_update == true) {
    p_ui_handle->print_new_update();
   
  }
    }
  }
}