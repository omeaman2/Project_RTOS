#include "Input/input.h"
#include "Output/output.h"

#include "RTES.h"
#include <stdio.h>

taskInfo_t getTaskInfoInput(buffer_t *outBuffer);
taskInfo_t getTaskInfoOutput(buffer_t *inBuffer, FILE *fpOutput);

int main(void) {
    buffer_t buffer = createBuffer(100);
    
    for (size_t i = 0; i < 50; i++) {
        insertIntoBuffer(&buffer, 5);
    }   

    printStatusBuffer(&buffer);

    removeFromBuffer(&buffer, 50);
    printStatusBuffer(&buffer);
    
    for (size_t i = 0; i < 60; i++) {
        insertIntoBuffer(&buffer, 5);
    }
    printStatusBuffer(&buffer);   

    removeFromBuffer(&buffer, 10);
    printStatusBuffer(&buffer);
 
    return 0;  
   
}

taskInfo_t getTaskInfoInput(buffer_t *outBuffer) {
    inputSettings_t settings = {
        .outBuffer = outBuffer
    };

    taskInfo_t info = {
        .pcTaskName = "Input Task",
        .xTaskPeriod = 500,
        .settings = &settings
    };
    return info;
}

taskInfo_t getTaskInfoOutput(buffer_t *inBuffer, FILE *fpOutput) {
    outputSettings_t settings = {
        .inBuffer = inBuffer,
        .fpOutput = fpOutput
    };

    taskInfo_t info = {
        .pcTaskName = "Output Task",
        .xTaskPeriod = 500,
        .settings = &settings
    };
    return info;
}
