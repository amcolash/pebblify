#include "test.h"
#include <pebble.h>

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
  text_layer_set_text(s_textlayer_1, "Text layer");
  text_layer_set_font(s_textlayer_1, s_res_gothic_18_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_1);
  
  // s_textlayer_2
  s_textlayer_2 = text_layer_create(GRect(2, 39, 120, 60));
  text_layer_set_text(s_textlayer_2, "Text layer");
  text_layer_set_font(s_textlayer_2, s_res_gothic_28_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_2);
  
  // s_textlayer_3
  s_textlayer_3 = text_layer_create(GRect(2, 131, 100, 20));
  text_layer_set_text(s_textlayer_3, "Text layer");
  text_layer_set_font(s_textlayer_3, s_res_gothic_18_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_3);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  action_bar_layer_destroy(media_bar);
  text_layer_destroy(s_textlayer_1);
  text_layer_destroy(s_textlayer_2);
  text_layer_destroy(s_textlayer_3);
  gbitmap_destroy(s_res_image_next_icon);
  gbitmap_destroy(s_res_image_play_icon);
  gbitmap_destroy(s_res_image_previous_icon);
}
// END AUTO-GENERATED UI CODE

static void handle_window_unload(Window* window) {
  destroy_ui();
}

void show_test(void) {
  initialise_ui();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  window_stack_push(s_window, true);
}

void hide_test(void) {
  window_stack_remove(s_window, true);
}
