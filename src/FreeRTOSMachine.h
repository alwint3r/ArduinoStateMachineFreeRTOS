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

    virtual void event(const StateEvent &event);
    virtual void eventFromISR(const StateEvent &event);
    virtual void run(void *pvParameters);

protected:
    QueueHandle_t _queue;
};

#endif // FREE_RTOS_MACHINE_H