#ifndef SETTINGS_H
#define SETTINGS_H

#include "RTES.h"
#include "Input/input.h"
#include "Output/output.h"

inputSettings_t inputSettings;
outputSettings_t outputSettings;
recognizeSettings_t recognizeSettings;

void createSettings(buffer_t *inputToRecognizeBuffer,
                    buffer_t *recognizeToCancelBuffer,
                    buffer_t *cancelToOutputBuffer, FILE *fpOutput) {
    inputSettings = {
        .base = {
            .pcTaskname = "Input Task",
            .xTaskPeriod = 0 //TODO 
        },
        .outBuffer = inputToRecognizeBuffer
    };
    
    outputSettings = {
        .base = {
            .pcTaskName = "Output Task",        
            .xTaskPeriod = 0 //TODO
        },
        .inBuffer = cancelToOutputBuffer,
        .fpOutput = fpOutput
    };

    recognizeSettings = {
        .base = {
            .pcTaskName = "Recognize Task",
            .xTaskPeriod = 0 //TODO
        },
        .inBuffer = inputToRecognizeBuffer,
        .outBuffer = recognizeToCancelBuffer,
        .samplesInSegment = 0, //TODO
        .lowerLimitRecognizeBegin = 0, //TODO
        .lowerLimitRecognizeEnd = 0 //TODO
    };
}

#endif /* SETTINGS_H */
