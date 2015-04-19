#pragma once
#include <pebble.h>
bool send_to_phone(int key, char *s_symbol);
enum {
  MEDIA_PLAY = 0x0,
  MEDIA_PAUSE = 0x1,
  MEDIA_PREVIOUS = 0x2,
  MEDIA_NEXT = 0x3,
};