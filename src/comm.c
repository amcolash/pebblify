#include <pebble.h>
#include "comm.h"
#include "main_window.h"
  
static bool is_init = false; 
static bool s_wasFirstMsg;
static bool s_dataInited;
static char s_string[120];
static char debug_string[64];

//AppSync
static AppSync s_sync;
static uint8_t s_sync_buffer[120];
static char* tok;
static char* fields[3];

char *translate_error(AppMessageResult result) {
  switch (result) {
    case APP_MSG_OK: return "APP_MSG_OK";
    case APP_MSG_SEND_TIMEOUT: return "APP_MSG_SEND_TIMEOUT";
    case APP_MSG_SEND_REJECTED: return "APP_MSG_SEND_REJECTED";
    case APP_MSG_NOT_CONNECTED: return "APP_MSG_NOT_CONNECTED";
    case APP_MSG_APP_NOT_RUNNING: return "APP_MSG_APP_NOT_RUNNING";
    case APP_MSG_INVALID_ARGS: return "APP_MSG_INVALID_ARGS";
    case APP_MSG_BUSY: return "APP_MSG_BUSY";
    case APP_MSG_BUFFER_OVERFLOW: return "APP_MSG_BUFFER_OVERFLOW";
    case APP_MSG_ALREADY_RELEASED: return "APP_MSG_ALREADY_RELEASED";
    case APP_MSG_CALLBACK_ALREADY_REGISTERED: return "APP_MSG_CALLBACK_ALREADY_REGISTERED";
    case APP_MSG_CALLBACK_NOT_REGISTERED: return "APP_MSG_CALLBACK_NOT_REGISTERED";
    case APP_MSG_OUT_OF_MEMORY: return "APP_MSG_OUT_OF_MEMORY";
    case APP_MSG_CLOSED: return "APP_MSG_CLOSED";
    case APP_MSG_INTERNAL_ERROR: return "APP_MSG_INTERNAL_ERROR";
    default: return "UNKNOWN ERROR";
  }
}

static int indexOf(char *s_string, char delimiter, int start_index) {
  size_t i;
  for(i = start_index; i < strlen(s_string); i++) {
    if(s_string[i] == delimiter){
      return i;
    }
  }
  return -1;
}

static bool parse_string(char *s_string){
  int positions[2];
  positions[0] = indexOf(s_string, '|', 0);
  positions[1] = indexOf(s_string, '|', positions[0] + 1);
  strncpy(fields[0], s_string, positions[0]);
  strncpy(fields[1], s_string + positions[0] + 1, positions[1] - positions[0] - 1);
  strncpy(fields[2], s_string + positions[1] + 1, strlen(s_string) - positions[1]);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "%s %s %s", fields[0], fields[1], fields[2]);
  return true;
}

static void sync_changed_handler(const uint32_t key, const Tuple *new_tuple, const Tuple *old_tuple, void *context) {
  strncpy(s_string, new_tuple->value->cstring, 120);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "%s", s_string);
  if(key == SONG_INFO_KEY){
    if(parse_string(s_string)) {
      if(fields[0]) set_artist_title(fields[0]);
      if(fields[1]) set_song_title(fields[1]);
      if(fields[2]) set_album_title(fields[2]);
    }
  }
}

static void sync_error_handler(DictionaryResult dict_error, AppMessageResult app_message_error, void *context) {
  // An error occured!
  APP_LOG(APP_LOG_LEVEL_ERROR, "sync error! %s", translate_error(app_message_error));
}

static void in_dropped_handler(AppMessageResult reason, void *context) {
  snprintf(debug_string, 64, "%s %s", "dr", translate_error(reason));
  set_debug_text(debug_string);
}

static void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {
  if (s_wasFirstMsg && s_dataInited) {
    // Ignore, was successful
  } else {
    snprintf(debug_string, 64, "%s %s", "sf", translate_error(reason));
    set_debug_text(debug_string);
 //   APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Failed to Send!");
  //  APP_LOG(APP_LOG_LEVEL_DEBUG, "%s", translate_error(reason));
    /*if(last_sent >= 0) {
      send_command_to_phone(last_sent);
      last_sent = -1;
    }*/
  }
  
  s_wasFirstMsg = false;
}

void init() {
  // Register message handlers
  //app_message_register_inbox_received(in_received_handler);
  
  //test for the app msg
  app_comm_set_sniff_interval(SNIFF_INTERVAL_REDUCED);
    
  app_message_register_inbox_dropped(in_dropped_handler);
  app_message_register_outbox_failed(out_failed_handler);
  
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
  APP_LOG(APP_LOG_LEVEL_DEBUG, "vals %lu, %lu", app_message_inbox_size_maximum(), app_message_outbox_size_maximum());

  fields[0] = calloc(1, 30);
  fields[1] = calloc(1, 30);
  fields[2] = calloc(1, 30);
  // Setup initial values
  Tuplet initial_values[] = {
    TupletCString(SONG_INFO_KEY, "artist|song|album"),
  };

  //Begin using AppSync
  app_sync_init(&s_sync, s_sync_buffer, sizeof(s_sync_buffer), initial_values, ARRAY_LENGTH(initial_values), sync_changed_handler, sync_error_handler, NULL);
  is_init = true; 
}

void comm_deinit() {
  free(fields[0]);
  free(fields[1]);
  free(fields[2]);
  // Finish using AppSync
  app_sync_deinit(&s_sync);
  app_comm_set_sniff_interval(SNIFF_INTERVAL_NORMAL);
}

bool send_command_to_phone(int value) {
  if(!is_init) init(); 
  if(value < 0) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "no data to send");
    return true;
  }
  
  DictionaryIterator *iter;
  AppMessageResult reason;
 // int retry = 3; 
 //while(retry > 0 && iter != NULL) {
    reason = app_message_outbox_begin(&iter);
 //   retry--; 
//  }
  if (iter == NULL) {
      //APP_LOG(APP_LOG_LEVEL_DEBUG, "null iter");
      //APP_LOG(APP_LOG_LEVEL_DEBUG, "%s", translate_error(reason));
      snprintf(debug_string, 64, "%s %s", "ni", translate_error(reason));
      set_debug_text(debug_string);
      return false;
  }
  
  Tuplet tuple = TupletInteger(0, value);
  dict_write_tuplet(iter, &tuple);
  dict_write_end(iter);

  app_message_outbox_send();
  return true;
}

bool send_string_to_phone(char *s_string) {
  if(!is_init) init(); 
  if(s_string == NULL) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "no data to send");
    return true;
  }
  
  DictionaryIterator *iter;
  //int retry = 3; 
  //while(retry > 0 && iter != NULL) {
    AppMessageResult reason = app_message_outbox_begin(&iter);
   // retry--; 
  //}
  if (iter == NULL) {
      APP_LOG(APP_LOG_LEVEL_DEBUG, "null iter");
     // APP_LOG(APP_LOG_LEVEL_DEBUG, "%s", translate_error(reason));
     return false;
  }
  
  Tuplet tuple = TupletCString(STRING_KEY, s_string);
  dict_write_tuplet(iter, &tuple);
  dict_write_end(iter);

  app_message_outbox_send();
  return true;
}

