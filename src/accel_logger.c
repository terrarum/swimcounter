#include <pebble.h>
#include "main.h"
#include "accel_logger.h"
#include "data_recorder.h"
#include <@smallstoneapps/linked-list/linked-list.h>

static char buf[60]; // TODO set this dynamically using malloc and free and all that.
uint32_t num_samples = 25;

LinkedRoot* swim_data;

// Handle the logged accelerometer data.
// This function 
void accel_data_handler(AccelData *data, uint32_t num_samples) {

  uint32_t i;
  // Store data in linked list.
  for (i = 0; i < num_samples; i++) {
    linked_list_append(swim_data, &data[i]);
  }

  // Read sample 0's x, y, and z values.
  int16_t x = data[0].x;
  int16_t y = data[0].y;
  int16_t z = data[0].z;

  // Determine if the sample occured during vibration, and when it occured.
  bool did_vibrate = data[0].did_vibrate;
  // uint64_t timestamp = data[0].timestamp;

  if(!did_vibrate) {
    // Log accel data.
    // APP_LOG(APP_LOG_LEVEL_INFO, "t: %llu, x: %d, y: %d, z: %d", timestamp, x, y, z);
    
    // Display accel data on watch.
    snprintf(buf, sizeof buf, "x: %d\ny: %d\nz: %d", x, y, z);
    text_layer_set_text(s_text_layer, buf);
  } else {
    // Discard with a warning.
    APP_LOG(APP_LOG_LEVEL_WARNING, "Vibration occured during collection");
  }
}

AccelData dataItem;

// Linked list foreach callback.
static bool process_swim_data(void* accelData, void* context) {
  AccelData data = *((AccelData *) accelData);
  int16_t x = data.x;
  int16_t y = data.y;
  int16_t z = data.z;
  uint64_t timestamp = data.timestamp;
  APP_LOG(APP_LOG_LEVEL_INFO, "t: %llu, x: %d, y: %d, z: %d", timestamp, x, y, z);
  return true;
}

void start_logging() {
  // Subscribe to the acceleration service.
  swim_data = linked_list_create_root();
  accel_data_service_subscribe(num_samples, accel_data_handler);
}

void stop_logging() {
  accel_data_service_unsubscribe();
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Size of swim_data: %d", linked_list_count(swim_data));
  linked_list_foreach(swim_data, process_swim_data, NULL);
}
