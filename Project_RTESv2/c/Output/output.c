#include "output.h"

sample_t readSample(buffer_t *buffer);
void outputSample(sample_t sample, FILE *fpOutput);

void vTaskOutput(void *pvParameters) {
    taskInfo_t *info = (taskInfo_t*) pvParameters;
    outputSettings_t *settings = (outputSettings_t*) info->settings;

    TickType_t xTimeTaskStarted;
    for (;;) {
        xTimeTaskStarted = xTaskGetTickCount();

        doOutput(settings);

        vTaskDelayUntil(&xTimeTaskStarted, info->xTaskPeriod);
    }
}

void doOutput(outputSettings_t *settings) {
    outputSample(readSample(settings->inBuffer), settings->fpOutput);
}

sample_t readSample(buffer_t *buffer) {
    sample_t sample = readFromBuffer(buffer, 0);
    removeFromBuffer(buffer, 1);
    return sample;
}

void outputSample(sample_t sample, FILE *fpOutput) {
    fprintf(fpOutput, "%d,", sample);
}
