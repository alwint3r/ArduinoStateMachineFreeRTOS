#include "FreeRTOSMachine.h"
#include <Arduino.h>

FreeRTOSMachine::FreeRTOSMachine() : StateMachine(), Task("freeRtosHSM", 4096, 10)
{
    _queue = xQueueCreate(5, sizeof(StateEvent));
}

FreeRTOSMachine::~FreeRTOSMachine()
{
    vQueueDelete(_queue);
}

void FreeRTOSMachine::event(const StateEvent &event)
{
    Serial.printf("Received event %d\n", event.id);
    xQueueSend(_queue, &event, 0);
}

void FreeRTOSMachine::eventFromISR(const StateEvent &event)
{
    xQueueSendFromISR(_queue, &event, 0);
}

void FreeRTOSMachine::run(void *pvParameters)
{
    StateEvent event = {0, 0};
    if (_currentState != nullptr)
    {
        _currentState->enter();
    }

    while (1)
    {
        if (xQueueReceive(_queue, &event, portMAX_DELAY) == pdPASS)
        {
            processEvent(&event);
    
            vTaskDelay(pdMS_TO_TICKS(100));
        }
    }
}