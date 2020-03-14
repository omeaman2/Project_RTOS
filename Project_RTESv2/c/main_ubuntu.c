#include "Input/input.h"
#include "Output/output.h"
#include "Recognize/recognize.h"
#include "Cancel/cancel.h"

#include "RTES.h"
#include "settings.h"

int main(void) {
    buffer_t inputToRecognizeBuffer = createBuffer("inputToRecognize",
                                                   numberOfSamples);
    buffer_t recognizeToCancelBuffer = createBuffer("recognizeToCancel",
                                                    numberOfSamples);
    buffer_t cancelToOutputBuffer = createBuffer("cancelToOutput",
                                                 numberOfSamples);
    
    FILE *fpOutput = fopen("../csv/output.csv", "w");

    createSettings(sampleRate, &inputToRecognizeBuffer,
                               &recognizeToCancelBuffer,
                               &cancelToOutputBuffer, fpOutput);

    for (size_t i = 1; i <= numberOfSamples; i++) {
        if (i % inputSettings.base.ratio == 0) 
            doInput(&inputSettings);
        if (i % outputSettings.base.ratio == 0) 
            doOutput(&outputSettings);
        if (i % recognizeSettings.base.ratio == 0) 
            doRecognize(&recognizeSettings);
        if (i % cancelSettings.base.ratio == 0) 
            doCancel(&cancelSettings);
        printf("%zu\n", i);
    }

    fclose(fpOutput);

    return 0;   
}
