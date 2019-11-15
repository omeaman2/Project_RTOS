#include<stdio.h>
#include<math.h>

double* getAutoCorrelationOfSeries(double da[], int size);
double GetCorrelaton(double x[], double y[]);
double GetAverage(double data[]);
double GetVariance(double data[]);
double GetStdev(double data[]);

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

    int size_result = sizeof(result) / sizeof(result[0]);
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
    printf("sizeof(da): %d -- size_da: %d\n", sizeof(da), size_da);
    double autoCorrelation[half];
    double a[half];
    double b[half];

    for (int i = 0; i < half; i++){
        a[i] = da[i];
        b[i] = da[i + i];
        autoCorrelation[i] = GetCorrelaton(a, b);
        //printf("%f - ", autoCorrelation[i]);
    }

    return autoCorrelation;
}

double GetCorrelaton(double x[], double y[]) {
    int size_x = sizeof(x) / sizeof(x[0]);
    int size_y = sizeof(y) / sizeof(y[0]);
    printf("%d\n%d\n", size_x, size_y);
    if(size_x != size_y){
        printf("Length of sources is different");
        return 0;
    }

    double avgX = GetAverage(x);
    double stdevX = GetStdev(x);
    double avgY = GetAverage(y);
    double stdevY = GetStdev(y);
    double covXY = 0;
    double pearson = 0;

    for(int i = 0; i < size_x; i++){
        covXY = (x[i] - avgX) * (y[i] - avgY);
    }

    covXY /= size_x;
    pearson = covXY / (stdevX * stdevY);

    return pearson;
}

double GetAverage(double data[]){
    size_t size_data = sizeof(data) / sizeof(data[0]);

    if(size_data == 0){
        return 0;
    }

    double sum = 0;

    for(int i = 0; i < size_data; i++){
        sum += data[i];
    }

    return sum / size_data;
}

double GetVariance(double data[]){
    size_t size_data = sizeof(data) / sizeof(data[0]);

    double avg = GetAverage(data);
    double sum = 0;

    for(int i = 0; i < size_data; i++){
        sum += pow((data[i] - avg), 2);
    }

    return sum / size_data;
}

double GetStdev(double data[]){
    return sqrt(GetVariance(data));
}
