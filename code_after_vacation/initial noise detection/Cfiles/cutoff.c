#include <stdio.h>
#include <stdlib.h>
#include "trainshortmicdata.c"
#include "math.h"

// Prototypes
int recognizeEnd(int start, unsigned long startMedium);

int arraySize = sizeof(dataarray) / sizeof(*dataarray);

int main() {
    unsigned long counter = 0;
    unsigned long average = 0;
    unsigned long prevAverage = 0;
    unsigned int used = 0;
    printf("arraySize: %d\n", arraySize);
    for(int k = 0; k < (arraySize - 3000); k+=3000) {
        counter = 0;
        used = 0;
        for(int i = k; i < (k + 3000); i++) {
            int data = abs(dataarray[i]);
            if( data > 500 ) {
                counter += data;
                used++;
            }
        }
        prevAverage = average;
        if(used != 0) {
            average = counter / used;
        } else {
            average = 0;
        }
        if(k != 0) {
            unsigned long safeZone = (prevAverage / 2);
            unsigned long tempAverage = prevAverage + safeZone;

            if(tempAverage < average && prevAverage > 500) {
                k = recognizeEnd(k, prevAverage - safeZone);
            }
        }
    }
}

int recognizeEnd(int start, unsigned long startMedium) {
    unsigned long average = 0;
    unsigned long counter = 0;
    unsigned long maxCount = start + 24000;
    unsigned long maxLoop = 0;
    if(maxCount < arraySize) {	
        maxLoop = maxCount;
    } else {
        maxLoop = arraySize - 250;
    }
    printf("start noise %d", start);
    for(int k = start; k < maxLoop; k++) {
        counter = 0;
        for(int i = k; i < (k + 250); i++) {
            counter += abs(dataarray[i]);
        }

        average = counter / 250;

        if(average <= startMedium) {
            printf(" end noise %d \n", k);
            return k;
        }
    }
    printf("end noise \n");
    return maxLoop;
}
