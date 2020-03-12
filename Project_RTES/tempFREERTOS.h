#if     USE_TEMPFREERTOS == 1
#ifndef TEMPFREERTOS_H
#define TEMPFREERTOS_H

typedef uint64_t TickType_t;
static inline TickType_t xTaskGetTickCount(void) { return (TickType_t)0; };
static inline void vTaskDelayUntil(TickType_t *pxPreviousWakeTime, TickType_t xTimeIncrement) { return; };

#endif /* TEMPFREERTOS_H */
#endif /* USE_TEMPFREERTOS */
