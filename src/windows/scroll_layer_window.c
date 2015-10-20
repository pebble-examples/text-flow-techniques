#include "scroll_layer_window.h"

static Window *s_main_window;
static ScrollLayer *s_scroll_layer;
static TextLayer *s_text_layer;

static char s_long_text[] = "Very long text sample designed to test the new Pebble SDK text flow feature. Another point to note is that the TextLayer must have paging enabled AFTER being added to the view heirachy, or it will have no effect.";

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  s_scroll_layer = scroll_layer_create(bounds);
  scroll_layer_set_click_config_onto_window(s_scroll_layer, window);
  layer_add_child(window_layer, scroll_layer_get_layer(s_scroll_layer));

  s_text_layer = text_layer_create(GRect(bounds.origin.x, bounds.origin.y, bounds.size.w, 2000));
  text_layer_set_text(s_text_layer, s_long_text);
  text_layer_set_text_alignment(s_text_layer, GTextAlignmentCenter);
  text_layer_set_font(s_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  scroll_layer_add_child(s_scroll_layer, text_layer_get_layer(s_text_layer));

  scroll_layer_set_content_size(s_scroll_layer, text_layer_get_content_size(s_text_layer));

  // Must be after added to the view heirachy
  text_layer_enable_screen_text_flow_and_paging(s_text_layer, 2);

  // Enable ScrollLayer paging
  scroll_layer_set_paging(s_scroll_layer, true);
}

static void window_unload(Window *window) {
  text_layer_destroy(s_text_layer);
  scroll_layer_destroy(s_scroll_layer);
  
  window_destroy(s_main_window);
  s_main_window = NULL;
}

void scroll_layer_window_push() {
  if(!s_main_window) {
    s_main_window = window_create();
    window_set_window_handlers(s_main_window, (WindowHandlers) {
      .load = window_load,
      .unload = window_unload,
    });
  }
  window_stack_push(s_main_window, true);
}
