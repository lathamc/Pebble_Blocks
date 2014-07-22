#include <pebble.h>
#include "SimpleFace.h"

Window *window;
TextLayer *hour_layer;
TextLayer *min_layer;
TextLayer *am_layer;
TextLayer *date_layer;
TextLayer *battery_layer;

/* Checks user settings regarding 12/24 hour time */
bool clock_is_24h_style(void);

/* Function to get the battery state whenever called */
static void update_battery_state(BatteryChargeState charge_state)
{
  double battery_percent = charge_state.charge_percent * 0.01; //Returns the battery level as a percentage
  layer_set_frame(text_layer_get_layer(battery_layer), GRect(0, 167, (int)(battery_percent * 144), 1)); //Resizes the battery layer
}
void handle_hourchanges(struct tm *tick_time, TimeUnits units_changed){
  /* Updates them hour and am/pm because the layers are on the same horizontal plane */
  static char hour_buffer[6];
  static char am_buffer[4];
  
  /* If the user set time to 24 hour style, then returns the 24 hour time (00, 01, ... 24).  If user set time to 12 hour style,
  will return the 12 hour time (01, 02, .... 12).  Fills hour_buffer and am_buffer with the proper text. */
  if(clock_is_24h_style() == true){
    strftime(hour_buffer, sizeof(hour_buffer), "%H", tick_time);
    strftime(am_buffer, sizeof(am_buffer), "  ", tick_time); //Clears the am/pm text field because of 24 hour time
  }
  else{
    strftime(hour_buffer, sizeof(hour_buffer), "%I", tick_time); 
    strftime(am_buffer, sizeof(am_buffer), "%p", tick_time);
  }
  text_layer_set_text(hour_layer, hour_buffer);
  text_layer_set_text(am_layer, am_buffer);
}

/* Handles the change in time.  If only minute changes, only the minute and date layers are updated.  If the hour changes,
the full screen is updated. */
void handle_timechanges(struct tm *tick_time, TimeUnits units_changed) {
  /* Updates the minute and date because the layers are on the same horizontal plane */
  static char min_buffer[6];
  static char date_buffer[10];
  
  strftime(min_buffer, sizeof(min_buffer), "%M", tick_time);
  text_layer_set_text(min_layer, min_buffer);
  strftime(date_buffer, sizeof(date_buffer), "%a\n%b %e", tick_time);
  text_layer_set_text(date_layer, date_buffer);
  
  update_battery_state(battery_state_service_peek());
  
  if(units_changed & HOUR_UNIT)
  {
    handle_hourchanges(tick_time, units_changed);
  }
}

/* Initializes the watchface */
void handle_init(void) {
  window = window_create();
  window_set_background_color(window, GColorBlack);
  
  /* Sets the custom fonts as resources. */
  GFont abadi_62 = 
    fonts_load_custom_font(resource_get_handle(RESOURCE_ID_ARIAL_NARROW_BOLD_69));
  GFont abadi_24 =
    fonts_load_custom_font(resource_get_handle(RESOURCE_ID_ABADI_CONDENSED_24));
  
  //HOUR LAYER
  hour_layer = text_layer_create(GRect(0,0, 82, 84));
  text_layer_set_font(hour_layer, abadi_62);
  text_layer_set_text_alignment(hour_layer, GTextAlignmentRight);
  text_layer_set_background_color(hour_layer, GColorBlack);
  text_layer_set_text_color(hour_layer, GColorWhite);
  
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(hour_layer));
  
  //MINUTE LAYER
  min_layer = text_layer_create(GRect(64, 80, 80, 88));
  text_layer_set_font(min_layer, abadi_62);
  text_layer_set_text_alignment(min_layer, GTextAlignmentLeft);
  text_layer_set_background_color(min_layer, GColorBlack);
  text_layer_set_text_color(min_layer, GColorWhite);
  
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(min_layer));
  
  //AM PM LAYER
  am_layer = text_layer_create(GRect(82, 29, 52, 36));
  text_layer_set_font(am_layer, abadi_24);
  text_layer_set_text_alignment(am_layer, GTextAlignmentCenter);
  text_layer_set_background_color(am_layer, GColorBlack);
  text_layer_set_text_color(am_layer, GColorWhite);
  
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(am_layer));
  
  //DATE LAYER
  date_layer = text_layer_create(GRect(0, 96, 64, 84));  //Manually centered vertically
  text_layer_set_font(date_layer, abadi_24);
  text_layer_set_text_alignment(date_layer, GTextAlignmentCenter);
  text_layer_set_background_color(date_layer, GColorBlack);
  text_layer_set_text_color(date_layer, GColorWhite);
  
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(date_layer));
  
  battery_layer = text_layer_create(GRect(0, 0, 0, 0)); //Creates the battery layer
  
  text_layer_set_background_color(battery_layer, GColorWhite);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(battery_layer));
  
  update_battery_state(battery_state_service_peek()); 
  battery_state_service_subscribe(update_battery_state);
  
  //TIME CHANGES
  /* Call the function to update time upon starting the watchface */
  time_t now = time(NULL);
  
  handle_timechanges(localtime(&now), MINUTE_UNIT);
  handle_hourchanges(localtime(&now), HOUR_UNIT);
  
  /* Every minute, calls the handle_timechanges function */
  tick_timer_service_subscribe(MINUTE_UNIT, handle_timechanges);  
  
  window_stack_push(window, true);
}

void handle_deinit(void) {
  text_layer_destroy(hour_layer);
  text_layer_destroy(min_layer);
  text_layer_destroy(am_layer);
  text_layer_destroy(date_layer);
  text_layer_destroy(battery_layer);
  
  window_destroy(window);
}