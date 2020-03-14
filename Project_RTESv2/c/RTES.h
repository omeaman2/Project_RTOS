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
// Name of the task in FreeRTOS.
    const char *pcTaskName;
// Value used in FreeRTOS for the period of the task.
    TickType_t xTaskPeriod;
// The ratio between the period of the input task and the respective task.
    size_t ratio;
// Variable used for testing, can be removed after testing.
    void *test;
} baseSettings_t;

typedef struct {
    sample_t *data; // Pointer to data, created by createBuffer()
    const char *name; // Name of the buffer (used for testing)
    size_t read; // Index to read the first value from
    size_t write; // Index to write the next value to
    size_t used; // Amount of sample_t values currently stored
    size_t size; // Amount of sample_t values can be stored
} buffer_t; 

// The variable which defines in how many steps the printStatusBuffer()
// function will print the sections of the buffer which contain data
static const size_t resolutionPrintStatus = 100;

buffer_t createBuffer(const char *name, size_t capacity);
void insertIntoBuffer(buffer_t *buffer, sample_t data);
sample_t readFromBuffer(buffer_t *buffer, size_t offset);
void removeFromBuffer(buffer_t *buffer, size_t n);
void copyBuffer(buffer_t *dest, buffer_t *src, size_t n);
void copyArrayFromBuffer(sample_t dest[], buffer_t *src, size_t n);
void copyBufferFromArray(buffer_t *dest, sample_t src[], size_t n);
void printStatusBuffer(buffer_t *buffer);
void freeBuffer(buffer_t *buffer);

#endif /* RTES_H */
