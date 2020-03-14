#ifndef RECOGNIZE_H
#define RECOGNIZE_H

#include "../RTES.h"
#include "../data.h"

#include <stdbool.h>
#include <limits.h>

typedef struct {
    // Basic information of the task
    baseSettings_t base;
    // Buffer between the Input and the Recognize Task
    buffer_t *inBuffer;
    // Buffer between the Recognize and the Cancel Task
    buffer_t *outBuffer;
    // Amount of samples in a segment
    size_t segmentSize;
    // Maximum amount of samples the noise may contain, if the end of
    // noise is not found before this number is reached than a false
    // positive is assumed to have occured in recognizeBegin
    size_t maxSamplesNoise;
    // Values below this are excluded in the recognizeBegin average()
    sample_t lowerLimitBegin;
    // Values below this are excluded in the recognizeEnd average()
    sample_t lowerLimitEnd;
    // An increase of this factor or higher may be the begin of noise
    float factorIncreaseBegin;
    // An decrease of this factor or lower may be the end of noise
    float factorDecreaseEnd;
} recognizeSettings_t;

void vTaskRecognize(void *pvParameters);
void doRecognize(recognizeSettings_t *settings);

#endif /* RECOGNIZE_H */
