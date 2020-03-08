#include "recognize.h"

unsigned long long calculateAverage();

void vTaskRecognize(void *pvParameters) {
    taskInfo_t *info = (taskInfo_t*) pvParameters;
    recognizeSettings_t *settings = (recognizeSettings_t*) info->settings;
    
    TickType_t xTimeTaskStarted;
    for (;;) {
        xTimeTaskStarted = xTaskGetTickCount();

        doRecognize(settings);

        vTaskDelayUntil(&xTimeTaskStarted, info->xTaskPeriod);
    }
}

void doRecognize(recognizeSettings_t *settings) {
    
}
unsigned long long calculateAverage() {
    unsigned long long sum = 0;
    
    // maximum value of unsigned long long: (ULLONG_MAX)
}
