#include "output.h"

sample_t read_sample(buffer_t *buffer);
void output_sample(sample_t sample, FILE *fp_output_csv);

void vRTES_Output(void *pvParameters) {
    task_info_output_t task_info = *(task_info_output_t*) pvParameters;
    buffer_t *buffer = task_info.in_buffer;
    const char * const pcTaskName = task_info.basic_info.pcTaskName;
    TickType_t taskPeriod = task_info.basic_info.xTaskPeriod;
    FILE *fp_output_csv = task_info.fp_output_csv;

    TickType_t xTimeTaskStarted;
    for (;;) {
        xTimeTaskStarted = xTaskGetTickCount();

        do_output(buffer, fp_output_csv);

        vTaskDelayUntil(&xTimeTaskStarted, taskPeriod);
    }
}

void do_output(buffer_t *buffer, FILE *fp_output_csv) {
    output_sample(read_sample(buffer), fp_output_csv);
}

sample_t read_sample(buffer_t *buffer) {
    sample_t sample = readFromBuffer(buffer, 0);
    removeFromBuffer(buffer, 1);
    return sample;
}

void output_sample(sample_t sample, FILE *fp_output_csv) {
    fprintf(fp_output_csv, "%d,", sample);
}
