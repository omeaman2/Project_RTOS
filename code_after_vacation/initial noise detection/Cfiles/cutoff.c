// ********************** DEPRECATED **********************
// continue work on these functions in using_the_kissfft_library/main.c
#include <stdio.h>
#include <stdlib.h>
#include "trainshortmicdata.h"
#include "segment1.c"
#include "segment2.c"
#include "segment3.c"
#include "segment4.c"
#include "math.h"

// Prototypes
int recognizeEnd(int start, unsigned long startMedium);
double *getAutoCorrelationOfSeries(double da[], double *result, int size);
double GetCorrelaton(double x[], double y[], int size_a, int size_b);
double GetAverage(double data[], int size);
double GetVariance(double data[], int size);
double GetStdev(double data[], int size);

/* Define the size of the global mic data array*/
int arraySize = sizeof(data_array) / sizeof(*data_array);

double GetVariance(double data[], int size) {
    double avg = GetAverage(data, size);
    double sum = 0;

    for (int i = 0; i < size; i++) {
        sum += pow((data[i] - avg), 2);
    }

    return sum / size;
}

double GetAverage(double data[], int size) {
    if (size == 0) {
        return 0;
    }

    double sum = 0;

    for (int i = 0; i < size; i++) {
        sum += data[i];
    }

    return sum / size;
}

double GetStdev(double data[], int size) {
    return sqrt(GetVariance(data, size));
}

double GetCorrelaton(double x[], double y[], int size_x, int size_y) {
    if (size_x != size_y) {
        printf("Length of sources is different");
        return 0;
    }

    double avgX = GetAverage(x, size_x);
    double stdevX = GetStdev(x, size_x);
    double avgY = GetAverage(y, size_y);
    double stdevY = GetStdev(y, size_y);
    double covXY = 0;
    double pearson = 0;

    for (int i = 0; i < size_x; i++) {
        covXY = (x[i] - avgX) * (y[i] - avgY);
    }

    covXY /= size_x;
    pearson = covXY / (stdevX * stdevY);

    return pearson;
}

double *getAutoCorrelationOfSeries(double da[], double *autoCorrelation, int size) {
    int half = (int)(size / 2);
    double x[half];
    double y[half];
    int size_x, size_y;

    for (int i = 0; i < half; i++) {
        x[i] = da[i];
        y[i] = da[i + i];
        size_x = sizeof(x) / sizeof(x[0]);
        size_y = sizeof(y) / sizeof(y[0]);
        *(autoCorrelation + i) = GetCorrelaton(x, y, size_x, size_y);
    }

    return autoCorrelation;
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
            counter += abs(data_array[i]);
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

int main() {
    unsigned long counter = 0;
    unsigned long average = 0;
    unsigned long prevAverage = 0;
    unsigned int used = 0;

    int *start_noises = (int *)malloc(MAX_NOISES * (sizeof(*start_noises)));
    int *end_noises = (int *)malloc(MAX_NOISES * (sizeof(*end_noises)));
    double *autoCorrelation = (double *)malloc(arraySize * sizeof(*autoCorrelation));
    double *seg_1 = (double *)malloc(arraySize * sizeof(*seg_1));
    double *seg_2 = (double *)malloc(arraySize * sizeof(*seg_2));
    double *seg_3 = (double *)malloc(arraySize * sizeof(*seg_3));
    double *seg_4 = (double *)malloc(arraySize * sizeof(*seg_4));

    if (start_noises == NULL || end_noises == NULL || autoCorrelation == NULL) {
        printf("ERROR: Malloc failed\n\n");
        return EXIT_FAILURE;
    }

    // printf("data_array[683] = %f\n", data_array[683]);
    //Loop compolete array, safezone of 400 because next 400 elements are looped before check is reached
    int numberOfNoiseSegments = 0;
    for (int k = 0; k < (arraySize - 400); k += 400) {
        counter = 0;
        used = 0;
        //Loop next 400 ellements calculate average
        for (int i = k; i < (k + 400); i++) {
            int data = abs(data_array[i]);
            //If calue of data < 500 it can't be heard and is not usable
            if (data > 500) {
                counter += data;
                used++;
            }
        }
        //Save avarage of last loop
        prevAverage = average;
        //If used == 0 don't divide by 0, otherwise divide the counter by the amount of time the counter is edited.
        if (used != 0) {
            average = counter / used;
        }
        else {
            average = 0;
        }
        if (k != 0) {
            //Check if an huge increase in volume occured.
            unsigned long safeZone = (prevAverage / 2);
            unsigned long tempAverage = prevAverage + safeZone;
            if (tempAverage < average && prevAverage > 500) {
                *(start_noises + numberOfNoiseSegments) = k;
                k = recognizeEnd(k, prevAverage - safeZone);
                *(end_noises + numberOfNoiseSegments) = k;
                numberOfNoiseSegments++;
            }
        }
    }

    // for (int i = 0; i < y; ++i){
    //     printf("*(start_noises + i): %d\n", *(start_noises + i));
    //     printf("*(end_noises + i): %d\n", *(end_noises + i));
    // }

    printf("Computing Autocorrelation \n\n");

    int sizeOfNoiseArray = 0;

    printf("numberOfNoiseSegments: %d\n", numberOfNoiseSegments);
    for (int i = 0; i < numberOfNoiseSegments; ++i) {
        printf("SEGMENT %d\n", i);

        sizeOfNoiseArray = *(end_noises + i) - *(start_noises + i);
        double noiseArray[sizeOfNoiseArray];

        printf("sizeOfNoiseArray: %d\n", sizeOfNoiseArray);

        for (int y = 0; y < sizeOfNoiseArray; ++y) {
	    /* printf("sizeOfNoiseArray: %d\n", sizeOfNoiseArray); */
            noiseArray[y] = data_array[*(start_noises + i) + y];
	    if (i == 0){
		*(seg_1 + y) = noiseArray[y];
		/* printf("%f\n", *(seg_1 + y)); */
	    }
	    if (i == 1){
		*(seg_2 + y) = noiseArray[y];
		/* printf("%f\n", *(seg_2 + y)); */
	    } 
	    if (i == 2){
		*(seg_3 + y) = noiseArray[y];
		/* printf("%f\n", *(seg_3 + y)); */
	    }
	    if (i == 3){
		*(seg_4 + y) = noiseArray[y];
		/* printf("%f\n", *(seg_4 + y)); */
	    }

        }

        autoCorrelation = getAutoCorrelationOfSeries(noiseArray, autoCorrelation, sizeOfNoiseArray);

        for (int z = 0; z < sizeOfNoiseArray / 2; ++z) {
    //        printf("%f -- %d -- %f\n", *(autoCorrelation + z), sizeOfNoiseArray + z, data_array[sizeOfNoiseArray + z]);
        }

        printf("\n\n");
    }

    
    printf("\nDONE!");

    free(autoCorrelation);
    free(start_noises);
    free(end_noises);
    free(seg_1);
    free(seg_2);
    free(seg_3);
    free(seg_4);

    return 0;
}
