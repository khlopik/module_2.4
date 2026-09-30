#include "HardwareConfig.h"
#include "Gpio.h"
#include "Time.h"
#include "esp_attr.h"

volatile static uint32_t s_buttonPressTimeMs = 0;
volatile static bool s_isButtonPressed = false;
volatile static uint16_t s_buttonPressCounter = 0;

static const uint8_t buttonPressedLevel = 0;

uint32_t getButtonPressTimeMs(void)
{
    return s_buttonPressTimeMs;
}

bool isButtonPressed(void)
{
    return s_isButtonPressed;
}

void resetButtonPressed(void)
{
    s_isButtonPressed = false;
}

uint16_t getPressCounter(void)
{
    return s_buttonPressCounter;
}

void IRAM_ATTR gpioIsrOnButtonPress(void *arg)
{
    s_buttonPressCounter++;
    s_buttonPressTimeMs = getCurrentTimeMs();
    s_isButtonPressed = true;
}

void gpioInit(void)
{
    const gpio_config_t buttonConfig = {
        .pin_bit_mask = 1ULL << buttonPin,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_NEGEDGE,
    };

    gpio_config(&buttonConfig);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(buttonPin, gpioIsrOnButtonPress, NULL);
}

button_state_t getButtonState(void)
{
    const uint8_t level = gpio_get_level(buttonPin);

    return level == buttonPressedLevel ? BUTTON_PRESSED : BUTTON_RELEASED;
}