#ifndef GPIO_H
#define GPIO_H

#include "stdint.h"
#include "stdbool.h"

uint32_t getButtonPressTimeMs(void);
bool isButtonPressed(void);
void resetButtonPressed(void);
uint16_t getPressCounter(void);

void gpioInit(void);

typedef enum {
    BUTTON_PRESSED,
    BUTTON_RELEASED,
} button_state_t;

button_state_t getButtonState(void);

#endif