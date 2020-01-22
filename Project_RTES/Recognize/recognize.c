#include "recognize.h"

unsigned long average_segment(RTES_Buffer_t *buffer, size_t size); 
bool is_begin_of_noise(unsigned long average, unsigned long prev_average);
bool is_end_of_noise(unsigned long average, unsigned long prev_average);

void vRTES_Recognize(void *pvParameters) {
    RTES_TaskParameters param = *(RTES_TaskParameters*) pvParameters;
    RTES_Buffer_t *in_buffer = param.in_buffer;
    RTES_Buffer_t *out_buffer = param.out_buffer;
    const char * const pcTaskName = param.pcTaskName;
    TickType_t taskPeriod = *param.xTaskPeriod;

    TickType_t xTimeTaskStarted;
    for (;;) {
        xTimeTaskStarted = xTaskGetTickCount();

        do_recognize(in_buffer, out_buffer);

        vTaskDelayUntil(&xTimeTaskStarted, taskPeriod);
    }
}

void do_recognize(RTES_Buffer_t *in_buffer, RTES_Buffer_t *out_buffer) {
    static size_t index_begin = 0, index_end = 0;
    static bool begin_recognized = false;
    static unsigned long prev_average = 0, average_start_of_noise = 0;
    static size_t frames_since_begin_recognized = 0;   
 
    /* Calculates the average of the current segment */
    unsigned long average = average_segment(in_buffer, frames_in_segment);
    
    if (!begin_recognized) {
        /* Look for the begin of noise */
        /* If found index_begin is the first index of this segment */
        if (is_begin_of_noise(average, prev_average)) {
            begin_recognized = true;
            index_begin = in_buffer->index_first;
            average_start_of_noise = average;
            frames_since_begin_recognized = 0;
        } else {
            /* If NOT found we can delete the first part of the segment */
            removeItemsFromBuffer(in_buffer, move_frames_period); 
        }
    } else {
        /* Look for the end of noise */
        /* If found index_end is index is the last index of this segment */
        if (is_end_of_noise(average, average_start_of_noise)) {
            index_end = in_buffer->index_first + move_frames_period;
           
            /* Copy the noise to out_buffer */
            copyBuffer(out_buffer, in_buffer, (index_end - index_begin));
            
            /* Then remove the current noise from the buffer */
            removeItemsFromBuffer(in_buffer, (index_end - index_begin));
        } else {
            /* If NOT found we can increment the count of frames we checked
               since recognizing the noise */
            frames_since_begin_recognized += move_frames_period; 
        }
    }

    prev_average = average;    
}

/* Calculates the average of a segment of size long 
    Note it may be worth */
unsigned long average_segment(RTES_Buffer_t *buffer, size_t size) { 
    unsigned long sum = 0;

    for (size_t i = 0; i < size; i++) {
        RTES_Sample_t sample = readFromBuffer(buffer, i);
        sum += (unsigned long)fabs(sample.data); 
    }

    return sum / size;
}

/* Checks whether a large increase of the average value has occured. */
bool is_begin_of_noise(unsigned long average, unsigned long prev_average) {
    if (average > (prev_average * factor_large_increase))
         return true;
    else return false;
}    

/* Checks whether a large decrease of the average value has occured. */
bool is_end_of_noise(unsigned long average, unsigned long prev_average) {
    if (average < (prev_average * factor_large_decrease))
         return true;
    else return false;
}
