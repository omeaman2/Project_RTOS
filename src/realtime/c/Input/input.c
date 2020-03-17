#include "input.h"

sample_t takeSample(void);

void vTaskInput(void *pvParameters) {
    inputSettings_t *settings = (inputSettings_t*) pvParameters;

    TickType_t xTimeTaskStarted;
    for (;;) {
        xTimeTaskStarted = xTaskGetTickCount();

        doInput(settings);

        vTaskDelayUntil(&xTimeTaskStarted, settings->base.xTaskPeriod);
    }
}

void doInput(inputSettings_t *settings) {
    insertIntoBuffer(settings->outBuffer, takeSample());
}

sample_t takeSample(void) {
    static size_t index = 0;

	if (index % 1000 == 0) printf("%zu\n", index);
	if (index % numberOfSamples == 0) index = 0;

    return readData(index++);
}
