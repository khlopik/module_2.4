#include "freertos/FreeRTOS.h"
#include "HardwareConfig.h"
#include "Gpio.h"
#include "Log.h"
#include "Time.h"

enum Approach
{
    NoDebounce,
    SoftTimeDebounce,
    StateDebounce,
    PollingDebounce,
    HardwareDebounce,
};

static enum Approach selectedApproach = NoDebounce;

static uint16_t approach1Counter = 0;
static uint16_t approach2Counter = 0;
static uint16_t approach3Counter = 0;
static uint16_t approach4Counter = 0;

typedef enum
{
    RELEASED,
    PRESS_DEBOUNCE,
    PRESSED,
    PRESSING,
    RELEASE_DEBOUNCE,
} approach4_state_t;

static uint32_t approach2InterrupTimeMs = 0;
static approach4_state_t approach4CurrentState = RELEASED;
static uint32_t approach4StateChangeTimeMs = 0;

void updateState(void)
{
    switch (selectedApproach)
    {
    case NoDebounce:
    case HardwareDebounce:
    {
        const uint16_t currentCount = getPressCounter();

        if (currentCount == approach1Counter)
            break;

        printCounter(currentCount);
        approach1Counter = currentCount;
    }
    break;
    case SoftTimeDebounce:
    {
        if (!isButtonPressed())
            break;

        resetButtonPressed();

        uint32_t currentPressTimeMs = getButtonPressTimeMs();
        if (currentPressTimeMs - approach2InterrupTimeMs < debounceTimeMs)
            break;

        approach2InterrupTimeMs = currentPressTimeMs;
        approach2Counter++;
        printCounter(approach2Counter);
    }
    break;
    case StateDebounce:
    {
        if (isButtonPressed())
        {
            resetButtonPressed();

            if (getButtonState() != BUTTON_PRESSED)
                break;

            approach3Counter++;
            printCounter(approach3Counter);
        }
    }
    break;
    case PollingDebounce:
    {
        const button_state_t currentState = getButtonState();
        const uint32_t currentTime = getCurrentTimeMs();

        switch (approach4CurrentState)
        {
        case RELEASED:
            if (currentState == BUTTON_PRESSED)
            {
                approach4CurrentState = PRESS_DEBOUNCE;
                approach4StateChangeTimeMs = currentTime;
            }
            break;
        case PRESS_DEBOUNCE:
        {
            if (currentState == BUTTON_RELEASED)
            {
                approach4CurrentState = RELEASED;
            }
            else if ((currentTime - approach4StateChangeTimeMs) >= debounceTimeMs)
            {
                approach4CurrentState = PRESSED;
            }
        }
        break;
        case PRESSED:
        {
            approach4Counter++;
            approach4CurrentState = PRESSING;
            printCounter(approach4Counter);
        }
        break;
        case PRESSING:
        {
            if (currentState == BUTTON_RELEASED)
            {
                approach4CurrentState = RELEASE_DEBOUNCE;
                approach4StateChangeTimeMs = currentTime;
            }
        }
        break;
        case RELEASE_DEBOUNCE:
        {
            if (currentState == BUTTON_PRESSED)
            {
                approach4CurrentState = PRESSING;
            }
            else if ((currentTime - approach4StateChangeTimeMs) >= debounceTimeMs)
            {
                approach4CurrentState = RELEASED;
            }
        }
        break;
        }
    }
    break;
    default:
        break;
    }
}

void app_main()
{
    gpioInit();

    while (true)
    {
        updateState();
        vTaskDelay(pdMS_TO_TICKS(superloopYieldDelayMs));
    }
}