#include "input.h"

sample_t take_sample(void);

void vRTES_Input(void *pvParameters) {
    task_info_input_t task_info = *(task_info_input_t*) pvParameters;
    buffer_t *buffer = task_info.out_buffer;
    const char * const pcTaskName = task_info.basic_info.pcTaskName;
    TickType_t taskPeriod = task_info.basic_info.xTaskPeriod;

    TickType_t xTimeTaskStarted;
    for (;;) {
        xTimeTaskStarted = xTaskGetTickCount();

        do_input(buffer);

        vTaskDelayUntil(&xTimeTaskStarted, taskPeriod);
    }
}

void do_input(buffer_t *buffer) {
    insertIntoBuffer(buffer, take_sample());
}

sample_t take_sample(void) {
    static size_t index = 0;

    return read_data(index++);
}
