#ifndef OUTPUT_H
#define OUTPUT_H

#include "../RTES.h"

void vRTES_Output(void *pvParameters);
void do_output(RTES_Buffer_t *buffer);
RTES_Sample_t read_sample(RTES_Buffer_t *buffer);
void output_sample(RTES_Sample_t sample);

#endif /* OUTPUT_H */
