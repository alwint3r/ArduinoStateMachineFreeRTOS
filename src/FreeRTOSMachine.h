#ifndef FREE_RTOS_MACHINE_H
#define FREE_RTOS_MACHINE_H

#include <StateMachine.h>
#include <Task.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

class FreeRTOSMachine: public StateMachine, public Task
{
public:
    FreeRTOSMachine();
    virtual ~FreeRTOSMachine();

    void run(void *pvParameters) override;

protected:
    void dispatchEvent(const StateEvent &event) override;
    void dispatchEventFromISR(const StateEvent &event) override;

protected:
    QueueHandle_t _queue;
};

#endif // FREE_RTOS_MACHINE_H