#include "RTES.h"

// Increment the index by 'n', rollover if index+n exceeds max.
void updateIndex(size_t *index, size_t n, size_t max);

buffer_t createBuffer(const char *name, size_t size) {
    sample_t *array = malloc(size * sizeof(sample_t));
    
    if (array == NULL) {
        printf("Error in 'createBuffer' (%s): malloc failed to allocate" 
               " %zu bytes of memory.\n", name, (size * sizeof(sample_t)));
        exit(EXIT_FAILURE);
    }

    if (size % resolutionPrintStatus != 0) {
        printf("Warning in 'createBuffer' (%s): to ensure"
               " printStatusBuffer works correctly a multiple of %zu" 
               " should be used for the size.\n", name, 
               resolutionPrintStatus);
    }
    
    buffer_t buffer = {
        .data = (sample_t *)array,
        .name = name,
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
        printf("Error in 'insertIntoBuffer' (%s): trying to insert into a"
               " full buffer.\n", buffer->name);
        exit(EXIT_FAILURE);
    }

    buffer->data[buffer->write] = data;
    buffer->used++;
    updateIndex(&buffer->write, 1, buffer->size);
}

// Read a sample from a index relative to buffer->read.
sample_t readFromBuffer(buffer_t *buffer, size_t offset) {
    if (buffer->used == 0) {
        printf("Error in 'readFromBuffer' (%s): trying to read from an" 
               " empty buffer.\n", buffer->name);
        exit(EXIT_FAILURE);
    } 

    if (offset > buffer->used - 1) {
        printf("Error in 'readFromBuffer' (%s): trying to read from an"
               " invalid index.\n", buffer->name);
        exit(EXIT_FAILURE);
    }

    size_t index = buffer->read;
    updateIndex(&index, offset, buffer->size);
    return buffer->data[index];
}

// Remove 'n' samples from the buffer by moving the read index.
void removeFromBuffer(buffer_t *buffer, size_t n) {
    if (n > buffer->used) {
        printf("Error in 'removeFromBuffer' (%s): trying to remove %zu" 
               " items from the buffer, buffer only contains %zu items.\n",
               buffer->name, n, buffer->used);
        exit(EXIT_FAILURE);
    }

    updateIndex(&buffer->read, n, buffer->size);
    buffer->used -= n;
}

// Copies 'n' samples from buffer 'src' to buffer 'dest'.
void copyBuffer(buffer_t *dest, buffer_t *src, size_t n) {
     if (n > src->used) {
        printf("Error in 'copyBuffer' (%s to %s): trying to copy %zu items"
               " from the source buffer, buffer contains %zu items.\n", 
               src->name, dest->name, n, src->used);
        exit(EXIT_FAILURE);
    }

    if (n > dest->size || n > dest->size - dest->used) {
        printf("Error in 'copyBuffer' (%s to %s): trying to copy %zu items"
               " to the destination buffer, buffer has room for an" 
               " additional %zu items (currently storing %zu items).\n", 
               src->name, dest->name, n, (dest->size - dest->used), 
               dest->used);
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < n; i++) {
        insertIntoBuffer(dest, readFromBuffer(src, i));
    }

} 

// Uses malloc() to get a new empty array, handles possible errors.
// The array has to be free()'ed after use.
sample_t *getNewEmptyArray(size_t size) {
    sample_t *array = malloc(size * sizeof(sample_t));

    if (array == NULL) {
        printf("Error in 'getNewEmptyArray': malloc failed to allocate %zu"
               " bytes of memory.\n", (size * sizeof(sample_t)));
        exit(EXIT_FAILURE);
    }
    return array;
}

// Copies the next 'n' samples from the buffer to a new array, this is used
// because the Windows port of FreeRTOS does not support Variable Length
// Arrays. (otherwise copyArrayFromBuffer would be the preferred function) 
// Because this uses malloc() a free() will need to be performed after 
// using the array.
sample_t *copyNewArrayFromBuffer(buffer_t *src, size_t n, size_t offset) {
   sample_t *array = malloc(n * sizeof(sample_t));

    if (array == NULL) {
        printf("Error in 'copyNewArrayFromBuffer' (%s): malloc failed to" 
               " allocate %zu bytes of memory.\n", src->name, 
               (n * sizeof(sample_t)));
        exit(EXIT_FAILURE);
    }

    if (n > src->used) {
        printf("Error in 'copyNewArrayFromBuffer' (%s): trying to copy %zu"
               " items from the source buffer, buffer only contains %zu" 
               " items.\n", src->name, n, src->used);
        exit(EXIT_FAILURE);
    }

    if (n + offset > src->used) {
        printf("Error in 'copyNewArrayFromBuffer' (%s): trying to copy the"
               " %zuth to %zuth item from the buffer, buffer only contains"
               " %zu items.\n", src->name, offset, offset+n, src->used);
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < n; i++) {
        array[i] = readFromBuffer(src, offset + i);
    }
    return array; 
}

// Copies the next 'n' samples from the buffer to an existing array.
void copyArrayFromBuffer(sample_t dest[], buffer_t *src, size_t n, 
                                                         size_t offset) {
     if (n > src->used) {
        printf("Error in 'copyArrayFromBuffer' (%s): trying to copy %zu"
               " items from the source buffer, buffer only contains %zu" 
               " items.\n", src->name, n, src->used);
        exit(EXIT_FAILURE);
    }

    if (n + offset > src->used) {
        printf("Error in 'copyArrayFromBuffer' (%s): trying to copy the"
               " %zuth to %zuth item from the buffer, buffer only contains"
               " %zu items.\n", src->name, offset, offset+n, src->used);
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < n; i++) {
        dest[i] = readFromBuffer(src, offset + i);
    }
}

// Copies the next 'n' samples from the array to the buffer
void copyBufferFromArray(buffer_t *dest, sample_t src[], size_t n) {
    if (n > dest->size - dest->used) {
        printf("Error in 'copyBufferFromArray' (%s): destination buffer is"
               " unable to store an extra %zu items.\n", dest->name, n);
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < n; i++) {
        insertIntoBuffer(dest, src[i]);
    }
}

// Prints which sections of the buffer contain data, checks stepSize
// indexes at a time. Prints "|" if atleast one of the indexes contain
// data, otherwise prints "-". Used for testing only.
void printStatusBuffer(buffer_t *buffer) {
    const size_t stepSize = buffer->size / resolutionPrintStatus;
   
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
