#include "RTES.h"

/* Source used for buffers:
    https://stackoverflow.com/questions/3536153/c-dynamically-growing-array */

/* Initiate the buffer with a samples buffer of the given size */
void initBuffer(RTES_Buffer_t *buffer, size_t size) {
    buffer->samples = (RTES_Sample_t *)malloc(size * sizeof(RTES_Sample_t));
    buffer->index_first = buffer->index_next = 0;
    buffer->size = size;
}

/* Insert a sample into index index_next of the samples buffer */
void insertIntoBuffer(RTES_Buffer_t *buffer, RTES_Sample_t sample) {
    buffer->samples[buffer->index_next] = sample;
    
    /* Should index_next point to index 0 after incrementing? */
    if (buffer->index_next == buffer->size - 1) {
        buffer->index_next = 0;
    } else {
        buffer->index_next++;
    }
}

/* Get a sample from the buffer at a offset relative to index_first,
    note index_first is not updated since samples may be read multiple times */
RTES_Sample_t readFromBuffer(RTES_Buffer_t *buffer, size_t offset) {
    size_t index;
    if (buffer->index_first + offset > buffer->size - 1) {
        index = buffer->index_first + offset - buffer->size;
    } else {
        index = buffer->index_first + offset;
    }

    return buffer->samples[index];
}

/* 'Remove' from the buffer by updating index_first,
    note the sample will remain in the buffer until overwritten */
void removeFromBuffer(RTES_Buffer_t *buffer) {
    /* Should index_first point to index 0 after incrementing? */
    if (buffer->index_first == buffer->size - 1) {
        buffer->index_first = 0;
    } else {
        buffer->index_first++;
    }
}

void freeBuffer(RTES_Buffer_t *buffer) {
    free(buffer->samples);
    buffer->samples = NULL;
    buffer->index_first = buffer->index_next = buffer->size = 0;
}
