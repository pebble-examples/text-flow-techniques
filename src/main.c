#include <pebble.h>

#include "windows/demo_selection_window.h"

static void init() {
  demo_selection_window_push();
}

static void deinit() { }

int main() {
  init();
  app_event_loop();
  deinit();
}
