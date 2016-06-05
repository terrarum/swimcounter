#include "pebble.h"
#include "main.h"
#include "accel_logger.h"

Window *s_window;
TextLayer *s_text_layer;
static bool s_button_states[1];

// Handler for a click on the select button.
void select_single_click_handler(ClickRecognizerRef recognizer, void *context) {
  // Store button state.
  s_button_states[0] = !s_button_states[0];
  
  // Start/Stop accelerometer service.
  if (s_button_states[0]) {
    start_logging();
  }
  else {
    // Unsubscribe from acceleration service.
    stop_logging();
  }
}

// Set up button listeners.
void click_config_provider(void *context) {
 // single click select:
  window_single_click_subscribe(BUTTON_ID_SELECT, select_single_click_handler);
}

static void init(void) {
  // Select has not been pressed.
  s_button_states[0] = false;
  
	// Create a window and get information about the window
	s_window = window_create();
  Layer *window_layer = window_get_root_layer(s_window);
  GRect bounds = layer_get_bounds(window_layer);
	
  // Create a text layer and set the text
	s_text_layer = text_layer_create(bounds);
	text_layer_set_text(s_text_layer, "Press Select to Start");
  
  // Set the font and text alignment
	text_layer_set_font(s_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
	// text_layer_set_text_alignment(s_text_layer, GTextAlignmentCenter);

	// Add the text layer to the window
	layer_add_child(window_get_root_layer(s_window), text_layer_get_layer(s_text_layer));
  
  // Enable text flow and paging on the text layer, with a slight inset of 10, for round screens
  text_layer_enable_screen_text_flow_and_paging(s_text_layer, 10);

	// Push the window, setting the window animation to 'true'
	window_stack_push(s_window, true);
	
  // Listen for clicks.
  window_set_click_config_provider(s_window, click_config_provider);
}

static void deinit(void) {
	// Destroy the text layer
	text_layer_destroy(s_text_layer);
	
	// Destroy the window
	window_destroy(s_window);
}

int main(void) {
	init();
	app_event_loop();
	deinit();
}
