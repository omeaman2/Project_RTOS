#ifndef RTES_H
#define RTES_H

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#define USE_TEMPFREERTOS       (1)
#include "tempFREERTOS.h"

/* This struct represents the data taken during one sample */
typedef struct {
    double data;
} RTES_Sample_t;

/* This struct represents the circular buffer used to store samples */
#define RTES_BUFFER_SIZE        (2000)
typedef struct {
    RTES_Sample_t *samples;
    size_t index_first; // Index to read first sample from
    size_t index_next; // Index to place next sample
    size_t size; // Amount of samples which can be stored in this buffer
} RTES_Buffer_t;

/* This struct specifies all parameters passed to the task */
typedef struct {
    const char *pcTaskName;
    RTES_Buffer_t *in_buffer;
    RTES_Buffer_t *out_buffer;
    TickType_t *xTaskPeriod;
} RTES_TaskParameters;

void initBuffer(RTES_Buffer_t *buffer, size_t size);
void insertIntoBuffer(RTES_Buffer_t *buffer, RTES_Sample_t sample);
RTES_Sample_t readFromBuffer(RTES_Buffer_t *buffer, size_t offset);
void removeFromBuffer(RTES_Buffer_t *buffer);
void freeBuffer(RTES_Buffer_t *buffer);

#endif /* RTES_H */
