#include "RTES.h"

/* Create the buffer, with a RTES_Sample_t array of size long */
RTES_Buffer_t createBuffer(size_t size) {
    void *tmp  = malloc(size * sizeof(RTES_Sample_t));
    
    /* Malloc returns NULL in case of failure */
    if (tmp == NULL) {
        printf("Error in createBuffer: malloc failed to allocate %zu"
               " bytes of memory.", (size * sizeof(RTES_Sample_t)));
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
        printf("Error insertIntoBuffer: trying to insert into a full"
        " buffer.");
        exit(EXIT_FAILURE);
    }

    buffer->samples[buffer->index_next] = sample;
    buffer->used++;    
    buffer->index_next = incrementIndexWithRollover(buffer->index_next, 
                                                    buffer->size, 1);
}

/* Get a sample from the buffer at an offset relative to index_first,
    note index_first is not updated as samples are read multiple times */
RTES_Sample_t readFromBuffer(RTES_Buffer_t *buffer, size_t offset) {
    if (buffer->used == 0) {
        printf("readFromBuffer: trying to read from an empty buffer.");
        exit(EXIT_FAILURE);
    }

    size_t index = incrementIndexWithRollover(buffer->index_first, 
                                              buffer->size, offset);

    return buffer->samples[index];
}

/* 'Remove' from the buffer by updating index_first,
    note the sample will remain in the buffer until overwritten */
void removeFromBuffer(RTES_Buffer_t *buffer) {
    if (buffer->used == 0) {
        printf("removeFromBuffer: trying to remove from an empty buffer.");
        exit(EXIT_FAILURE);
    }

    buffer->index_first = incrementIndexWithRollover(buffer->index_first, 
                                                     buffer->size, 1);
    buffer->used--;
}

/* 'Remove' x items from the buffer by updating index_first,
    note the sample will remain in the buffer until overwritten */
void removeItemsFromBuffer(RTES_Buffer_t *buffer, size_t amount) {
    if (amount > buffer->used) {
        printf("removeItemsFromBuffer: trying to remove %zu items from the"
        " the buffer, but the buffer only contains %zu items.", amount,
        buffer->used);
        exit(EXIT_FAILURE);
    } 

    /* Since 'remove' only updates the index_first, the index can be updated at once */ 
    buffer->index_first = incrementIndexWithRollover(buffer->index_first, buffer->size, amount);
}

/* Copies n samples from src buffer to dest buffer */
void copyBuffer(RTES_Buffer_t *dest, RTES_Buffer_t *src, size_t n) {
    if (n > src->used) {
        printf("copyBuffer: trying to copy %zu items from the buffer, but"
        " the buffer only contains %zu items.", n, src->used);
        exit(EXIT_FAILURE);
    }

    if (n > dest->size) {
        printf("copyBuffer: trying to copy %zu items from the buffer, but"
        " the dest buffer only has %zu size.", n, dest->size);
        exit(EXIT_FAILURE);

    }
        
    for (size_t i = 0; i < src->used; i++) {
        insertIntoBuffer(dest, readFromBuffer(src, i));    
    }
}

void freeBuffer(RTES_Buffer_t *buffer) {
    free(buffer->samples);
}
