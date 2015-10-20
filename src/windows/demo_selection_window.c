#include "demo_selection_window.h"

#include "static_text_window.h"
#include "scroll_layer_window.h"
#include "text_attributes_window.h"

#define FOCUSED_TALL_CELL_HEIGHT   60
#define UNFOCUSED_TALL_CELL_HEIGHT 30
#define NUM_WINDOWS                3

static Window *s_main_window;
static MenuLayer *s_menu_layer;

static uint16_t get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *context) {
  return NUM_WINDOWS;
}

static void draw_row_callback(GContext *ctx, Layer *cell_layer, MenuIndex *cell_index, void *context) {
  switch(cell_index->row) {
    case 0:
      menu_cell_basic_draw(ctx, cell_layer, "Static TextLayer", NULL, NULL);
      break;
    case 1:
      menu_cell_basic_draw(ctx, cell_layer, "Paging ScrollLayer", NULL, NULL);
      break;
    case 2:
      menu_cell_basic_draw(ctx, cell_layer, "GTextAttributes", NULL, NULL);
      break;
    default:
      break;
  }
}

static int16_t get_cell_height_callback(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *context) {
  MenuIndex selected = menu_layer_get_selected_index(menu_layer);
  return selected.row == cell_index->row && selected.section == cell_index->section ?
    FOCUSED_TALL_CELL_HEIGHT : UNFOCUSED_TALL_CELL_HEIGHT;
}

static void select_callback(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *context) {
  switch(cell_index->row) {
    case 0:
      static_text_window_push();
      break;
    case 1:
      scroll_layer_window_push();
      break;
    case 2:
      text_attributes_window_push();
      break;
    default:
      break;
  }
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  s_menu_layer = menu_layer_create(bounds);
  menu_layer_set_click_config_onto_window(s_menu_layer, window);
  menu_layer_set_center_focused(s_menu_layer, PBL_IF_ROUND_ELSE(true, false));
  menu_layer_set_callbacks(s_menu_layer, NULL, (MenuLayerCallbacks) {
      .get_num_rows = (MenuLayerGetNumberOfRowsInSectionsCallback)get_num_rows_callback,
      .draw_row = (MenuLayerDrawRowCallback)draw_row_callback,
      .get_cell_height = (MenuLayerGetCellHeightCallback)get_cell_height_callback,
      .select_click = (MenuLayerSelectCallback)select_callback,
  });
  layer_add_child(window_layer, menu_layer_get_layer(s_menu_layer));
}

static void window_unload(Window *window) {
  menu_layer_destroy(s_menu_layer);
  window_destroy(s_main_window);
  s_main_window = NULL;
}

void demo_selection_window_push() {
  if(!s_main_window) {
    s_main_window = window_create();
    window_set_window_handlers(s_main_window, (WindowHandlers) {
      .load = window_load,
      .unload = window_unload
    });
  }
  window_stack_push(s_main_window, true);
}
