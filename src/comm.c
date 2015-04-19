#include <pebble.h>
#include "comm.h"
bool is_init = false; 
void init() {
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
  is_init = true; 
}

bool send_to_phone(int key, char *s_symbol) {
  if(!is_init) init(); 
  if((key == -1)  && (s_symbol == NULL)) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "no data to send");
    return true;
  }
  
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);
  
  if (iter == NULL) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "null iter");
    return false;
  }
  
  Tuplet tuple = (s_symbol == NULL)
                      ? TupletInteger(key, 1)
                      : TupletCString(key, s_symbol);
  dict_write_tuplet(iter, &tuple);
  dict_write_end(iter);

  app_message_outbox_send();
  return true;
}