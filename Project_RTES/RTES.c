#include "RTES.h"

/* Create the buffer, with a RTES_Sample_t array of size long */
RTES_Buffer_t createBuffer(size_t size) {
    void *tmp  = malloc(size * sizeof(RTES_Sample_t));
    
    /* Malloc returns NULL in case of failure */
    if (tmp == NULL) {
        printf("Error in initBuffer: malloc failed to allocate memory!");
        free(tmp);
        exit(EXIT_FAILURE);
    }
    
    RTES_Buffer_t buffer = {
        .samples = (RTES_Sample_t *)tmp,
        .index_first = 0,
        .index_next = 0,
        .used = 0,
        .size = size
    };

    return buffer;
}

/* Increment the index by n, rollover is handled by applying '% size' */
size_t incrementIndexWithRollover(size_t base, size_t size, size_t n) {
    return ((base + n) % size);
}

/* Insert a sample into index index_next of the samples buffer */
void insertIntoBuffer(RTES_Buffer_t *buffer, RTES_Sample_t sample) {
    if (buffer->used == buffer->size) {
        printf("Error in insertIntoBuffer: trying to insert into a full buffer!");
        exit(EXIT_FAILURE);
    }

    buffer->samples[buffer->index_next] = sample;
    buffer->used++;    
    buffer->index_next = incrementIndexWithRollover(buffer->index_next, buffer->size, 1);
}

/* Get a sample from the buffer at a offset relative to index_first,
    note index_first is not updated since samples may be read multiple times */
RTES_Sample_t readFromBuffer(RTES_Buffer_t *buffer, size_t offset) {
    if (buffer->used == 0) {
        printf("Error in readFromBuffer: trying to read from an empty buffer!");
        exit(EXIT_FAILURE);
    }

    size_t index = incrementIndexWithRollover(buffer->index_first, buffer->size, offset);

    return buffer->samples[index];
}

/* 'Remove' from the buffer by updating index_first,
    note the sample will remain in the buffer until overwritten */
void removeFromBuffer(RTES_Buffer_t *buffer) {
    if (buffer->used == 0) {
        printf("Error in removeFromBuffer: trying to remove items from an empty buffer!");
        exit(EXIT_FAILURE);
    }

    buffer->index_first = incrementIndexWithRollover(buffer->index_first, buffer->size, 1);
    buffer->used--;
}

/* 'Remove' x items from the buffer by updating index_first,
    note the sample will remain in the buffer until overwritten */
void removeItemsFromBuffer(RTES_Buffer_t *buffer, size_t amount) {
    if (amount > buffer->used) {
        printf("Error in removeItemsFromBuffer: trying to remove more items than the buffer contains!");
        exit(EXIT_FAILURE);
    } 

    /* Since 'remove' only updates the index_first, the index can be updated at once */ 
    buffer->index_first = incrementIndexWithRollover(buffer->index_first, buffer->size, amount);
}

void freeBuffer(RTES_Buffer_t *buffer) {
    free(buffer->samples);
}
