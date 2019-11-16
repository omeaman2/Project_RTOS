
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double *getAutoCorrelationOfSeries(double da[], double *result, int size);
double GetCorrelaton(double x[], double y[], int size_a, int size_b);
double GetAverage(double data[], int size);
double GetVariance(double data[], int size);
double GetStdev(double data[], int size);

#define MAX_SIZE 10

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
    int half = (int) (size / 2);
    double x[half];
    double y[half];
    int size_x, size_y;

    for (int i = 0; i < half; i++){
        x[i] = da[i];
        y[i] = da[i + i];
        size_x = sizeof(x) / sizeof(x[0]);
        size_y = sizeof(y) / sizeof(y[0]);
        *(autoCorrelation + i) = GetCorrelaton(x, y, size_x, size_y);
    }

    return autoCorrelation;
}


int main() {
    double data_array[MAX_SIZE] = {1, 4, 6, 3, 5, 6, 1, 5, 3, 3};
    int size_data_array = sizeof(data_array) / sizeof(data_array[0]);
    double *result = (double *)malloc(MAX_SIZE * sizeof(*result));

    if (result == NULL) {
        printf("ERROR: Malloc failed\n\n");
        return 0;
    }

    printf("Computing Autocorrelation \n\n");

    result = getAutoCorrelationOfSeries(data_array, result, size_data_array);

    for (int i = 0; i < size_data_array / 2; ++i) {
        // printf("%d - %f\n", i, *(result + i));
        printf("%f\n", result[i]);
    }

    printf("\nDONE!\n");

    free(result);

    return 0;
}
