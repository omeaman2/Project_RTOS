#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wave.h"

#define N_SAMPLES 100

// Create a sine wave.
// params:
// sine: array of doubles to store the sine wave in.
// theta: the angle to start at.
// n_samples: number of samples.
// increment: number to increment theta with every iteration.
void make_sine(double* sine, double theta, int n_samples, double increment);
void print_sine(double* s, int n_samples);

// Write a sine wave as audio to a wav file.
// params:
// sine: the sine wave
// filename: a file with the wav extension.
// n_samples: number of samples
void write_sine_as_audio(double* sine, char* filename, int n_samples);

int main() {
    double sine[N_SAMPLES];
    make_sine(sine, 0.0, N_SAMPLES, M_PI/4);
    print_sine(sine, N_SAMPLES);

    return 0;
}

void make_sine(double* sine, double theta, int n_samples, double increment) {
    for (int i = 0; i < n_samples; ++i) {
        sine[i] = sin(theta);
        theta +=  increment;
    }
}

void print_sine(double* s, int n_samples) {
    for (int i = 0; i < n_samples; ++i) {
        printf("%f\n", s[i]);
    }
}

void write_sine_as_audio(double* sine, char* filename, int n_samples) {
    FILE *f = fopen(filename, "wb");
    if (f == NULL) {
        printf("Error opening file\n");
        exit(1);
    }

    // Prepare the wav file format data.
    unsigned char buffer4[4];
    unsigned char buffer2[2];
    struct wave_header header;

    // Read header parts.
    int written = 

    for (int i =  0; i < n_samples; ++i) {
    }

    fclose(f);
}
