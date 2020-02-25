#include "recognize.h"

unsigned long average_segment(RTES_Buffer_t *buffer, size_t size); 
bool is_begin_of_noise(unsigned long average, unsigned long prev_average);
bool is_end_of_noise(unsigned long average, unsigned long prev_average);

void vRTES_Recognize(void *pvParameters) {
    RTES_TaskParameters param = *(RTES_TaskParameters*) pvParameters;
    RTES_Buffer_t *in_buffer = param.in_buffer;
    RTES_Buffer_t *out_buffer = param.out_buffer;
    //const char * const pcTaskName = param.pcTaskName;
    TickType_t taskPeriod = *param.xTaskPeriod;

    TickType_t xTimeTaskStarted;
    for (;;) {
        xTimeTaskStarted = xTaskGetTickCount();

        do_recognize(in_buffer, out_buffer, NULL);

        vTaskDelayUntil(&xTimeTaskStarted, taskPeriod);
    }
}

//#define DONT_PRINT
//#define DONT_PRINT_FILE
bool do_recognize(RTES_Buffer_t *in_buffer, RTES_Buffer_t *out_buffer, FILE *f) {
    static size_t index_begin = 0, index_end = 0;
    static bool begin_recognized = false;
    static unsigned long prev_average = 0, average_start_of_noise = 0;
    static size_t frames_since_begin_recognized = 0;   
    
    /* Calculates the average of the current segment */
    unsigned long average = average_segment(in_buffer, frames_in_segment);
 
#ifndef DONT_PRINT_FILE
    fprintf(f, "%lu,", average);
#endif

    if (!begin_recognized) {
#ifndef DONT_PRINT
        const char message[] = "Begin not recognized:";
        printf("%21s"
               " [average: %6lu,"
               " prev_average: %6lu]\n", 
               message, average, prev_average);
#endif

        /* Look for the begin of noise */
        /* If found index_begin is the first index of this segment */
        if (prev_average != 0 && is_begin_of_noise(average, prev_average)) {
            begin_recognized = true;
            index_begin = in_buffer->index_first;
            average_start_of_noise = average;
            frames_since_begin_recognized = 0;
        } else {
            /* If NOT found we can delete the first part of the segment */
            removeItemsFromBuffer(in_buffer, move_frames_period); 
        }
    } else {
#ifndef DONT_PRINT
        const char message[] = "Begin recognized:";
        printf("%21s"
               " [average: %6lu,"
               " prev_average: %6lu]\n",
               message, average, prev_average);
#endif
        /* Look for the end of noise */
        /* If found index_end is index is the last index of this segment */
        if (is_end_of_noise(average, average_start_of_noise)) {
#ifndef DONT_PRINT
            const char message[] = "End recognized:";
            printf("%21s"
                   " [average: %6lu,"
                   "       frames: %6zu]\n",
                   message, average_start_of_noise, 
                   frames_since_begin_recognized);
#endif    
            index_end = in_buffer->index_first + move_frames_period;
           
            /* Copy the noise to out_buffer */
            copyBuffer(out_buffer, in_buffer, (index_end - index_begin));
            
            /* Then remove the current noise from the buffer */
            removeItemsFromBuffer(in_buffer, (index_end - index_begin));
            
            /* We can start searching for the next noise again */
            begin_recognized = false;
            
            /* Solely used for testing, remove return later */
            return true;
        } else {
            /* If NOT found we can increment the count of frames we checked
               since recognizing the noise */
            frames_since_begin_recognized += move_frames_period; 
        }
    }

    prev_average = average;    
    return false;
}


/*************************************************************************/
void do_recognize(RTES_Buffer_t *in_buffer, RTES_Buffer_t *out_buffer,
                  FILE *f) {
    static size_t index_begin = 0, index_end = 0;
    static bool begin_recognized = false;
    static unsigned long prev_average = 0, average_start_of_noise = 0;
    static size_t frames_since_begin_recognized = 0;
    
    /* Calculate the average of the current segment */
    unsigned long average = average_segment(in_buffer, frames_in_segment);

    if (begin_recognized) {
        /* Look for the end of the noise */
        if (recognize_end(average, average_start_of_noise)) {
            print_end_message("End recognized:", average, 
                              average_start_of_noise, 
                              frames_since_begin_recognized);

            /* Set index_end to the end of the current segment */ 
            /* TODO: This might actually be the index of the first value
                outside of this segment. Check this. */
            index_end = in_buffer->index_first + move_frames_period;

            /* Copy the noise to out_buffer */
            copyBuffer(out_buffer, in_buffer, (index_end - index_begin));

            /* Remove the noise from the current buffer */
            removeItemsFromBuffer(in_buffer, (index_end - index_begin));

            begin_recognized = false;
        } else {
            print_end_message("End not recognized:", average,
                              average_start_of_noise,
                              frames_since_begin_recognized);
        }
    } else {
        /* Look for the begin of the noise */
        if (begin_recognized = recognize_begin(average, prev_average)) {
            print_message("Begin recognized:", average, prev_average);
            index_begin = in_buffer->index_first;
            average_start_of_noise = average;
            frames_since_begin_recognized = 0; 
        } else {
            print_message("Begin not recognized:", average, prev_average);
            /* If NOT found we can delete the first part of the segment */
            removeItemsFromBuffer(in_buffer, move_frames_period);
        }
    }
}

bool recognize_begin(unsigned long average, unsigned long prev_average) {
    if (prev_average != 0 && is_begin_of_noise(average, prev_average)) { 
         return true;
    else return false;
}

/* Checks whether a large increase of the average value has occured. */
bool is_begin_of_noise(unsigned long average, unsigned long prev_average) {
    if (average > (prev_average * factor_large_increase))
         return true;
    else return false;
}    

bool recognize_end(unsigned long average, 
                   unsigned long average_start_of_noise) {
    if (is_end_of_noise(average, average_start_of_noise)) {
         return true;
    else return false;
}

/* Checks whether a large decrease of the average value has occured. */
bool is_end_of_noise(unsigned long average, unsigned long prev_average) {
    if (average < (prev_average * factor_large_decrease))
         return true;
    else return false;
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

void print_message(const char msg[], unsigned long average, 
                   unsigned long prev_average) {
    printf("%21s"
           " [average: %6lu,"
           " prev_average: %6lu]\n",
           msg, average, prev_average);
}

void print_end_message(const char msg[], unsigned long average,
                       unsigned long average_start_of_noise, 
                      size_t frames_since_begin_recognized) {
    printf("%21s"
           " [average: %6lu,"
           " average_start_of_noise: %6lu,"
           "       frames: %6zu]\n",
           msg, average, average_start_of_noise, 
           frames_since_begin_recognized);
}
