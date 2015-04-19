#pragma once
#include <pebble.h>
enum {
  DATA_KEY = 0x0,
  STRING_KEY = 0x1,
  SONG_INFO_KEY = 0x2,
  MEDIA_PLAYPAUSE = 0x0,
  MEDIA_PREVIOUS = 0x1,
  MEDIA_NEXT = 0x2,
  MEDIA_VOLDOWN = 0x3,
  MEDIA_VOLUP = 0x4,
};

bool send_command_to_phone(int value);
bool send_string_to_phone(char *s_string);
void comm_deinit(); 