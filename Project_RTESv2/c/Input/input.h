#ifndef INPUT_H
#define INPUT_H

#include "../RTES.h"
#include "../data.h"
#include <stddef.h>

void vRTES_Input(void *pvParameters);
void do_input(buffer_t *buffer);

typedef struct {
    basic_task_info_t basic_info;
    buffer_t *out_buffer;
} task_info_input_t;

#endif /* INPUT_H */

