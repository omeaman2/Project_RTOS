#ifndef RECOGNIZE_H
#define RECOGNIZE_H

#include <math.h> /* used for fabs() */
#include <stdbool.h>
#include <stddef.h>
#include "../RTES.h"

#define SIZE_SEGMENT       (120)
#define LOWER_LIMIT        (500)

void vRTES_Recognize(void *pvParameters);
void do_recognize(RTES_Buffer_t *buffer);
unsigned long calculate_average_segment(RTES_Buffer_t *buffer, size_t size);
bool recognize_begin_of_noise(unsigned long average, unsigned long prev_average);
bool recognize_end_of_noise(void);

#endif /* RECOGNIZE_H */
