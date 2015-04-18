#include <pebble.h>

Window *main_window;
TextLayer *artist_text_layer;
TextLayer *song_text_layer;
TextLayer *album_text_layer;

static Window *s_main_window;
//static TextLayer *s_time_layer; 

ActionBarLayer *action_bar;

//Methods for window
static void main_window_load(Window *window);
static void main_window_unload(Window *window); 

//Methods for action bar
static void click_config_provider(void *context); 
static void my_next_click_handler();
static void my_previous_click_handler(); 
static void my_playpause_click_handler();

//Icons
static GBitmap *my_icon_previous; 
static GBitmap *my_icon_play; 
static GBitmap *my_icon_pause; 
static GBitmap *my_icon_next; 

//State variables
bool playing; 

void handle_init(void) {
  s_main_window = window_create();
 
   // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  
  window_stack_push(s_main_window, true);
  
  //TODO: need to have it poll for this
  playing = false;
}

void handle_deinit(void) {
  text_layer_destroy(artist_text_layer);
  text_layer_destroy(song_text_layer);
  text_layer_destroy(album_text_layer);
  window_destroy(s_main_window);
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}

static void main_window_load(Window *window) {
  
  //Init action bar and set click provider
  action_bar = action_bar_layer_create(); 
  action_bar_layer_add_to_window(action_bar, window); 
  action_bar_layer_set_click_config_provider(action_bar, click_config_provider);
  
  //Set the icons
  my_icon_previous = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_PREVIOUS_ICON);
  my_icon_next = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NEXT_ICON);
  my_icon_play = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_PLAY_ICON);
  my_icon_pause = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_PAUSE_ICON);

  action_bar_layer_set_icon(action_bar, BUTTON_ID_UP, my_icon_previous);
  action_bar_layer_set_icon(action_bar, BUTTON_ID_SELECT, my_icon_play);
  action_bar_layer_set_icon(action_bar, BUTTON_ID_DOWN, my_icon_next);
  
  artist_text_layer = text_layer_create(GRect(0, 0, 120, 40));
  text_layer_set_text_color(artist_text_layer, GColorBlack);
  
  song_text_layer = text_layer_create(GRect(0, 45, 120, 60));
  text_layer_set_text_color(song_text_layer, GColorBlack);
  
  album_text_layer = text_layer_create(GRect(0, 124, 120, 20));
  text_layer_set_text_color(album_text_layer, GColorBlack);

  text_layer_set_font(artist_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_text(artist_text_layer, "Text");
  text_layer_set_text_alignment(artist_text_layer, GTextAlignmentLeft); 
  
  text_layer_set_font(song_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text(song_text_layer, "Text2");
  text_layer_set_text_alignment(song_text_layer, GTextAlignmentLeft); 
  
  text_layer_set_font(album_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  text_layer_set_text(album_text_layer, "Text3");
  text_layer_set_text_alignment(album_text_layer, GTextAlignmentLeft); 
  
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(artist_text_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(song_text_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(album_text_layer));
}

static void main_window_unload(Window *window) {
  gbitmap_destroy(my_icon_previous);
  gbitmap_destroy(my_icon_next);
  gbitmap_destroy(my_icon_play);
  gbitmap_destroy(my_icon_pause);
}

void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_DOWN, (ClickHandler) my_next_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, (ClickHandler) my_playpause_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, (ClickHandler) my_previous_click_handler); 
}

static void my_playpause_click_handler() {
  if(playing){
    action_bar_layer_set_icon(action_bar, BUTTON_ID_SELECT, my_icon_pause);
    playing = false;
  }
  else {
    action_bar_layer_set_icon(action_bar, BUTTON_ID_SELECT, my_icon_play);
    playing = true; 
  }
}

static void my_next_click_handler() {
  
}

static void my_previous_click_handler() {
  
}