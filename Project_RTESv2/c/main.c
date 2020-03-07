#include "Input/input.h"
#include "Output/output.h"

#include "RTES.h"
#include <stdio.h>

task_info_input_t get_task_info_input(buffer_t *out_buffer);
task_info_output_t get_task_info_output(buffer_t *in_buffer,
                                        FILE *fp_output_csv);

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

task_info_input_t get_task_info_input(buffer_t *out_buffer) {
    task_info_input_t task_info = {
        .basic_info = {
            .pcTaskName = "Input Task",
            .xTaskPeriod = 500
        },
        .out_buffer = out_buffer
    };
    return task_info;
}

task_info_output_t get_task_info_output(buffer_t *in_buffer,
                                        FILE *fp_output_csv) {
    task_info_output_t task_info = {
        .basic_info = {
            .pcTaskName = "Output Task",
            .xTaskPeriod = 500
        },
        .in_buffer = in_buffer, 
        .fp_output_csv = fp_output_csv
    };
    return task_info;
}
