#include "noisecancel.h"

void vRTES_Noisecancel(void *pvParameters) {
    RTES_TaskParameters param = *(RTES_TaskParameters*) pvParameters;
    RTES_Buffer_t *in_buffer = param.in_buffer;
    RTES_Buffer_t *out_buffer = param.out_buffer;
    const char * const pcTaskName = param.pcTaskName;
    TickType_t *taskPeriod = param.xTaskPeriod;

    TickType_t xTimeTaskStarted;
    for (;;) {
        xTimeTaskStarted = xTaskGetTickCount();

        do_noisecancel(in_buffer, out_buffer);

        vTaskDelayUntil(&xTimeTaskStarted, *taskPeriod);
    }
}

void do_noisecancel(RTES_Buffer_t *in_buffer, RTES_Buffer_t *out_buffer) {
    
}
