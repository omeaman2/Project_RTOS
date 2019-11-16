#include<stdio.h>
#include<math.h>

double* getAutoCorrelationOfSeries(double da[], int size);
double GetCorrelaton(double x[], double y[], int size_a, int size_b);
double GetAverage(double data[], int size);
double GetVariance(double data[], int size);
double GetStdev(double data[], int size);

int main(){
    double data_array[10] = {1, 4, 6, 3, 5, 6, 1, 5, 3, 3};
    double* result;
    int size_data_array = sizeof(data_array) / sizeof(data_array[0]);

    printf("Computing Autocorrelation \n\n");

    result = getAutoCorrelationOfSeries(data_array, size_data_array);

    // int size_array = sizeof(data_array) / sizeof(data_array[0]);
    // for (int i = 0; i < size_array; i++){
    //     printf("%d - %f\n", i, *(data_array + i));
    // }

    // int size_result = sizeof(result) / sizeof(result[0]);
    // printf("%d\n", size_result);
    // for (int i = 0; i < size_result; i++){
    //     printf("%d - %f\n", i, *(result + i));
    // }

    printf("\nDONE!\n");

    return 0;
}

double* getAutoCorrelationOfSeries(double da[], int size){
    int half = (int) (size / 2);
    int size_da = sizeof(da) / sizeof(da[0]);
    double autoCorrelation[half];
    double x[half];
    double y[half];
    int size_x, size_y;

    for (int i = 0; i < half; i++){
        x[i] = da[i];
        y[i] = da[i + i];
        size_x = sizeof(x) / sizeof(x[0]);
        size_y = sizeof(y) / sizeof(y[0]);
        autoCorrelation[i] = GetCorrelaton(x, y, size_x, size_y);
        //printf("%f - ", autoCorrelation[i]);
    }

    int size_result = sizeof(autoCorrelation) / sizeof(autoCorrelation[0]);
    printf("%d\n", size_result);
    for (int i = 0; i < size_result; i++){
        printf("%d - %f\n", i, *(autoCorrelation + i));
    }

    return autoCorrelation;
}

double GetCorrelaton(double x[], double y[], int size_x, int size_y) {
    if(size_x != size_y){
        printf("Length of sources is different");
        return 0;
    }

    double avgX = GetAverage(x, size_x);
    double stdevX = GetStdev(x, size_x);
    double avgY = GetAverage(y, size_y);
    double stdevY = GetStdev(y, size_y);
    double covXY = 0;
    double pearson = 0;

    for(int i = 0; i < size_x; i++){
        covXY = (x[i] - avgX) * (y[i] - avgY);
    }

    covXY /= size_x;
    pearson = covXY / (stdevX * stdevY);

    return pearson;
}

double GetAverage(double data[], int size){
    if(size == 0){
        return 0;
    }

    double sum = 0;

    for(int i = 0; i < size; i++){
        sum += data[i];
    }

    return sum / size;
}

double GetVariance(double data[], int size){
    double avg = GetAverage(data, size);
    double sum = 0;

    for(int i = 0; i < size; i++){
        sum += pow((data[i] - avg), 2);
    }

    return sum / size;
}

double GetStdev(double data[], int size){
    return sqrt(GetVariance(data, size));
}
