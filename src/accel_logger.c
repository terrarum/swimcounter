#include <pebble.h>
#include "main.h"
#include "accel_logger.h"

static char buf[60];
const uint32_t num_samples = 20; // Seems to struggle to log more than 20 at a time.

#define ACCEL_DATA_LOG 1
static DataLoggingSessionRef s_session_ref;

const int value = 16;

// Handle the logged accelerometer data.
void accel_data_handler(AccelData *data, uint32_t num_sampless) {
  
  // Store data.
  // Loops because I can't figure out how to batch-log data.
  for (uint32_t i = 0; i < num_sampless; i++) {
    DataLoggingResult result = data_logging_log(s_session_ref, &data[i], 1);
    
    switch((int)result) {
      case DATA_LOGGING_SUCCESS:
        APP_LOG(APP_LOG_LEVEL_ERROR, "DATA_LOGGING_SUCCESS: %d", (int)result);
        break;
      case DATA_LOGGING_NOT_FOUND:
        APP_LOG(APP_LOG_LEVEL_ERROR, "DATA_LOGGING_NOT_FOUND: %d", (int)result);
        break;
      case DATA_LOGGING_CLOSED:
        APP_LOG(APP_LOG_LEVEL_ERROR, "DATA_LOGGING_CLOSED: %d", (int)result);
        break;
      case DATA_LOGGING_BUSY:
        APP_LOG(APP_LOG_LEVEL_ERROR, "DATA_LOGGING_BUSY: %d", (int)result);
        break;
      case DATA_LOGGING_INVALID_PARAMS:
        APP_LOG(APP_LOG_LEVEL_ERROR, "DATA_LOGGING_INVALID_PARAMS: %d", (int)result);
        break;
      case DATA_LOGGING_FULL:
        APP_LOG(APP_LOG_LEVEL_ERROR, "DATA_LOGGING_FULL: %d", (int)result);
        break;
    }
    
  }

  // Print to screen.
  snprintf(buf, sizeof buf, "x: %d\ny: %d\nz: %d\nt: %llu\nv: %d", data[0].x, data[0].y, data[0].z, data[0].timestamp, data[0].did_vibrate);
  text_layer_set_text(s_text_layer, buf);
}

// Start logging.
void start_logging() {
  // Beging DataLogging session.
  s_session_ref = data_logging_create(ACCEL_DATA_LOG, DATA_LOGGING_BYTE_ARRAY, sizeof(AccelData), true);
  // Subscribe to the acceleration service.
  accel_data_service_subscribe(num_samples, accel_data_handler);
}

// Stop logging, process stored data.
void stop_logging() {
  // Unsubscribe from acceleration service.
  accel_data_service_unsubscribe();
  // Finish logging data.
  data_logging_finish(s_session_ref);
}
