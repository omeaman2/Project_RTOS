#include "recognize.h"

unsigned long long calculateAverage(sample_t array[], size_t sizeArray, 
                                                    sample_t lowerLimit);
void addWithOverflowCheck(unsigned long long *sum, sample_t value);
bool recognizeBegin(recognizeSettings_t *settings,
                    unsigned long long *previousAverage);
bool recognizeEnd(recognizeSettings_t *settings,
                  unsigned long long *previousAverage, 
                  size_t samplesChecked);

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
    static unsigned long long previousAverage = 0;
    static size_t samplesChecked = 0;   

    if (!beginRecognized) {
        if (recognizeBegin(settings, &previousAverage)) {
            getchar();
            samplesChecked += settings->segmentSize;
            beginRecognized = true;
        } else {
            //Remove the current segment from the inBuffer
            removeFromBuffer(settings->inBuffer, settings->segmentSize);
        } 
    } else {
        if (recognizeEnd(settings, &previousAverage, samplesChecked)) {
            getchar();

            // Noise is considered to end at the end of this segment
            samplesChecked += settings->segmentSize;
            
            // Copy the noise to the outBuffer for the Cancel Task
            copyBuffer(settings->outBuffer, settings->inBuffer,
                                            samplesChecked);

            // Remove the noise from the inBuffer
            removeFromBuffer(settings->inBuffer, samplesChecked);
            
            // Reset the variables, next period the task will start 
            // searching for the next begin of noise again
            beginRecognized = false;
            samplesChecked = 0;
        } else {
            samplesChecked += settings->segmentSize;
            if (samplesChecked >= settings->maxSamplesNoise) {
                // Maximum size of the noise has been exceeded, assume last
                // recognize begin was a false positive.
                beginRecognized = false;
                samplesChecked = 0;
            }
        }
    }
}

bool recognizeBegin(recognizeSettings_t *settings, 
                    unsigned long long *previousAverage) {
    sample_t array[settings->segmentSize];
    copyArrayFromBuffer(array, settings->inBuffer, settings->segmentSize,
                                                   0); 
    unsigned long long average = calculateAverage(array, 
                                                settings->segmentSize,
                                                settings->lowerLimitBegin);
    
    if (*previousAverage != 0 && //TODO: better way to exclude first period
        average > *previousAverage * settings->factorIncreaseBegin) {
        *previousAverage = average;
        return true;
    } else {
        *previousAverage = average;
        return false;
    }               
}

// For recognizeEnd previousAverage revers to the average when the
// begin of noise was first recognized, as such it's not replaced
// if the end of noise is not found
bool recognizeEnd(recognizeSettings_t *settings,
                  unsigned long long *previousAverage,
                  size_t samplesChecked) {
    sample_t array[settings->segmentSize];
    copyArrayFromBuffer(array, settings->inBuffer, settings->segmentSize,
                                                   samplesChecked); 
    unsigned long long average = calculateAverage(array, 
                                                  settings->segmentSize,
                                                  settings->lowerLimitEnd);
        
    if (average < *previousAverage * settings->factorDecreaseEnd) {
        *previousAverage = average;
        return true;
    } else {
        return false;
    }               
}

/* Calculates the average of sizeArray items of the array, 
    excluding values below lowerLimit */
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
