#ifndef RECOGNIZE_H
#define RECOGNIZE_H

#include <math.h> /* used for fabs() */
#include <stdbool.h>
#include <stdio.h> /* used for temporary file usages */
#include <stddef.h>
#include "../RTES.h"
#include "../Timing.h"

/* Note there are 8000 frames in one second, the task should run every
   20ms which means the segment should contain atleast 160 frames. This
   means the task also needs to ensure that on average 160 frames are
   removed every period. (Ofcourse we can't delete frames we haven't
   received yet) */
static const size_t frames_in_segment = 480;
static const size_t move_frames_period = 160;

/* When the average exceeds the previous average multiplied by the factor 
    below a large increase is assumed to have occured */
static const float factor_large_increase = 1.5F;
static const float factor_large_decrease = 0.5F;

void vRTES_Recognize(void *pvParameters);
bool do_recognize(RTES_Buffer_t *in_buffer, RTES_Buffer_t *out_buffer, FILE *f);

#endif /* RECOGNIZE_H */
