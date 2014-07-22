#include <pebble.h>
/*
extern Window *window;
extern TextLayer *hour_layer;
extern TextLayer *min_layer;
extern TextLayer *am_layer;
extern TextLayer *date_layer;
extern TextLayer *battery_layer;
*/
  
/*************************************************************************************
Name: clock_is_24h_style
Description: Determines if the user preference is for 24 hour time of 12 hour time.
Input:
    void
Output:
    void
*************************************************************************************/
bool clock_is_24h_style(void);

/*************************************************************************************
Name: update_battery_state
Description: Reads the battery state and outputs a white line at the bottom of the
              Pebble dispay as a battery percentage indicator
Input:
    BatteryChargeState    structure that contains information about the battery,
                          structure is included in the Pebble SDK
Output:
    static void
*************************************************************************************/
static void update_battery_state( BatteryChargeState );

/*************************************************************************************
Name: handle_hourchanges
Description: Changes the hour and am/pm status once called
Input:
    struct tm *     structure pointer, used with tick_time structure that is the
                    time at which the tick event was triggered
    TimeUnits       type of unit that triggered the tick_event
Output:
    void
*************************************************************************************/
void handle_hourchanges(struct tm *, TimeUnits );

/*************************************************************************************
Name: handle_hourchanges
Description: Changes the hour and am/pm status once called
Input:
    struct tm *     structure pointer, used with tick_time structure that is the
                    time at which the tick event was triggered
    TimeUnits       type of unit that triggered the tick_event
Output:
    void
*************************************************************************************/
void handle_timechanges(struct tm *, TimeUnits );

/*************************************************************************************
Name: handle_init
Description: Initializes the watchface when called
Input:
    void
Output:
    void
*************************************************************************************/
void handle_init(void);

/*************************************************************************************
Name: handle_init
Description: Initializes the watchface when called
Input:
    void
Output:
    void
*************************************************************************************/
void handle_deinit(void);