#include "Noisecancel/noisecancel.h"
#include "Recognize/recognize.h"
#include "RTES.h"
#include <stdio.h>

int main(void) {
    RTES_Buffer_t buffer = createBuffer(RTES_BUFFER_SIZE);

    /* Get some samples to insert */
    RTES_Sample_t sample_a = { 25.5f };
    RTES_Sample_t sample_b = { 35.5f };
    RTES_Sample_t sample_c = { 45.5f };
    RTES_Sample_t sample_d = { 55.5f };
    RTES_Sample_t sample_e = { 65.5f };
    
    insertIntoBuffer(&buffer, sample_a);
    insertIntoBuffer(&buffer, sample_b);
    insertIntoBuffer(&buffer, sample_c);
    insertIntoBuffer(&buffer, sample_d);
    insertIntoBuffer(&buffer, sample_e);

    for(int i = 0; i < 5; i++) {
        RTES_Sample_t sample = readFromBuffer(&buffer, i);
        printf("Test: %f\n", sample.data);
    }

    freeBuffer(&buffer);
    
    return 0;
}
