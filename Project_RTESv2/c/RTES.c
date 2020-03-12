#include "RTES.h"

buffer_t createBuffer(size_t size) {
    sample_t *array = malloc(size * sizeof(sample_t));
    
    if (array == NULL) {
        printf("Error in 'createBuffer': malloc failed to allocate %zu"
               " bytes of memory.\n", (size * sizeof(sample_t)));
        exit(EXIT_FAILURE);
    }

    if (size % 100 != 0) {
        printf("Warning in 'createBuffer': to ensure printStatusBuffer"
               " works correctly a multiple of 100 should be used for the"
               " size.\n");
    }
    
    buffer_t buffer = {
        .data = (sample_t *)array,
        .read = 0,
        .write = 0,
        .used = 0,
        .size = size
    };

    return buffer;
}

// Increment the index by 'n', rollover if index+n exceeds max.
void updateIndex(size_t *index, size_t n, size_t max) {
    *index = (*index + n) % max;
}

// Insert a sample into index buffer->write.
void insertIntoBuffer(buffer_t *buffer, sample_t data) {
    if (buffer->used == buffer->size) {
        printf("Error in 'insertIntoBuffer': trying to insert into a"
               " full buffer.\n");
        exit(EXIT_FAILURE);
    }

    buffer->data[buffer->write] = data;
    buffer->used++;
    updateIndex(&buffer->write, 1, buffer->size);
}

// Read a sample from a index relative to buffer->read.
sample_t readFromBuffer(buffer_t *buffer, size_t offset) {
    if (buffer->used == 0) {
        printf("Error in 'readFromBuffer': trying to read from an empty"
               " buffer.\n");
        exit(EXIT_FAILURE);
    } 

    if (offset > buffer->used - 1) {
        printf("Error in 'readFromBuffer': trying to read from an invalid"
               " index.\n");
        exit(EXIT_FAILURE);
    }

    size_t index = buffer->read;
    updateIndex(&index, offset, buffer->size);
    return buffer->data[index];
}

// Remove 'n' samples from the buffer by moving the read index.
void removeFromBuffer(buffer_t *buffer, size_t n) {
    if (n > buffer->used) {
        printf("Error in 'removeFromBuffer': trying to remove %zu items"
               " from the buffer, buffer only contains %zu items.\n", 
               n, buffer->used);
        exit(EXIT_FAILURE);
    }
    updateIndex(&buffer->read, n, buffer->size);
    buffer->used -= n;
}

// Copies 'n' samples from buffer 'src' to buffer 'dest'.
void copyBuffer(buffer_t *dest, buffer_t *src, size_t n) {
     if (n > src->used) {
        printf("Error in 'copyBuffer': trying to copy %zu items from the"
               " source buffer, buffer only contains %zu items.\n", n, 
               src->used);
        exit(EXIT_FAILURE);
    }

    if (n > dest->size) {
        printf("Error in 'copyBuffer': trying to copy %zu items to the" 
               " destination buffer, buffer can only contain %zu items.\n",
               n, dest->size);
        exit(EXIT_FAILURE);

    }

    for (size_t i = 0; i < n; i++) {
        insertIntoBuffer(dest, readFromBuffer(src, i));
    }

} 

// Copies the next 'n' samples from the buffer to an array, this is used 
// so the underlying algorithms don't need to know about the buffer.
// This does mean free() has to be called on the array.
sample_t *getArrayFromBuffer(buffer_t *buffer, size_t n) {
    sample_t *array = malloc(n * sizeof(sample_t));
    
    if (array == NULL) {
        printf("Error in 'getArrayFromBuffer': malloc failed to allocate"
               " %zu bytes of memory.\n", (n * sizeof(sample_t)));
        exit(EXIT_FAILURE);
    }
    
    for (size_t i = 0; i < n; i++) {
        *(array + i) = readFromBuffer(buffer, i);
    }

    return array;
}

// Can be used for debugging to see which parts of the buffer contain data.
void printStatusBuffer(buffer_t *buffer) {
    const size_t stepSize = buffer->size / 100;
   
    const size_t firstIndex = buffer->read;
    size_t lastIndex = buffer->read;
    updateIndex(&lastIndex, buffer->used, buffer->size);
    
    printf("["); 
    for (size_t i = 0; i < 100; i++) {
        size_t index = i * stepSize;
        if (firstIndex > lastIndex) {
            // Rollover has occured
            if (index <= lastIndex || index >= firstIndex) {
                printf("|"); 
            } else {
                printf("-"); 
            }
        } else {
            // No rollover has occured
            if (index >= firstIndex && index < lastIndex) {
                printf("|");
            } else {
                printf("-");
            }
        }
    }
    printf("]\n");
}

void freeBuffer(buffer_t *buffer) {
    free(buffer->data);
}
