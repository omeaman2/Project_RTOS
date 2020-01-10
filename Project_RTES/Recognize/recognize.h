#ifndef RECOGNIZE_H
#define RECOGNIZE_H

#include <math.h> /* used for fabs() */
#include <stdbool.h>
#include <stddef.h>
#include "../RTES.h"
#include "../Timing.h"

/* Move 1/x part of the segment for every period */
#define R_MOVE_PART_OF_SEGMENT          (4)
/* The factor between the period of the input and recognize task */ 
#define R_FACTOR_INPUT_RECOGNIZE        (T_RECOGNIZE_PERIOD / T_IO_PERIOD)
/* The amount of frames in one segment */
#define R_FRAMES_SEGMENT                (R_FACTOR_INPUT_RECOGNIZE * R_MOVE_PART_OF_SEGMENT)

/* What's the lower limit of what could be part of the noise */
#define R_LOWER_LIMIT_NOISE              (500)

void vRTES_Recognize(void *pvParameters);
void do_recognize(RTES_Buffer_t *in_buffer, RTES_Buffer_t *out_buffer);
unsigned long calculate_average_segment(RTES_Buffer_t *buffer, unsigned long lower_limit_noise, size_t size);
bool recognize_begin_of_noise(unsigned long average, unsigned long prev_average);
bool recognize_end_of_noise(unsigned long average, unsigned long prev_average);

#endif /* RECOGNIZE_H */
