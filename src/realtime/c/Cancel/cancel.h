#ifndef CANCEL_H
#define CANCEL_H

#include "../RTES.h"

#include "../kissfft/kiss_fft.h"
#ifndef USE_TEMPFREERTOS
// The windows version only compiles if both .h and .c are included
#include "../kissfft/kiss_fft.c"
#endif /* USE_TEMPFREERTOS */

#include "../data.h"

typedef struct {
    baseSettings_t base;
    buffer_t *inBuffer;
    buffer_t *outBuffer;
    double cancelPercentage; //range [0, 100]
} cancelSettings_t;

void vTaskCancel(void *pvParameters);
void doCancel(cancelSettings_t *settings);

#endif /* CANCEL_H */
