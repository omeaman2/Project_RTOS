#ifndef TEMPFREERTOS_H
#define TEMPFREERTOS_H

#include <stdint.h>

typedef uint64_t TickType_t;
static inline TickType_t xTaskGetTickCount(void) { return (TickType_t)0; };
static inline void vTaskDelayUntil(TickType_t *pxPreviousWakeTime, 
                                   TickType_t xTimeIncrement) { return; };

/* The defines below are directly copied from the windows port: */
#define configMAX_PRIORITIES	        ( 7 )
#define configTIMER_TASK_PRIORITY       ( configMAX_PRIORITIES - 1 )

/* In this non-real time simulated environment the tick frequency has to 
    be at least a multiple of the Win32 tick frequency, and therefore very 
    slow. */
#define configTICK_RATE_HZ      ( 1000 ) 

#ifndef pdMS_TO_TICKS
#define pdMS_TO_TICKS( xTimeInMs ) ( ( TickType_t ) ( ( ( TickType_t ) \
        ( xTimeInMs ) * ( TickType_t ) configTICK_RATE_HZ ) / \
        ( TickType_t ) 1000 ) )
#endif

#endif /* TEMPFREERTOS_H */
