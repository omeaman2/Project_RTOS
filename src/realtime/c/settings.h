#ifndef SETTINGS_H
#define SETTINGS_H

#include "RTES.h"
#include "Input/input.h"
#include "Output/output.h"

inputSettings_t inputSettings;
outputSettings_t outputSettings;
cancelSettings_t cancelSettings;
recognizeSettings_t recognizeSettings;

void createSettings(uint32_t sampleRate,
                    buffer_t *inputToRecognizeBuffer,
                    buffer_t *recognizeToCancelBuffer,
                    buffer_t *cancelToOutputBuffer, FILE *fpOutput) {
    // Wav-file has a sample rate of 44,1 kHz
    // 44100 = 2^2 * 3^2 * 5^2 * 7^2
    // (note: 2, 3, 5 and 7 are the first 4 prime numbers)
    // We want to run the recognize task every 20ms (50/s)
    // 50 = 2 * 5^2
    // therefore the ratio between Recognize and Input is the remaining
    // prime factors: 2 * 3^2 * 7^2 = 882
    // So the ratio between the Recognize and Input Task is 882.
    // This means the recognize tasks has to process 882 samples per period

    inputSettings.base.pcTaskName = "Input Task";
	inputSettings.base.xTaskPeriod = pdMS_TO_TICKS(1);
    // Ratio is compared to the input task, therefore this is 1
    inputSettings.base.ratio = 1;
    inputSettings.base.test = NULL;
    inputSettings.outBuffer = inputToRecognizeBuffer;
    
    outputSettings.base.pcTaskName = "Output Task";        
    outputSettings.base.xTaskPeriod = pdMS_TO_TICKS(1);
    // Output happens at the same samplerate as the input
    outputSettings.base.ratio = 1;
    outputSettings.base.test = NULL;
    outputSettings.inBuffer = cancelToOutputBuffer;
    outputSettings.fpOutput = fpOutput;

    cancelSettings.base.pcTaskName = "Cancel Task";
    cancelSettings.base.xTaskPeriod = pdMS_TO_TICKS(1);
    cancelSettings.base.ratio = 1;
    cancelSettings.base.test = NULL; 
    cancelSettings.inBuffer = recognizeToCancelBuffer;
    cancelSettings.outBuffer = cancelToOutputBuffer;
    cancelSettings.cancelPercentage = 90;

    recognizeSettings.base.pcTaskName = "Recognize Task";
    recognizeSettings.base.xTaskPeriod = pdMS_TO_TICKS(882); // Same as ratio
    recognizeSettings.base.ratio = 882; // Same as segmentSize
    recognizeSettings.base.test = NULL;
    recognizeSettings.inBuffer = inputToRecognizeBuffer;
    recognizeSettings.outBuffer = recognizeToCancelBuffer;
    recognizeSettings.segmentSize = 882; // Same as ratio
    recognizeSettings.maxSamplesNoise = (size_t) sampleRate;
    recognizeSettings.lowerLimitBegin = 500;
    recognizeSettings.lowerLimitEnd = 0;
    recognizeSettings.factorIncreaseBegin = 1.5F;
    recognizeSettings.factorDecreaseEnd = 0.75F;
}

#endif /* SETTINGS_H */
