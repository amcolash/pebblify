#include <pebble.h>
#include "main_window.h"
void handle_init(void) {
  show_main_window(); 
}

void handle_deinit(void) {
  hide_main_window(); 
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}