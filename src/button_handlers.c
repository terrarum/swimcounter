#include <pebble.h>
#include "main.h"
#include "button_handlers.h"
#include "accel_logger.h"

// Store button states.
static bool s_button_states[1];

// Handler for a click on the select button.
static void select_single_click_handler(ClickRecognizerRef recognizer, void *context) {
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
static void click_config_provider(void *context) {
  // Init button states.
  s_button_states[0] = false;
  // single click select.
  window_single_click_subscribe(BUTTON_ID_SELECT, select_single_click_handler);
}

void button_handlers_init() {
  window_set_click_config_provider(s_window, click_config_provider);
}
