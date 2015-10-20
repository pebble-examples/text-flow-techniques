#include "static_text_window.h"

static Window *s_main_window;
static TextLayer *s_title_layer, *s_body_layer;

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  s_title_layer = text_layer_create(bounds);
  text_layer_set_text(s_title_layer, "Fastest Ship in the Universe Stolen!");
  text_layer_set_text_alignment(s_title_layer, GTextAlignmentCenter);
  text_layer_set_font(s_title_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_background_color(s_title_layer, GColorBlueMoon);
  text_layer_set_text_color(s_title_layer, GColorWhite);
  layer_add_child(window_layer, text_layer_get_layer(s_title_layer));

  // Must be after added to the view heirachy
  text_layer_enable_screen_text_flow_and_paging(s_title_layer, 2);

  GSize title_size = text_layer_get_content_size(s_title_layer);

  s_body_layer = text_layer_create(GRect(bounds.origin.x, bounds.origin.y + title_size.h + 5, bounds.size.w, bounds.size.h));
  text_layer_set_text(s_body_layer, "The President of the Galaxy is on the run from Galactic Police after a ceremonial ship launch, quoted as saying \"I hereby kidnap myself, and I'm taking the ship with me!\"");
  text_layer_set_text_alignment(s_body_layer, GTextAlignmentCenter);
  text_layer_set_background_color(s_body_layer, GColorPictonBlue);
  text_layer_set_text_color(s_body_layer, GColorWhite);
  layer_add_child(window_layer, text_layer_get_layer(s_body_layer));

  text_layer_enable_screen_text_flow_and_paging(s_body_layer, 2);
}

static void window_unload(Window *window) {
  text_layer_destroy(s_title_layer);
  text_layer_destroy(s_body_layer);
  
  window_destroy(s_main_window);
  s_main_window = NULL;
}

void static_text_window_push() {
  if(!s_main_window) {
    s_main_window = window_create();
    window_set_window_handlers(s_main_window, (WindowHandlers) {
      .load = window_load,
      .unload = window_unload,
    });
  }
  window_stack_push(s_main_window, true);
}
