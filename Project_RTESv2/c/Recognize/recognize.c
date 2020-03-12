#include "recognize.h"

unsigned long long calculateAverage(sample_t array[], size_t sizeArray,
                                    sample_t lowerLimit);
void addWithOverflowCheck(unsigned long long *sum, sample_t value);
bool recognizeBegin(recognizeSettings_t *settings);
bool recognizeEnd(recognizeSettings_t *settings);

void vTaskRecognize(void *pvParameters) {
    recognizeSettings_t *settings = (recognizeSettings_t*) pvParameters;
    
    TickType_t xTimeTaskStarted;
    for (;;) {
        xTimeTaskStarted = xTaskGetTickCount();

        doRecognize(settings);

        vTaskDelayUntil(&xTimeTaskStarted, settings->base.xTaskPeriod);
    }
}

void doRecognize(recognizeSettings_t *settings) {
    static bool beginRecognized = false;
    if (!beginRecognized) {
        
    } else {

    }
}

bool recognizeBegin(recognizeSettings_t *settings) {
    sample_t *array = getArrayFromBuffer(settings->inBuffer,
                                         settings->samplesInSegment);    
}

bool recognizeEnd(recognizeSettings_t *settings) {
    
}

unsigned long long calculateAverage(sample_t array[], size_t sizeArray,
                                    sample_t lowerLimit) {
    unsigned long long sum = 0;
    size_t count = 0;    

    for (size_t i = 0; i < sizeArray; i++) {
        sample_t value = abs(array[i]);
        if (value > lowerLimit) {
            addWithOverflowCheck(&sum, value);
            count++;
        }
    }
    
    if (count == 0) {
        return 0;
    } else { 
        return sum / (unsigned long long) count;
    }
}

void addWithOverflowCheck(unsigned long long *sum, sample_t value) {
    if (ULLONG_MAX - (unsigned long long) value < *sum) {
            printf("Error in 'addWithOveflowCheck': addition exceeds"
                   " maximum value of unsigned long long!\n");
            exit(EXIT_FAILURE);
    }
    *sum = *sum + (unsigned long long) value;
}
