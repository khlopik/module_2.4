#ifndef HARDWARECONFIG_H
#define HARDWARECONFIG_H

#include "driver/gpio.h"

static const gpio_num_t buttonPin = GPIO_NUM_15;
static const uint8_t debounceTimeMs = 50;
static const uint16_t usToMsDivider = 1000;
static const uint8_t superloopYieldDelayMs = 10;

#endif