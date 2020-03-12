#ifndef RECOGNIZE_H
#define RECOGNIZE_H

#include "../RTES.h"
#include "../data.h"

#include <stdbool.h>
#include <limits.h>

typedef struct {
    baseSettings_t base;
    buffer_t *inBuffer;
    buffer_t *outBuffer;
    size_t samplesInSegment;
    sample_t lowerLimitRecognizeBegin;
    sample_t lowerLimitRecognizeEnd;
} recognizeSettings_t;

void vTaskRecognize(void *pvParameters);
void doRecognize(recognizeSettings_t *settings);

#endif /* RECOGNIZE_H */
