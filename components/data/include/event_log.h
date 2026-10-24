#ifndef EVENT_LOG_H
#define EVENT_LOG_H

#include <stdint.h>

#define EVENT_LOG_MAX_ENTRIES (32U)

typedef enum {
  EVENT_ALERT_TEMP_HIGH = 0U,
  EVENT_ALERT_TEMP_LOW,
  EVENT_ALERT_HUMI_HIGH,
  EVENT_SENSOR_ERROR,
  EVENT_SYSTEM_RESET,
  EVENT_NUM_TYPES
} event_type_t;

typedef struct event_node {
  event_type_t type;
  uint32_t timestamp;
  int16_t value;
  struct event_node *p_next;
} event_node_t;

void event_log_init(void);

void event_log_add(event_type_t type, uint32_t timestamp, int16_t value);

uint32_t event_log_get_count(void);

void event_log_print_all(void);

const event_node_t *event_log_get_at(uint32_t index);

#endif /* EVENT_LOG_H */