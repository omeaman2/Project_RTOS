#ifndef INPUT_H
#define INPUT_H

#include <stddef.h>
#include "../RTES.h"
#include "../data.h"

void vRTES_Input(void *pvParameters);
void do_input(RTES_Buffer_t *buffer);
RTES_Sample_t take_sample(void);

#endif /* INPUT_H */
