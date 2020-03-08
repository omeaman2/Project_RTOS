#ifndef RTES_H
#define RTES_H

#define USE_TEMPFREERTOS

#ifdef USE_TEMPFREERTOS
#include "tempFREERTOS.h"
#endif /* USE_TEMPFREERTOS */

#include "data.h"
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

typedef struct {
    const char *pcTaskName;
    TickType_t xTaskPeriod;
    void *settings;
} taskInfo_t;

typedef struct {
    sample_t *data;
    size_t read;
    size_t write;
    size_t used;
    size_t size;
} buffer_t;

buffer_t createBuffer(size_t capacity);
void insertIntoBuffer(buffer_t *buffer, sample_t data);
sample_t readFromBuffer(buffer_t *buffer, size_t offset);
void removeFromBuffer(buffer_t *buffer, size_t n);
void copyBuffer(buffer_t *dest, buffer_t *src, size_t n);
void printStatusBuffer(buffer_t *buffer);
void freeBuffer(buffer_t *buffer);

#endif /* RTES_H */
