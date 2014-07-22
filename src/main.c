#include <pebble.h>
#include "SimpleFace.h"

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}
