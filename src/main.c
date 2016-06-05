// Testing github changes.

#include "pebble.h"

static Window *s_window;
static TextLayer *s_text_layer;
uint32_t num_samples = 25;
static char buf[60]; // TODO set this dynamically using malloc and free and all that.
static bool s_button_states[1];

// Handle the logged accelerometer data.
// This function 
static void accel_data_handler(AccelData *data, uint32_t num_samples) {
  
  // Read sample 0's x, y, and z values
  int16_t x = data[0].x;
  int16_t y = data[0].y;
  int16_t z = data[0].z;

  // Determine if the sample occured during vibration, and when it occured
  bool did_vibrate = data[0].did_vibrate;
  uint64_t timestamp = data[0].timestamp;

  if(!did_vibrate) {
    // Log accel data.
    // APP_LOG(APP_LOG_LEVEL_INFO, "t: %llu, x: %d, y: %d, z: %d", timestamp, x, y, z);
    
    // Display accel data on watch.
    snprintf(buf, sizeof buf, "x: %d\ny: %d\nz: %d", x, y, z);
    text_layer_set_text(s_text_layer, buf);
  } else {
    // Discard with a warning
    APP_LOG(APP_LOG_LEVEL_WARNING, "Vibration occured during collection");
  }
}

// Handler for a click on the select button.
static void select_single_click_handler(ClickRecognizerRef recognizer, void *context) {
  // Store button state.
  s_button_states[0] = !s_button_states[0];
  
  // Start/Stop accelerometer service.
  if (s_button_states[0]) {
    // Subscribe to the acceleration service.
    accel_data_service_subscribe(num_samples, accel_data_handler);
  }
  else {
    // Unsubscribe from acceleration service.
    accel_data_service_unsubscribe();
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
