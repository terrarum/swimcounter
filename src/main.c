#include "pebble.h"
#include "main.h"
#include "button_handlers.h"
#include "accel_logger.h"

Window *s_window;
TextLayer *s_text_layer;

static void init(void) {  
  // Create a window and get information about the window
  s_window = window_create();
  Layer *window_layer = window_get_root_layer(s_window);
  GRect bounds = layer_get_bounds(window_layer);
	
  // Create a text layer and set the text
  s_text_layer = text_layer_create(bounds);
  text_layer_set_text(s_text_layer, "Press Select to Start");
  
  // Set the font and text alignment
  text_layer_set_font(s_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  
  // Add the text layer to the window
  layer_add_child(window_get_root_layer(s_window), text_layer_get_layer(s_text_layer));
  
  // Enable text flow and paging on the text layer, with a slight inset of 10, for round screens
//   text_layer_enable_screen_text_flow_and_paging(s_text_layer, 10);
  
  // Push the window, setting the window animation to 'true'
  window_stack_push(s_window, true);
  
  // Listen for clicks.
  button_handlers_init();
}

static void deinit(void) {
	// Destroy the text layer
	text_layer_destroy(s_text_layer);
	
	// Destroy the window
	window_destroy(s_window);
}

// App starts here.
int main(void) {
  // Starts the app.
	init();
  // Enter the main event loop. This will block until the app is ready to exit.
	app_event_loop();
  // Clean up.
	deinit();
}
