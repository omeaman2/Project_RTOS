#include "output.h"

void vRTES_Output(void *pvParameters) {
    RTES_TaskParameters param = *(RTES_TaskParameters*) pvParameters;
    RTES_Buffer_t *buffer = param.in_buffer;
    //const char * const pcTaskName = param.pcTaskName;
    TickType_t *taskPeriod = param.xTaskPeriod;

    TickType_t xTimeTaskStarted;
    for (;;) {
        xTimeTaskStarted = xTaskGetTickCount();

        do_output(buffer);

        vTaskDelayUntil(&xTimeTaskStarted, *taskPeriod);
    }
}

void do_output(RTES_Buffer_t *buffer) {
    RTES_Sample_t sample = read_sample(buffer);

    output_sample(sample);
}

RTES_Sample_t read_sample(RTES_Buffer_t *buffer) {
    RTES_Sample_t sample = readFromBuffer(buffer, 0);
    removeFromBuffer(buffer);
    return sample;
}

void output_sample(RTES_Sample_t sample) {
    double data = sample.data;

    /* Output data here */
}
