#include <pebble.h>
#include "comm.h"
#include "main_window.h"
  
static bool is_init = false; 
static bool s_wasFirstMsg;
static bool s_dataInited;
static char s_string[64];
int last_sent;

static void in_received_handler(DictionaryIterator *iter, void *context) {
  Tuple *playpause_tuple = dict_find(iter, MEDIA_PLAYPAUSE);
  Tuple *song_tuple = dict_find(iter, SONG_KEY);
  Tuple *artist_tuple = dict_find(iter, ARTIST_KEY);
  Tuple *album_tuple = dict_find(iter, ALBUM_KEY);
  
  if (playpause_tuple) {
    set_playpause_icon(); 
  }
  if(song_tuple){
    strncpy(s_string, song_tuple->value->cstring, 64);
    set_song_title(s_string);
  }
  if(artist_tuple) {
    strncpy(s_string, artist_tuple->value->cstring, 64);
    set_artist_title(s_string);
  }
   if(album_tuple) {
    strncpy(s_string, artist_tuple->value->cstring, 64);
    set_album_title(s_string);
  }
}

static void in_dropped_handler(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Dropped!");
}

static void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {
  if (s_wasFirstMsg && s_dataInited) {
    // Ignore, was successful
  } else {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Failed to Send!");
    if(last_sent >= 0) {
      send_command_to_phone(last_sent);
      last_sent = -1;
    }
  }
  
  s_wasFirstMsg = false;
}

void init() {
  // Register message handlers
  app_message_register_inbox_received(in_received_handler);
  app_message_register_inbox_dropped(in_dropped_handler);
  app_message_register_outbox_failed(out_failed_handler);
  
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
  is_init = true; 
}

bool send_command_to_phone(int value) {
  if(!is_init) init(); 
  if(value < 0) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "no data to send");
    return true;
  }
  
  DictionaryIterator *iter;
  int retry = 3; 
  while(retry > 0 && iter != NULL) {
    app_message_outbox_begin(&iter);
    retry--; 
  }
  if (iter == NULL) {
      APP_LOG(APP_LOG_LEVEL_DEBUG, "null iter");
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
  int retry = 3; 
  while(retry > 0 && iter != NULL) {
    app_message_outbox_begin(&iter);
    retry--; 
  }
  if (iter == NULL) {
      APP_LOG(APP_LOG_LEVEL_DEBUG, "null iter");
      return false;
  }
  
  Tuplet tuple = TupletCString(STRING_KEY, s_string);
  dict_write_tuplet(iter, &tuple);
  dict_write_end(iter);

  app_message_outbox_send();
  return true;
}

