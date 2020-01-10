#include "recognize.h"

void vRTES_Recognize(void *pvParameters) {
    RTES_TaskParameters param = *(RTES_TaskParameters*) pvParameters;
    RTES_Buffer_t *in_buffer = param.in_buffer;
    RTES_Buffer_t *out_buffer = param.out_buffer;
    const char * const pcTaskName = param.pcTaskName;
    TickType_t *taskPeriod = param.xTaskPeriod;

    TickType_t xTimeTaskStarted;
    for (;;) {
        xTimeTaskStarted = xTaskGetTickCount();

        do_recognize(in_buffer, out_buffer);

        vTaskDelayUntil(&xTimeTaskStarted, *taskPeriod);
    }
}

void do_recognize(RTES_Buffer_t *in_buffer, RTES_Buffer_t *out_buffer) {
    const size_t frames_in_segment = R_FRAMES_SEGMENT;
    const unsigned long lower_limit_noise = R_LOWER_LIMIT_NOISE;   
    static size_t index_begin = 0, index_end = 0;
    static bool begin_recognized = false, end_recognized = false;
    static unsigned long prev_average = 0;
    static unsigned long average_start_noise = 0; 

    if (end_recognized) {
        begin_recognized = end_recognized = false;

        /* Noise was recognized, send a copy to the Noise Cancel Task */
        for(size_t i = index_begin; i <= index_end; i++) {
            /* in_buffer copy to out_buffer insertIntoBuffer */
        }
    } else if (begin_recognized) {
        /* Next segment should be checked for end of noise */
        unsigned long average = calculate_average_segment(in_buffer, 0, frames_in_segment);

        /* Does this segment contain the end of noise? */
        if (recognize_end_of_noise(average, average_start_noise)) {
            index_end = in_buffer->index_first + frames_in_segment;
            end_recognized = true;
        }

        prev_average = average;
    } else {
        /* Next segment should be checked for begin of noise */
        unsigned long average = calculate_average_segment(in_buffer, lower_limit_noise, frames_in_segment);
        
        /* Does this segment contain the begin of noise? */
        if (recognize_begin_of_noise(average, prev_average)) {
            begin_recognized = true;
            index_begin = in_buffer->index_first;
            average_start_noise = average;
        }

        prev_average = average;
    }
}

unsigned long calculate_average_segment(RTES_Buffer_t *buffer, unsigned long lower_limit_noise, const size_t size) {
    unsigned long sum = 0, counter = 0;

    for (size_t i = 0; i < size; i++) {
        RTES_Sample_t data = readFromBuffer(buffer, i);
        unsigned long value = (unsigned long)fabs(data.data); 
        
        /* Value below the lower limit are assumed to be irrelevant */
        if (value > lower_limit_noise) {
            sum += value;
            counter++;
        }      
    }

    if (counter == 0) {
        return 0;
    } else {
        return sum / counter;
    }
}

bool recognize_begin_of_noise(unsigned long average, unsigned long prev_average) {
    if ((average > prev_average * 1.5) && prev_average > 500) {
        /* If true then the noise started at index_first of buffer */
        return true;
    } else return false;
}

bool recognize_end_of_noise(unsigned long average, unsigned long prev_average) {
    if (average <= prev_average * 1.5) {
        /* If true then the noise ended at index_first + frames_in_segment of buffer */
        return true;
    } else return false;
}
