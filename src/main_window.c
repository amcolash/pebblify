#include "main_window.h"
#include <pebble.h>
#include "netdownload.h"
#include "png.h"
#include "comm.h"

  
static Layer *window_layer;

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GBitmap *s_res_image_next_icon;
static GBitmap *s_res_image_play_icon;
static GBitmap *s_res_image_previous_icon;
static GFont s_res_gothic_18_bold;
static GFont s_res_gothic_28_bold;
static ActionBarLayer *media_bar;
static TextLayer *s_textlayer_1;
static TextLayer *s_textlayer_2;
static TextLayer *s_textlayer_3;
static TextLayer *s_textlayer_4;
static BitmapLayer *s_bitmaplayer_1;

static void initialise_ui(void) {
  s_window = window_create();
  window_set_fullscreen(s_window, false);
  
  s_res_image_next_icon = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NEXT_ICON);
  s_res_image_play_icon = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_PLAY_ICON);
  s_res_image_previous_icon = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_PREVIOUS_ICON);
  s_res_gothic_18_bold = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);
  s_res_gothic_28_bold = fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD);
  // media_bar
  media_bar = action_bar_layer_create();
  action_bar_layer_add_to_window(media_bar, s_window);
  action_bar_layer_set_background_color(media_bar, GColorClear);
  action_bar_layer_set_icon(media_bar, BUTTON_ID_UP, s_res_image_next_icon);
  action_bar_layer_set_icon(media_bar, BUTTON_ID_SELECT, s_res_image_play_icon);
  action_bar_layer_set_icon(media_bar, BUTTON_ID_DOWN, s_res_image_previous_icon);
  layer_add_child(window_get_root_layer(s_window), (Layer *)media_bar);
  
  // s_textlayer_1
  s_textlayer_1 = text_layer_create(GRect(2, 0, 120, 20));
  text_layer_set_text(s_textlayer_1, "Artist");
  text_layer_set_font(s_textlayer_1, s_res_gothic_18_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_1);
  
  // s_textlayer_2
  s_textlayer_2 = text_layer_create(GRect(2, 48, 120, 60));
  text_layer_set_text(s_textlayer_2, "Song");
  text_layer_set_font(s_textlayer_2, s_res_gothic_28_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_2);
  
  // s_textlayer_3
  s_textlayer_3 = text_layer_create(GRect(2, 131, 100, 20));
  text_layer_set_text(s_textlayer_3, "Album");
  text_layer_set_font(s_textlayer_3, s_res_gothic_18_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_3);
  
  // s_textlayer_4
  s_textlayer_4 = text_layer_create(GRect(1, 91, 100, 20));
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_4);
  
  // s_bitmaplayer_1
  s_bitmaplayer_1 = bitmap_layer_create(GRect(1, 12, 125, 125));
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_bitmaplayer_1);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  action_bar_layer_destroy(media_bar);
  text_layer_destroy(s_textlayer_1);
  text_layer_destroy(s_textlayer_2);
  text_layer_destroy(s_textlayer_3);
  text_layer_destroy(s_textlayer_4);
  bitmap_layer_destroy(s_bitmaplayer_1);
  gbitmap_destroy(s_res_image_next_icon);
  gbitmap_destroy(s_res_image_play_icon);
  gbitmap_destroy(s_res_image_previous_icon);
}
// END AUTO-GENERATED UI CODE

//img download
static BitmapLayer *bitmap_layer;
static GBitmap *current_bmp;
static char *images[] = {
  "http://madhacks.wespetal.com/images/newimg.png"
};

void show_next_image() {
  // show that we are loading by showing no image
  bitmap_layer_set_bitmap(bitmap_layer, NULL);

  text_layer_set_text(s_textlayer_4, "Loading...");

  // Unload the current image if we had one and save a pointer to this one
  if (current_bmp) {
    gbitmap_destroy(current_bmp);
    current_bmp = NULL;
  }

  netdownload_request(images[0]);
}

//Methods for action bar
static void click_config_provider(void *context); 
static void my_next_click_handler();
static void my_previous_click_handler(); 
static void my_playpause_click_handler();
static void my_vol_up_click_handler();
static void my_vol_down_click_handler();

static GBitmap *s_res_image_pause_icon;

static bool playing; 

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  /*s_textlayer_4 = text_layer_create((GRect) { .origin = { 0, 72 }, .size = { bounds.size.w, 20 } });
  text_layer_set_text(s_textlayer_4, "Shake it!");
  text_layer_set_text_alignment(s_textlayer_4, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(s_textlayer_4));
*/
  bitmap_layer = bitmap_layer_create(bounds);
  layer_add_child(window_layer, bitmap_layer_get_layer(bitmap_layer));
  current_bmp = NULL;
}

static void handle_window_unload(Window* window) {
  text_layer_destroy(s_textlayer_4);
  bitmap_layer_destroy(bitmap_layer);
  destroy_ui();
}

void download_complete_handler(NetDownload *download) {
  GBitmap *bmp = gbitmap_create_with_png_data(download->data, download->length);
     
  bitmap_layer_set_bitmap(bitmap_layer, bmp);

  // Save pointer to currently shown bitmap (to free it)
  if (current_bmp) {
    gbitmap_destroy(current_bmp);
  }
  current_bmp = bmp;

 /* // Free the memory now
  #ifdef PBL_PLATFORM_APLITE
  // gbitmap_create_with_png_data will free download->data
  #else
    free(download->data);
  #endif*/
  // We null it out now to avoid a double free
  download->data = NULL;
  netdownload_destroy(download);
}

void tap_handler(AccelAxisType accel, int32_t direction) {
  show_next_image();
}

void show_main_window(void) {
  initialise_ui();
  
  window_layer = window_get_root_layer(s_window);
  
  //init some more UI
  s_res_image_pause_icon = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_PAUSE_ICON);
  action_bar_layer_set_click_config_provider(media_bar, click_config_provider);
  
  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = window_load,
    .unload = handle_window_unload,
  });
  
   netdownload_initialize(download_complete_handler);
    const bool animated = true;
  window_stack_push(s_window, animated);
  accel_tap_service_subscribe(tap_handler);

  playing = false;
  window_stack_push(s_window, true);
}

void hide_main_window(void) {
  window_stack_remove(s_window, true);
}

void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_UP, (ClickHandler) my_next_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, (ClickHandler) my_playpause_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, (ClickHandler) my_previous_click_handler); 
  window_long_click_subscribe(BUTTON_ID_UP, 0, (ClickHandler) my_vol_up_click_handler, NULL); 
  window_long_click_subscribe(BUTTON_ID_DOWN, 0, (ClickHandler) my_vol_down_click_handler, NULL); 
}

void set_playpause_icon() {
    if(playing) {
      playing = false;
      action_bar_layer_set_icon(media_bar, BUTTON_ID_SELECT, s_res_image_play_icon);
    }
    else {
      playing = true;
      action_bar_layer_set_icon(media_bar, BUTTON_ID_SELECT, s_res_image_pause_icon);
    }
}
void set_song_title(char *s_string) {
    text_layer_set_text(s_textlayer_2, s_string);
}
void set_artist_title(char *s_string) {
    text_layer_set_text(s_textlayer_1, s_string);
}
void set_album_title(char *s_string) {
    text_layer_set_text(s_textlayer_3, s_string);
}

//this isn't perfect, should query phone for current status
static void my_playpause_click_handler() {
    set_playpause_icon(); 
    playing = false;
    send_command_to_phone(MEDIA_PLAYPAUSE);
}

static void my_next_click_handler() {
  send_command_to_phone(MEDIA_NEXT);
}

static void my_previous_click_handler() {
  send_command_to_phone(MEDIA_PREVIOUS);
}

static void my_vol_up_click_handler() {
  send_command_to_phone(MEDIA_VOLUP);
}

static void my_vol_down_click_handler() {
  send_command_to_phone(MEDIA_VOLDOWN);
}

void set_debug_text(char *s_string) {
  //if(s_string) text_layer_set_text(s_textlayer_4, s_string);
  //layer_mark_dirty((Layer *) s_textlayer_4);
}

void refresh_window() {
  layer_mark_dirty((Layer *) s_textlayer_1);
  layer_mark_dirty((Layer *) s_textlayer_2);
  layer_mark_dirty((Layer *) s_textlayer_3);
  layer_mark_dirty((Layer *) window_layer);
}

