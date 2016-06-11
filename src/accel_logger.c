#include <pebble.h>
#include "main.h"
#include "accel_logger.h"
#include "data_recorder.h"
#include <@smallstoneapps/linked-list/linked-list.h>

static char buf[60];
uint32_t num_samples = 25;
LinkedRoot* swim_data;

// Handle the logged accelerometer data.
void accel_data_handler(AccelData *data, uint32_t num_samples) {

  // Store data in linked list.
  for (uint32_t i = 0; i < num_samples; i++) {
    
//     APP_LOG(APP_LOG_LEVEL_INFO, "accel_data_handler data - t: %llu", data[i].timestamp);
    
    // Copy data.
    AccelData *item = malloc(sizeof(AccelData));
    memcpy(item, &data[i], sizeof(AccelData));
    
    linked_list_append(swim_data, item);
  }

  // Print to screen.
  snprintf(buf, sizeof buf, "x: %d\ny: %d\nz: %d\nt: %llu\nv: %d", data[0].x, data[0].y, data[0].z, data[0].timestamp, data[0].did_vibrate);
  text_layer_set_text(s_text_layer, buf);
}

// Linked list foreach callback.
static bool process_swim_data(void* accelData, void* context) {
  // Cast data back to AccelData.
  AccelData* data = (AccelData*) accelData;
  
  int16_t x = data->x;
  int16_t y = data->y;
  int16_t z = data->z;
  
  APP_LOG(APP_LOG_LEVEL_INFO, "process_swim_data\nx: %d\ny: %d\nz: %d\n", x, y, z);
  
  return true;
}

// Start logging.
void start_logging() {
  APP_LOG(APP_LOG_LEVEL_INFO, "-----");
  // Created linked list.
  swim_data = linked_list_create_root();
  // Subscribe to the acceleration service.
  accel_data_service_subscribe(num_samples, accel_data_handler);
}

// Stop logging, process stored data.
void stop_logging() {
  // Unsubscribe from acceleration service.
  accel_data_service_unsubscribe();
  // Loop through the swim_data linked list.
  linked_list_foreach(swim_data, process_swim_data, NULL);
}
