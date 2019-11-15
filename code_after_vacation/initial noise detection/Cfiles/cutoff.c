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
    
    //Loop compolete array, safezone of 400 because next 400 elements are looped before check is reached
    for(int k = 0; k < (arraySize - 400); k+=400) {
        counter = 0;
        used = 0;
	//Loop next 400 ellements calculate average
        for(int i = k; i < (k + 400); i++) {
            int data = abs(dataarray[i]);
            //If calue of data < 500 it can't be heard and is not usable
	    if( data > 500 ) {
                counter += data;
                used++;
            }
        }
	//Save avarage of last loop
        prevAverage = average;
	//If used == 0 don't divide by 0, otherwise divide the counter by the amount of time the counter is edited.
        if(used != 0) {
            average = counter / used;
        } else {
            average = 0;
        }
        if(k != 0) {
		//Check if an huge increase in volume occured.
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
    //max noise length is the start moment plus 0.5 second.
    unsigned long maxCount = start + 3000;
    unsigned long maxLoop = 0;
    //If the max noise length exeeds the arraysize, use arraysize - safezone as max noise length. Safezone is used because next 30 elements will be looped.
    if(maxCount < arraySize) {
        maxLoop = maxCount;
    } else {
        maxLoop = arraySize - 30;
    }
    printf("start noise %d", start);
    //Loop from start of noise to max 0.5 second further.
    for(int k = start; k < maxLoop; k++) {
        counter = 0;
	//Check if noise drasticly decreases in next 0.05 seconds 
        for(int i = k; i < (k + 30); i++) {
            counter += abs(dataarray[i]);
        }

        average = counter / 30;
	//If average is lower than the value at the start of the noise, noise ended so function is stopped.
        if(average <= startMedium) {
            printf(" end noise %d \n", k);
            return k;
        }
    }
    //No end of noise is detected, detected noise get discarded.
    printf("end noise \n");
    return maxLoop;
}
