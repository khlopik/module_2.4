#include "esp_log.h"
#include "Log.h"

void printCounter(uint16_t count)
{
    ESP_LOGI("Counter", "%u", count);
}