#include "input.h"

void vRTES_Input(void *pvParameters) {
    RTES_TaskParameters param = *(RTES_TaskParameters*) pvParameters;
    RTES_Buffer_t *buffer = param.out_buffer;
    const char * const pcTaskName = param.pcTaskName;
    TickType_t *taskPeriod = param.xTaskPeriod; 

    TickType_t xTimeTaskStarted;
    for (;;) {
        xTimeTaskStarted = xTaskGetTickCount();
        
        do_input(buffer);
        
        vTaskDelayUntil(&xTimeTaskStarted, *taskPeriod);
    }
}

void do_input(RTES_Buffer_t *buffer) {
    RTES_Sample_t sample = take_sample();

    insertIntoBuffer(buffer, sample);
}

RTES_Sample_t take_sample(void) {
    static size_t index = 0;
    RTES_Sample_t sample;

    sample.data = data[index];

    /* Should index point to index 0 after incrementing? */
    if (index == data_size - 1) {
        index = 0;
    } else {
        index++;
    }

    return sample;
}
