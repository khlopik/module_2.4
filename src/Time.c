#include "Time.h"
#include "HardwareConfig.h"
#include "esp_timer.h"
#include "esp_attr.h"

uint32_t IRAM_ATTR getCurrentTimeMs(void) {
    return esp_timer_get_time() / usToMsDivider;
}