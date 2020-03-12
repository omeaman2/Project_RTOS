#ifndef OUTPUT_H
#define OUTPUT_H

#include "../RTES.h"
#include "../data.h"
#include <stddef.h>
#include <stdio.h>

typedef struct {
    baseSettings_t base;
    buffer_t *inBuffer;
    FILE *fpOutput;
} outputSettings_t;

void vTaskOutput(void *pvParameters);
void doOutput(outputSettings_t *settings);

#endif /* OUTPUT_H */
