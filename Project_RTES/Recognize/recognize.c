#include "recognize.h"

void vRTES_Recognize(void *pvParameters) {
    RTES_TaskParameters param = *(RTES_TaskParameters*) pvParameters;
    RTES_Buffer_t *buffer = param.in_buffer;
    const char * const pcTaskName = param.pcTaskName;
    TickType_t *taskPeriod = param.xTaskPeriod;

    TickType_t xTimeTaskStarted;
    for (;;) {
        xTimeTaskStarted = xTaskGetTickCount();

        do_recognize(buffer);

        vTaskDelayUntil(&xTimeTaskStarted, *taskPeriod);
    }
}

void do_recognize(RTES_Buffer_t *buffer) {
    static size_t index_begin = 0, index_end = 0;
    static bool begin_recognized = false;
    
    if (begin_recognized) {
        /* Next segment should be checked for end of noise */
        average = calculate_average_segment(buffer, SIZE_SEGMENT);

        /* Does this segment contain the end of noise? */
        if (recognize_end_of_noise()) {

        }
    } else {
        static unsigned long average = 0, prev_average = 0;
    
        /* Next segment should be checked for begin of noise */
        average = calculate_average_segment(buffer, SIZE_SEGMENT);
        
        /* Does this segment contain the begin of noise? */
        if (recognize_begin_of_noise(average, prev_average)) {
            begin_recognized = true;
            index_begin = buffer->index_first;
        } else {
            prev_average = average;
        }
    }
}

unsigned long calculate_average_segment(RTES_Buffer_t *buffer, size_t size) {
    unsigned long sum = 0, counter = 0;
    for (size_t i = 0; i < SIZE_SEGMENT; i++) {
        RTES_Sample_t data = readFromBuffer(buffer, i);
        unsigned long value = (unsigned long)fabs(data.data); 
        
        /* Value below the lower limit are assumed to be irrelevant */
        if (value > LOWER_LIMIT) {
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
    unsigned long safezone = prev_average / 2;
    if ((average > prev_average + safezone) && prev_average > 500) {
        /* If true then the noise started at index_first of buffer */
        return true;
    }
    return false;
}

bool recognize_end_of_noise(void) {

}

