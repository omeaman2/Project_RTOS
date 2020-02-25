#include "RTES.h"
#include <stdio.h>

/* Tasks */
#include "Input/input.h"
#include "Recognize/recognize.h"
#include "Noisecancel/noisecancel.h"
#include "Output/output.h"

int main(void) {
    const size_t max_ticks = 3000000 + 1;    
    RTES_Buffer_t input_buffer  = createBuffer(max_ticks);
    RTES_Buffer_t cancel_buffer = createBuffer(max_ticks);

    RTES_TaskParameters taskInput = {
        .pcTaskName = "taskInput",
        .in_buffer = NULL,
        .out_buffer = &input_buffer,
        .xTaskPeriod = 0
    };

    RTES_TaskParameters taskRecognize = {
        .pcTaskName = "taskRecognize",
        .in_buffer = &input_buffer,
        .out_buffer = &cancel_buffer,
        .xTaskPeriod = 0
    };

    ///* Create Tasks */ 
    //vRTES_Input((void*)&taskInput);
    //vRTES_Recognize((void*)&taskRecognize);

#ifndef DONT_PRINT_FILE
    FILE *f = fopen("average.csv", "w");

    if (f == NULL) {
        printf("Couldn't open file!");
        freeBuffer(&input_buffer);
        freeBuffer(&cancel_buffer);
        exit(EXIT_FAILURE);
    }
#endif

    for (size_t i = 1; i < max_ticks; i++) {
        if ((i %    1) == 0) {
            do_input(taskInput.out_buffer);
        }
        if ((i %  160) == 0) {
            if (do_recognize(taskRecognize.in_buffer,
                             taskRecognize.out_buffer, f)) {
                printf("Recognized finished at %zu\n", i);
                return 0;
            }
        }
        //printf("%zu\n", i);
    }

#ifndef DONT_PRINT_FILE
    fclose(f);
#endif   
 
    freeBuffer(&input_buffer);
    freeBuffer(&cancel_buffer);
    
    return 0;
}
