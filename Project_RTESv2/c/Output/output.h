#ifndef OUTPUT_H
#define OUTPUT_H

#include "../RTES.h"
#include "../data.h"
#include <stddef.h>
#include <stdio.h>

void vRTES_Output(void *pvParameters);
void do_output(buffer_t *buffer, FILE *fp_output_csv);

typedef struct {
    basic_task_info_t basic_info;
    buffer_t *in_buffer;
    FILE *fp_output_csv;
} task_info_output_t;

#endif /* OUTPUT_H */
