// ===========================================================================
// GPS_handle.cpp
// ---------------------------------------------------------------------------
// Description:
// Implementation of the GPS_handle class.
// Author: Mariana Arvelaiz
// Date: Nov. 2024
//  Last updated: Dec. 2025
// Version: 1.0
// ===========================================================================

#include "gps_handle.h"
#include "sensors_handle.h"
#include "user_interface.h"
#include <cstdio>
#include <stdlib.h>
#include <string.h>

GPS_handle *gps_handle;

// Class methods

GPS_handle::GPS_handle()
    : is_sensor_available(false), hour(0), minutes(0), seconds(0) {}

char *GPS_handle::get_data_to_print() {
  // This funtion configure the format in which the sensors reading is going to
  // be printed
  snprintf(formated_data, sizeof(formated_data),
           "GPS: #Sats: %d, Lat(UTC): %.7f,%c Long(UTC): -%.7f %c, Altitude: %.1fm, "
           "GPS_time: %02d:%02d:%02d\n",
           num_satellites, latitude, latDirection, longitude,longDirection, altitude, hour, minutes,
           seconds);
  return formated_data;
}

const int timezone_offset =
    1; // UTC+1 Invierno, en verano const int timezone_offset = 2

// Función para convertir hora UTC a hora local
void convertToLocalTime(int &hour, int &minute) {
  hour += timezone_offset;
  if (hour < 0) {
    hour += 24; // Ajustar si es necesario
  }
  hour %= 24; // Asegurarse de que la hora esté en el rango [0, 23]
}

void GPS_handle::parseGPGGA(char *gpgga) {

  char *token = strtok(gpgga, ",");
  int fieldIndex = 0;

  char gps_time[9];

  // if (sscanf(gpgga,
  // "$GPGGA,%2d%2d%2d.%*f,%f,%c,%f,%c,%d,%d,%f,%f,%*c,%*f,%*f,%*f",
  //         &hour, &minute, &second, &latitude, &latDirection,
  //         &longitude, &longDirection, &fixQuality, &numSats, &hdop,
  //         &altitude) >= 9) {

  //     convertToLocalTime(hour, minute);

  //     if (latDirection == 'S') {
  //         latitude = -latitude; // Latitud sur es negativa
  //     }
  //     if (longDirection == 'W') {
  //         longitude = -longitude; // Longitud oeste es negativa
  //     }
  //     printf("GPS: #Sats: %d, Lat(UTC): %.7f, Long(UTC): %.7f, Altitude:
  //     %.1fm, GPS_time: %02d:%02d:%02d\n",
  //         numSats, latitude / 100.0, longitude / 100.0, altitude, hour,
  //         minute, second);

  // }
  //   char parallel, meridian, measurement;

  //   while (token != NULL) {
  //     switch (fieldIndex) {
  //     case 1:
  //       if (!(strncmp(token, "", 1)) && *(token) != NULL) {
  //         snprintf(gps_time, sizeof(gps_time), "%s:%s:%s", token, token + 2,
  //                  token + 4);
  //       }

  //       break;
  //     case 2:
  //       latitude = atof(token) / 100;
  //       break;
  //     case 3:
  //       parallel = *(token);

  //       break;
  //     case 4:
  //       longitude = atof(token) / 100;
  //       break;
  //     case 5:
  //       meridian = *(token);
  //       break;
  //     case 7:
  //       num_satellites = atoi(token);
  //       break;
  //     case 9:
  //       altitude = atof(token);
  //       break;
  //     case 10:
  //       measurement = token[0] + 32;
  //       break;
  //     }

  //     token = strtok(NULL, ",");
  //     fieldIndex++;
  //   }

  // TODO: checkand trasnforn lat and long to degrees. Also evaluate the sign
}

bool GPS_handle::make_meassurement() { return false; }

void GPS_handle::read_meassurement() {

  Sensors_interface *p_comm_handle = sensors_interface_get();

  if (p_comm_handle->_serial_port->readable()) {
    is_sensor_available = true;
    // Leer hasta que encuentres un carácter de nueva línea o final de buffer
    size_t n = p_comm_handle->_serial_port->read(data, sizeof(data) - 1);
    data[n] = '\0'; // Asegurarte de terminar la cadena

    // Parsing the string
    if (strstr(data, "$GPGGA")) {
      char *reading_pointer = strstr(data, "$GPGGA");

      char *token = strtok((char *)reading_pointer, ",");
      int fieldIndex = 0;

      while (token != NULL) {
        switch (fieldIndex) {
        case 1:
          hour = (token[0] - '0') * 10 + (token[1] - '0');
          minutes = (token[2] - '0') * 10 + (token[3] - '0');
          seconds = (token[4] - '0') * 10 + (token[5] - '0');
          break;
        case 2:
          latitude = atof(token) / 100;
          break;
        case 3:
          latDirection = token[0];
          if (latDirection == 'S') {
            latitude = -latitude;
          }

          break;
        case 4:
          longitude = atof(token) / 100;
          break;
        case 5:
          longDirection = token[0];
          longitude = -longitude;
          if (longDirection == 'W') {
            longitude = -longitude; //
          }
          break;

        case 7:
          num_satellites = atoi(token);
          break;
        case 9:
          altitude = atof(token);
          break;
        }

        token = strtok(NULL, ",");
        fieldIndex++;
      }
    }
  }
}
// External Funtions

GPS_handle *GPS_handle_get() { return gps_handle; }

void GPS_handle_init() {
  // Allocate the pointer
  gps_handle = new GPS_handle();
  memset(gps_handle->formated_data, '\0', sizeof(gps_handle->formated_data));
  gps_handle->is_sensor_available=true;
}