#include "RTES.h"
#include <stdio.h>

/* Tasks */
#include "Input/input.h"
#include "Recognize/recognize.h"
#include "Noisecancel/noisecancel.h"
#include "Output/output.h"

int main(void) {
    RTES_Buffer_t input_buffer = createBuffer(RTES_BUFFER_SIZE);
    RTES_Buffer_t cancel_buffer = createBuffer(RTES_BUFFER_SIZE);

    RTES_TaskParameters taskInput = {
        .pcTaskName = "",
        .in_buffer = NULL,
        .out_buffer = &input_buffer,
        .xTaskPeriod = 0
    };

    RTES_TaskParameters taskRecognize = {
        .pcTaskName = "",
        .in_buffer = &input_buffer,
        .out_buffer = &cancel_buffer,
        .xTaskPeriod = 0
    };

    /* Create Tasks */ 
    vRTES_Input((void*)&taskInput);
    vRTES_Recognize((void*)&taskRecognize);

    freeBuffer(&input_buffer);
    freeBuffer(&cancel_buffer);
    
    return 0;
}
