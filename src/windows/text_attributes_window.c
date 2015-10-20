#include "text_attributes_window.h"

static Window *s_main_window;
static Layer *s_layer;

static GTextAttributes *s_attributes;

static char s_sample_text[] = "In addition to paging and text flow with TextLayer and ScrollLayer, you can also take advantage of these features when drawing text into a graphics context inside a LayerUpdateProc.";

static void update_proc(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(layer);

  // Calculate the size of the text that will be drawn with the current attribute settings
  GSize text_size = graphics_text_layout_get_content_size_with_attributes(
    s_sample_text, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD), bounds,
    GTextOverflowModeWordWrap, GTextAlignmentCenter, s_attributes
  );

  // Draw the text in this box with the current attribute settings
  graphics_context_set_text_color(ctx, GColorBlack);
  graphics_draw_text(ctx, s_sample_text, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD),
    GRect(bounds.origin.x, bounds.origin.y, text_size.w, text_size.h), 
    GTextOverflowModeWordWrap, GTextAlignmentCenter, s_attributes
  );
}

static void animate_handler(void *context) {
  GRect bounds = layer_get_bounds(window_get_root_layer(s_main_window));

  // Animate the Layer upwards, lengthening it to allow the next page to be drawn
  GRect start = layer_get_frame(s_layer);
  GRect finish = GRect(start.origin.x, start.origin.y - bounds.size.h, start.size.w, start.size.h * 2);
  PropertyAnimation *prop_anim = property_animation_create_layer_frame(s_layer, &start, &finish);
  Animation *animation = property_animation_get_animation(prop_anim);
  animation_set_duration(animation, 1000);
  animation_schedule(animation);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  // Create the attributes object used for text rendering
  s_attributes = graphics_text_attributes_create();

  // Enable text flow with an inset of 5 pixels
  graphics_text_attributes_enable_screen_text_flow(s_attributes, 5);

  // Enable text paging with a fixed reference point, used for animating or scrolling
  graphics_text_attributes_enable_paging(s_attributes, bounds.origin, bounds);

  s_layer = layer_create(bounds);
  layer_set_update_proc(s_layer, update_proc);
  layer_add_child(window_layer, s_layer);

  // Schedule a paging animation
  app_timer_register(2000, animate_handler, NULL);
}

static void window_unload(Window *window) {
  layer_destroy(s_layer);

  // Destroy text attributes object
  graphics_text_attributes_destroy(s_attributes);
  
  window_destroy(s_main_window);
  s_main_window = NULL;
}

void text_attributes_window_push() {
  if(!s_main_window) {
    s_main_window = window_create();
    window_set_window_handlers(s_main_window, (WindowHandlers) {
      .load = window_load,
      .unload = window_unload,
    });
  }
  window_stack_push(s_main_window, true);
}
