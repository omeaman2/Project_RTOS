#ifndef INPUT_H
#define INPUT_H

#include "../RTES.h"
#include "../data.h"
#include <stddef.h>

typedef struct {
    baseSettings_t base;
    buffer_t *outBuffer;
} inputSettings_t;

void vTaskInput(void *pvParameters);
void doInput(inputSettings_t *settings);

#endif /* INPUT_H */

