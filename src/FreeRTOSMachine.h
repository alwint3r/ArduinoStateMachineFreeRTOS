#ifndef FREE_RTOS_MACHINE_H
#define FREE_RTOS_MACHINE_H

#include <StateMachine.h>
#include <Task.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
template <typename T, class C>
class FreeRTOSMachine: public StateMachine<T, C>, public Task
{
public:
    FreeRTOSMachine()
    {
        _queue = xQueueCreate(5, sizeof(StateEvent<T, C>));
    }

    virtual ~FreeRTOSMachine()
    {
        vQueueDelete(_queue);
    }

    void run(void *pvParameters) override
    {
        StateEvent<T, C> event;
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

protected:
    void dispatchEvent(const StateEvent<T, C> &event) override
    {
        Serial.printf("Received event %d\n", event.id);
          xQueueSend(_queue, &event, 0);
    }

    void dispatchEventFromISR(const StateEvent<T, C> &event) override
    {
        xQueueSendFromISR(_queue, &event, 0);
    }

protected:
    using StateMap = std::map<std::string, StateClass<T, C> *>;
    using StatePair = std::pair<std::string, StateClass<T, C> *>;

    StateMap *_stateMap;
    StateClass<T, C> *_currentState = nullptr;

    QueueHandle_t _queue;
};

#endif // FREE_RTOS_MACHINE_H