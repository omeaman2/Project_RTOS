#ifndef NOISECANCEL_H
#define NOISECANCEL_H

#include "../RTES.h"

void vRTES_Noisecancel(void *pvParameters);
void do_noisecancel(RTES_Buffer_t *in_buffer, RTES_Buffer_t *out_buffer);

#endif /* NOISECANCEL_H */
