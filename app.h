// ===========================================================================
// sensors_handle.h
// ---------------------------------------------------------------------------
// Description:
//   Header file for the app class and sesors interface funtion.Works as the
//   control center(supervisor)
//
// Author: Mariana Arvelaiz
// Date: Nov. 2024
// Version: 1.0
// ===========================================================================
#ifndef APP_H
#define APP_H

#include <cstdint>

#define ADVANCE_MODE_ON 1
#define TEST_MODE_UPDATE_TIME_US 2000000
#define NORMAL_MODE_UPDATE_TIME_US 10000000
#define NORMAL_MODE_STATS_PERIOD_S 3600
//#define NORMAL_MODE_MEASSUREMENT_TOSHOW_STATS
//NORMAL_MODE_MEASSUREMENT_PERHOUR/(NORMAL_MODE_UPDATE_TIME_US/1000000)

#define NORMAL_MODE_MEASSUREMENT_TOSHOW_STATS 3
#define ADVANCE_MODE_UPDATE_TIME_US NORMAL_MODE_UPDATE_TIME_US

typedef enum {
  IDLE = 0,                // Not doing anything
  Start = 1,               // time to Trigger all the meassurements
  Check_meassurements = 2, // At the middle of the meassurment cycle check and
                           // retrive all the meassurements
  Show_Meassurement = 3,   // Time to show new meassurements
} App_cycle_state;

typedef enum {
  TEST_MODE = 0,   // Control cycle of 2s
  NORMAL_MODE = 1, // Control cycle of 30s
  ADVANCE_MODE =
      3, // COntrol cycle of 30s but sleep mode for the MCUs uses to save power
} App_mode;

class App {

public:
  App();
  bool flag_ready_to_sleep; // true when not task is pending. False if the
                            // app_state_control funtion need to bee executed at
                            // least once before activate sleep mode
  bool show_stats;
  int counter;
  bool flag_change_mode;
  App_cycle_state get_status();
  void set_status(App_cycle_state new_status);
  App_mode get_mode();
  void change_to_next_mode();
  void set_mode_advance_mode();

private:
  App_cycle_state _status;
  App_mode _mode;
};

void app_init();
void app_state_control(); // This funtion runs diferent protocol over the system
                          // to validate behavior
App *app_get();

#endif // APP_H