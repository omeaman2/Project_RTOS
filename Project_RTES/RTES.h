#ifndef RTES_H
#define RTES_H

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#define USE_TEMPFREERTOS       (1)
#include "tempFREERTOS.h"

/* This struct represents the data taken during one sample */
typedef struct {
    double data;
} RTES_Sample_t;

/* This struct represents the circular buffer used to store samples */
#define RTES_BUFFER_SIZE        (2000)
typedef struct {
    RTES_Sample_t * samples;
    size_t index_first; // Read first sample from samples[index_first]
    size_t index_next; // Place next sample into samples[index_next]
    size_t used; // Amount of samples stored in samples[]
    const size_t size; // Maximum amount of samples in samples[]
} RTES_Buffer_t;

/* This struct specifies all parameters passed to the task */
typedef struct {
    const char *pcTaskName;
    RTES_Buffer_t *in_buffer;
    RTES_Buffer_t *out_buffer;
    TickType_t *xTaskPeriod;
} RTES_TaskParameters;

RTES_Buffer_t createBuffer(size_t size);
size_t incrementIndexWithRollover(size_t base, size_t size, size_t n);
void insertIntoBuffer(RTES_Buffer_t *buffer, RTES_Sample_t sample);
RTES_Sample_t readFromBuffer(RTES_Buffer_t *buffer, size_t offset);
void removeFromBuffer(RTES_Buffer_t *buffer);
void removeItemsFromBuffer(RTES_Buffer_t *buffer, size_t amount);
void copyBuffer(RTES_Buffer_t *dest, const RTES_Buffer_t *src, size_t n);
void freeBuffer(RTES_Buffer_t *buffer);

#endif /* RTES_H */
