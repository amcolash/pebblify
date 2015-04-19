#include <pebble.h>
#define PBL_PLATFORM_APLITE 1
#include "main_window.h"
#include "comm.h"
#include "netdownload.h"
void handle_init(void) {
  
  show_main_window(); 
}

void handle_deinit(void) {
  netdownload_deinitialize(); // call this to avoid 20B memory leak
  hide_main_window(); 
  comm_deinit();
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}