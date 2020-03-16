#include "output.h"

sample_t readSample(buffer_t *buffer);
void outputSample(sample_t sample, FILE *fpOutput);

void vTaskOutput(void *pvParameters) {
    outputSettings_t *settings = (outputSettings_t*) pvParameters;

    TickType_t xTimeTaskStarted;
    for (;;) {
        xTimeTaskStarted = xTaskGetTickCount();

        doOutput(settings);

        vTaskDelayUntil(&xTimeTaskStarted, settings->base.xTaskPeriod);
    }
}

void doOutput(outputSettings_t *settings) {
    outputSample(readSample(settings->inBuffer), settings->fpOutput);
}

sample_t readSample(buffer_t *buffer) {
    if (buffer->used == 0) {
        return 0;
    } else {
        sample_t sample = readFromBuffer(buffer, 0);
        removeFromBuffer(buffer, 1);
        return sample;
    }    
}

void outputSample(sample_t sample, FILE *fpOutput) {
    fprintf(fpOutput, "%d,", sample);
}
