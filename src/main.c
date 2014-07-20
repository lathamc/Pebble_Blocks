#include <pebble.h>

Window *window;
TextLayer *hour_layer;
TextLayer *min_layer;
TextLayer *am_layer;
TextLayer *date_layer;
InverterLayer *inverter_layer;

void handle_hourchanges(struct tm *tick_time, TimeUnits units_changed){
  /* Updates them hour and am/pm because the layers are on the same horizontal plane */
  static char hour_buffer[6];
  static char am_buffer[4];
  
  strftime(hour_buffer, sizeof(hour_buffer), "%I", tick_time);
  text_layer_set_text(hour_layer, hour_buffer);
  
  strftime(am_buffer, sizeof(am_buffer), "%p", tick_time);
  text_layer_set_text(am_layer, am_buffer);
}

void handle_timechanges(struct tm *tick_time, TimeUnits units_changed) {
  /* Updates the minute and date because the layers are on the same horizontal plane */
  static char min_buffer[6];
  static char date_buffer[10];
  
  strftime(min_buffer, sizeof(min_buffer), "%M", tick_time);
  text_layer_set_text(min_layer, min_buffer);
  strftime(date_buffer, sizeof(date_buffer), "%a\n%b %e", tick_time);
  text_layer_set_text(date_layer, date_buffer);
  
  if(units_changed & HOUR_UNIT)
  {
    handle_hourchanges(tick_time, units_changed);
  }
}

void handle_init(void) {
  window = window_create();
  
  GFont abadi_62 = 
    fonts_load_custom_font(resource_get_handle(RESOURCE_ID_ARIAL_NARROW_BOLD_69));
  GFont abadi_24 =
    fonts_load_custom_font(resource_get_handle(RESOURCE_ID_ABADI_CONDENSED_24));
  
  //HOUR LAYER
  hour_layer = text_layer_create(GRect(0,0, 82, 84));
  text_layer_set_font(hour_layer, abadi_62);
  text_layer_set_text_alignment(hour_layer, GTextAlignmentRight);
  
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(hour_layer));
  
  //MINUTE LAYER
  min_layer = text_layer_create(GRect(64, 80, 80, 88));
  text_layer_set_font(min_layer, abadi_62);
  text_layer_set_text_alignment(min_layer, GTextAlignmentLeft);
  
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(min_layer));
  
  //AM PM LAYER
  am_layer = text_layer_create(GRect(82, 28, 52, 36));
  text_layer_set_font(am_layer, abadi_24);
  text_layer_set_text_alignment(am_layer, GTextAlignmentCenter);
  
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(am_layer));
  
  //DATE LAYER
  date_layer = text_layer_create(GRect(0, 98, 64, 84));
  text_layer_set_font(date_layer, abadi_24);
  text_layer_set_text_alignment(date_layer, GTextAlignmentCenter);
  
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(date_layer));
  
  //INVERTER LAYER
  inverter_layer = inverter_layer_create(GRect(0, 0, 144, 168));
  layer_add_child(window_get_root_layer(window), 
                  inverter_layer_get_layer(inverter_layer));
  
  //TIME CHANGES
  time_t now = time(NULL);
  handle_timechanges(localtime(&now), MINUTE_UNIT);
  handle_hourchanges(localtime(&now), HOUR_UNIT);
  tick_timer_service_subscribe(MINUTE_UNIT, handle_timechanges);  
  
  window_stack_push(window, true);
}

void handle_deinit(void) {
  text_layer_destroy(hour_layer);
  text_layer_destroy(min_layer);
  text_layer_destroy(am_layer);
  text_layer_destroy(date_layer);
  inverter_layer_destroy(inverter_layer);
  
  window_destroy(window);
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}
