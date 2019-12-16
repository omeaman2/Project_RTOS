#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "kissfft/kiss_fft.h"
#include "trainshortmicdata.h"
#include "segment1.c"
#include "segment2.c"
#include "segment3.c"
#include "segment4.c"

#define TRUE 1
#define FALSE 0

/*** Prototypes ***/

/* Ideally these 'do' functions should become tasks in FreeRTOS. */

// Perform fourier and inverse fourier functionality. 
// Params: all start and end indices of the noise.
int do_fourier(int* startNoise, int* endNoise);
// Internally called by do_fourier. Computes fourier for a single noise segment.
int do_fourier_internal(int startNoise, int endNoise);

// Test fourier and inverse fourier functionality. 
int do_fourier_for_testing_purposes(void);
// Perform recognize functionality. Previously cutoff.c
int do_recognize(void);

// Use epsilon to compare floating point numbers.
// epsilon specifies how small of a difference can be ignored. i.e. if
// i = 0.0001
// j = 0.0002
// epsilon = 0.001
// then i and j equal each other.
void epsilon_compare_signals(const kiss_fft_cpx *s1, const kiss_fft_cpx *s2,
        int n);

// The comparison
int epsilon_compare(double d1, double d2, double epsilon);

// Print a complex numbered signal.
void print_signal(const kiss_fft_cpx *s, const int n);

// Do inverse Fourier transform and restore the original signal from a fourier
// transformed signal.
void ifft_and_restore(const kiss_fft_cfg* state, const kiss_fft_cpx *in,
        kiss_fft_cpx *out, const int n);

// Create a signal and put it in the argument kissfft complex array.
void create_signal(kiss_fft_cpx* cx_in);

int recognizeEnd(int start, unsigned long startMedium);
double *getAutoCorrelationOfSeries(double da[], double *result, int size);
double getCorrelation(double x[], double y[], int size_a, int size_b);
double GetAverage(double data[], int size);
double GetVariance(double data[], int size);
double GetStdev(double data[], int size);

// Get the signal and put it in the argument kissfft complex array.
// cx_in must have enough space to hold a segment.
int get_noise_segment(kiss_fft_cpx* cx_in, const int start_noise, const int
        end_noise);

// Invert the frequencies of a complex numbered fourier transformed signal.
int invert_frequencies(kiss_fft_cpx* cx_in, const int num_segment_samples);

// Print start and end indices of noises.
void print_noise_indices();

// Print a noise segment
void print_segment(const kiss_fft_cpx* s, const int n);

#if USE_MALLOC
void print_segments(kiss_fft_cpx** segments, const int num_segments,
        const int* sizes);
#else
void print_segments(const kiss_fft_cpx segments[MAX_NSEGMENTS][MAX_NSAMPLES],
        const int num_segments, const int* sizes);
#endif

// Allocates a 2-dimensional array on the heap.
void** malloc2d(const int nrows, const int ncols , const size_t size);

/*** Global variables ***/

// Arrays containing the start index and the end index of the noises.
int startNoise[MAX_NSEGMENTS];
int endNoise[MAX_NSEGMENTS];
// Actual number of noise segments.
int num_noise_segments = 0;


// The program
// This should become a infinite loop.
int main(void) {
    for (int i = 0; ; ++i) {
        int r;
        r = do_recognize();
        if (r != EXIT_SUCCESS) return EXIT_FAILURE;
        print_noise_indices();
        /* r = do_fourier_for_testing_purposes(); */
        /* if (r != EXIT_SUCCESS) return EXIT_FAILURE; */
        r = do_fourier(startNoise, endNoise);
        if (r != EXIT_SUCCESS) return EXIT_FAILURE;
        return r;
    }
}


/*** Function definitions ***/

int do_fourier(int* startNoise, int* endNoise) {
    int r;
    if (num_noise_segments <= 0) {
        fprintf(stderr, "do_fourier: num_noise_segments is less than"
                "or equal to zero.\n");
        return EXIT_FAILURE;
    }

    // kissfft states.
    kiss_fft_cfg kfft_fourier_state;
    kiss_fft_cfg kfft_inverse_fourier_state;

    // These will contain the cancelling noise segments that will be output to
    // the speaker.
    // Simply speaking this is an array of signals (a signal is an array too,
    // thus we have got a 2d array).
#if USE_MALLOC
        kiss_fft_cpx** cx_cancelling_segments = (kiss_fft_cpx**)
            malloc2d(num_noise_segments, MAX_NSAMPLES, sizeof(kiss_fft_cpx));
#else
        kiss_fft_cpx cx_cancelling_segments[num_noise_segments][MAX_NSAMPLES];
#endif
    // Array of the number of samples of each noise segment.
    int segment_sizes[num_noise_segments];

    // Initialise the kissfft state buffers.
    kfft_fourier_state = kiss_fft_alloc(NSAMPLES, FOURIER, 0, 0);
    kfft_inverse_fourier_state = kiss_fft_alloc(NSAMPLES,
            INVERSE_FOURIER, 0, 0);

    // For all noise segments:
    //
    // 1. Get a noise segment.
    // 2. Compute Fourier to get the noise frequencies.
    // 3. Invert the frequencies.
    // 4. Compute inverse fourier to generate cancelling noise.
    for (int i = 0; i < num_noise_segments; ++i) {
        /* if (i == 1 || i == 3) { */
            /* continue; */
        /* } */
        // Compute the length of the segment.
        segment_sizes[i] = endNoise[i] - startNoise[i];
        if (segment_sizes[i] > MAX_NSAMPLES) {
            fprintf(stderr, "do_fourier: I cannot fit it in it is too big!\n");
            fprintf(stderr, "Continuing\n");
            /* goto fail; */
            continue;
        }

        // 1. Get a noise segment.
        kiss_fft_cpx cx_noise_segment[segment_sizes[i]];
        r = get_noise_segment(cx_noise_segment, startNoise[i], endNoise[i]);
        if (r != EXIT_SUCCESS) {
            fprintf(stderr, "do_fourier: Could not get noise segment.\n");
            goto fail;
        }


        // 2. Compute Fourier to get the noise frequencies.
        kiss_fft_cpx cx_noise_segment_fourier[segment_sizes[i]];
        printf("segment_size[%d]: %d\n", i, segment_sizes[i]);
        kiss_fft(kfft_fourier_state, cx_noise_segment, cx_noise_segment_fourier);
        
        printf("Awaiting segmentation faults! :D\n");
        // 3. Invert the frequencies.
        invert_frequencies(cx_noise_segment_fourier, segment_sizes[i]);
        if (r != EXIT_SUCCESS) {
            fprintf(stderr, "do_fourier: Could not invert frequencies\n");
            goto fail;
        }

        // 4. Compute inverse fourier to generate cancelling noise.
        ifft_and_restore(&kfft_inverse_fourier_state, cx_noise_segment_fourier,
                cx_cancelling_segments[i], segment_sizes[i]);
    }

    print_segments(cx_cancelling_segments, num_noise_segments, segment_sizes);
    
    // Free configs.
    free(kfft_fourier_state);
    free(kfft_inverse_fourier_state);
#if USE_MALLOC
    free(cx_cancelling_segments);
#endif
    return EXIT_SUCCESS;

fail:
    free(kfft_fourier_state);
    free(kfft_inverse_fourier_state);
#if USE_MALLOC
    free(cx_cancelling_segments);
#endif
    return EXIT_FAILURE;
}

int do_fourier_for_testing_purposes(void) {
    // The kiss_fft config.
    kiss_fft_cfg kfft_state;
    // kiss_fft's complex number type.
    kiss_fft_cpx cx_in[NSAMPLES];
    kiss_fft_cpx cx_out[NSAMPLES];
    kiss_fft_cpx cx_iout[NSAMPLES];

    // Initialise the fft's state buffer.
    // kfft_state is used internally by the fft function.
    // Elaboration on params:
    // NSAMPLES: Is this the number of samples?
    // FOURIER:    Do not use ifft.
    // 0:          Do not place the kfft_state in mem.
    // 0:          No length specify for mem, since we do not use it.
    kfft_state = kiss_fft_alloc(NSAMPLES, FOURIER, 0, 0);

    // Create signal
    create_signal(cx_in);

    // Perform fast fourier transform on a complex input buffer.
    // Elaboration on params:
    // kfft_state: The config used internally by the fft.
    // cx_in: input buffer which is an complex numbers array.
    // cx_out: output buffer which is an complex numbers array.
    kiss_fft(kfft_state, cx_in, cx_out);

    // Compare original signal with fourier transform.
    /* printf("Comparing cx_in and cx_out\n"); */
    /* epsilon_compare_signals(cx_in, cx_out, NSAMPLES); */

    /* intensity = sqrt(pow(cx_out[i].r,2) + pow(cx_out[i].i,2)); */
    /* printf("%d - %9.4f\n", i, intensity); */

    // Free data used for config.
    //kiss_fft_free(kfft_state);
    // According to comments in kiss_fft.c regular free should work just fine.
    free(kfft_state);

    // Allocate inverse fourier state.
    kfft_state = kiss_fft_alloc(NSAMPLES, INVERSE_FOURIER, 0, 0);
    ifft_and_restore(&kfft_state, cx_out, cx_iout, NSAMPLES);

    // Compare fourier transformed signal with inverse fourier transformed
    // signal.
    /* printf("Comparing cx_in and cx_iout\n"); */
    /* epsilon_compare_signals(cx_in, cx_iout, NSAMPLES); */

    printf("\ncx_in\n");
    print_signal(cx_in, NSAMPLES);
    /* printf("\ncx_out\n"); */
    /* print_signal(cx_out); */
    printf("\ncx_iout\n");
    print_signal(cx_iout, NSAMPLES);

    free(kfft_state);

    printf("\nDONE with do_fourier_for_testing_purposes!\n");
    return EXIT_SUCCESS;
}

void epsilon_compare_signals(const kiss_fft_cpx *s1, const kiss_fft_cpx *s2,
        int n) {
    int there_is_a_difference = 0;
    const double epsilon = 0.0001;

    for (int i = 0; i < n; ++i) {
        // Compare real and imaginary part of the signals.
        if (!epsilon_compare(s1[i].r, s2[i].r, epsilon))
            there_is_a_difference = 1;
            printf("s1[%d].r = %f\ts2[%d].r = %f\tRe differs.\n", i,
                    s1[i].r, i, s2[i].r);
        if (!epsilon_compare(s1[i].i, s2[i].i, epsilon))
            there_is_a_difference = 1;
            printf("s1[%d].i = %f\ts2[%d].i = %f\tIm differs.\n",
                    i, s1[i].i, i, s2[i].i);
        }
    if (there_is_a_difference) {
        putchar('\n');
    } else {
        printf("No difference between the signals.\n");
    }
}

int epsilon_compare(double d1, double d2, double epsilon) {
    if (fabs(d1 - d2) < epsilon) {
        return TRUE;
    }
    return FALSE;
}

void print_signal(const kiss_fft_cpx *s, const int n) {
    for (int i = 0; i < n; ++i) {
        printf("s[%d].r = %f\ts[%d].i = %f\n", i, s[i].r, i, s[i].i);
    }
}

void ifft_and_restore(const kiss_fft_cfg* state, const kiss_fft_cpx *in,
        kiss_fft_cpx *out, const int n) {
    // Perform the kiss fft inverse fourier transform.
    kiss_fft(*state, in, out);
    // We have to divide by n to actually restore the original signal, as
    // the numbers at every index of the signal are multiplied by n. I am
    // not sure why the numbers are multiplied.
    for (int i = 0; i < n; ++i) {
        out[i].r /= n;
        out[i].i /= n;
    }
}

void create_signal(kiss_fft_cpx* cx_in) {
    for (int i = 0; i < NSAMPLES; ++i) {
        // Fill the real part.
        // I did not want to put only 1's in so I made this if statement up.
        /* cx_in[i].r = 0.0; */
        /* if (i % 2 == 0) { */
        /*     /1* cx_in[i].r = 0.0; *1/ */
        /*     cx_in[i].r = (double) i; */
        /* } */
        cx_in[i].r = 1.0;
        // No imaginary part.
        /* cx_in[i].i = 0.0; */
        cx_in[i].i = (double) i;
    }

    /* // Conjugate symmetry of real signal. */
    /* for (int i = 0; i < NSAMPLES/2; ++i) { */
    /*     cx_in[NSAMPLES-i].r = cx_in[i].r; */
    /*     cx_in[NSAMPLES-i].i = - cx_in[i].i; */
    /* } */
}


// The following functions come from cutoff.c
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

double getCorrelation(double x[], double y[], int size_x, int size_y) {
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
        *(autoCorrelation + i) = getCorrelation(x, y, size_x, size_y);
    }

    return autoCorrelation;
}

int recognizeEnd(int start, unsigned long startMedium) {
    unsigned long average = 0;
    unsigned long counter = 0;
    // max noise length is the start moment plus 0.5 second.
    unsigned long maxCount = start + 3000;
    unsigned long maxLoop = 0;

    // If the max noise length exeeds the arraysize, use arraysize - safezone as
    // max noise length. Safezone is used because next 30 elements will be
    // looped.
    if(maxCount < data_array_size) {
        maxLoop = maxCount;
    } else {
        maxLoop = data_array_size - 30;
    }
    /* printf("start noise %d", start); */
    // Loop from start of noise to max 0.5 second further.
    for(int k = start; k < maxLoop; k++) {
        counter = 0;
        // Check if noise drasticly decreases in next 0.05 seconds 
        for(int i = k; i < (k + 30); i++) {
            counter += fabs(data_array[i]);
        }

        average = counter / 30;
        // If average is lower than the value at the start of the noise, noise
        // ended so function is stopped.
        if(average <= startMedium) {
            /* printf(" end noise %d \n", k); */
            return k;
        }
    }
    //No end of noise is detected, detected noise get discarded.
    /* printf("end noise \n"); */
    return maxLoop;
}

int do_recognize(void) {
    unsigned long counter = 0;
    unsigned long average = 0;
    unsigned long prevAverage = 0;
    unsigned int used = 0;

    /* int* startNoise = (int *)malloc(MAX_NSEGMENTS * (sizeof(*startNoise))); */
    /* int* endNoise = (int *)malloc(MAX_NSEGMENTS * (sizeof(*endNoise))); */
    /* double *autoCorrelation = (double *)malloc(data_array_size * sizeof(*autoCorrelation)); */
    /* double *seg_1 = (double *)malloc(data_array_size * sizeof(*seg_1)); */
    /* double *seg_2 = (double *)malloc(data_array_size * sizeof(*seg_2)); */
    /* double *seg_3 = (double *)malloc(data_array_size * sizeof(*seg_3)); */
    /* double *seg_4 = (double *)malloc(data_array_size * sizeof(*seg_4)); */

/*     if ( autoCorrelation == NULL || */
/*             seg_1 == NULL || seg_2 == NULL || seg_3 == NULL || seg_4 == NULL) { */
/*         fprintf(stderr, "ERROR: Malloc failed\n\n"); */

/*         free(autoCorrelation); */
/*         /1* free(startNoise); *1/ */
/*         /1* free(endNoise); *1/ */
/*         free(seg_1); */
/*         free(seg_2); */
/*         free(seg_3); */
/*         free(seg_4); */

/*         return EXIT_FAILURE; */
/*     } */

    // printf("data_array[683] = %f\n", data_array[683]);
    // Loop complete array, safezone of 400 because next 400 elements are looped
    // before check is reached
    num_noise_segments = 0;
    for (int k = 0; k < (data_array_size - 400); k += 400) {
        counter = 0;
        used = 0;
        // Loop next 400 ellements calculate average
        for (int i = k; i < (k + 400); i++) {
            int data = fabs(data_array[i]);
            // If value of data < 500 it can't be heard and is not usable
            if (data > 500) {
                counter += data;
                used++;
            }
        }
        // Save avarage of last loop
        prevAverage = average;
        // If used == 0 don't divide by 0, otherwise divide the counter by the
        // amount of time the counter is edited.
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
                /* *(startNoise + num_noise_segments) = k; */
                startNoise[num_noise_segments] = k;
                k = recognizeEnd(k, prevAverage - safeZone);
                /* *(endNoise + num_noise_segments) = k; */
                endNoise[num_noise_segments] = k;
                num_noise_segments++;
            }
        }
    }

    /* printf("Computing Autocorrelation \n\n"); */

    /* int sizeOfNoiseArray = 0; */

    /* /1* printf("num_noise_segments: %d\n", num_noise_segments); *1/ */
    /* for (int i = 0; i < num_noise_segments; ++i) { */
    /*     /1* printf("SEGMENT %d\n", i); *1/ */

    /*     /1* sizeOfNoiseArray = *(endNoise + i) - *(startNoise + i); *1/ */
    /*     sizeOfNoiseArray = endNoise[i] - startNoise[i]; */
    /*     double noiseArray[sizeOfNoiseArray]; */

    /*     /1* printf("sizeOfNoiseArray: %d\n", sizeOfNoiseArray); *1/ */

    /*     for (int y = 0; y < sizeOfNoiseArray; ++y) { */
    /*         /1* printf("sizeOfNoiseArray: %d\n", sizeOfNoiseArray); *1/ */
    /*         /1* noiseArray[y] = data_array[*(startNoise + i) + y]; *1/ */
    /*         noiseArray[y] = data_array[startNoise[i] + y]; */
    /*         if (i == 0){ */
    /*             *(seg_1 + y) = noiseArray[y]; */
    /*             /1* printf("%f\n", *(seg_1 + y)); *1/ */
    /*         } */
    /*         if (i == 1){ */
    /*             *(seg_2 + y) = noiseArray[y]; */
    /*             /1* printf("%f\n", *(seg_2 + y)); *1/ */
    /*         } */ 
    /*         if (i == 2){ */
    /*             *(seg_3 + y) = noiseArray[y]; */
    /*             /1* printf("%f\n", *(seg_3 + y)); *1/ */
    /*         } */
    /*         if (i == 3){ */
    /*             *(seg_4 + y) = noiseArray[y]; */
    /*             /1* printf("%f\n", *(seg_4 + y)); *1/ */
    /*         } */

    /*     } */

    /*     autoCorrelation = getAutoCorrelationOfSeries(noiseArray, */
    /*             autoCorrelation, sizeOfNoiseArray); */

    /*     for (int z = 0; z < sizeOfNoiseArray / 2; ++z) { */
    /* //        printf("%f -- %d -- %f\n", *(autoCorrelation + z), */
    /* //        sizeOfNoiseArray + z, data_array[sizeOfNoiseArray + z]); */
    /*     } */

    /*     printf("\n\n"); */
    /* } */

    
    printf("\nDONE with recognize!\n");

    /* free(autoCorrelation); */
    /* /1* free(startNoise); *1/ */
    /* /1* free(endNoise); *1/ */
    /* free(seg_1); */
    /* free(seg_2); */
    /* free(seg_3); */
    /* free(seg_4); */

    return EXIT_SUCCESS;
}

void print_noise_indices() {
    printf("\n############");
    printf(" Start and end indices of noises.");
    printf(" ############\n");
    for (int i = 0; i < num_noise_segments; ++i) {
        printf("startNoise[%d]:\t%d\n", i, startNoise[i]);
        printf("endNoise[%d]:\t%d\n", i, endNoise[i]);
        printf("size:\t\t%d\n", endNoise[i]-startNoise[i]);
    }
    printf("############\n\n");
}

int get_noise_segment(kiss_fft_cpx* cx_in, const int start_noise, const int
        end_noise) {
    const int num_segment_samples = end_noise - start_noise;
    // Ensure array bounds are correct.
    if (start_noise < 0 || end_noise > data_array_size ||
            num_segment_samples > data_array_size || num_segment_samples < 0) {
        fprintf(stderr, "get_noise_segment: error in segment bounds.\n");
        return EXIT_FAILURE;
    }
    // Put data_array noise segment in the kissfft complex array.
    // We assume cx_in is big enough to contain a noise segment.
    for (int i = 0, j = start_noise; j < end_noise; ++i, ++j) {
        cx_in[i].r = data_array[j];
        cx_in[i].i = 0.0;
    }
    return EXIT_SUCCESS;
}

int invert_frequencies(kiss_fft_cpx* cx_in, const int num_segment_samples) {
    for (int i = 0; i < num_segment_samples; ++i) {
        cx_in[i].r = -cx_in[i].r;
        cx_in[i].i = -cx_in[i].i;
    }
    return EXIT_SUCCESS;
}

void print_segment(const kiss_fft_cpx* s, const int n) {
    for (int i = 0; i < n; ++i) {
        printf("s[%d].r = %f\ts[%d].i = %f\n", i, s[i].r, i, s[i].i);
    }
}

#if USE_MALLOC
void print_segments(kiss_fft_cpx** segments, const int num_segments,
        const int* sizes) {
    for (int i = 0; i < num_segments; ++i) {
        printf("Segment:\t%d\n", i);
        print_segment(segments[i], sizes[i]);
    }
}
#else
void print_segments(const kiss_fft_cpx segments[MAX_NSEGMENTS][MAX_NSAMPLES],
        const int num_segments, const int* sizes) {
    for (int i = 0; i < num_segments; ++i) {
        printf("Segment:\t%d\n", i);
        print_segment(segments[i], sizes[i]);
    }
}
#endif

void** malloc2d(const int nrows, const int ncols , const size_t size) {
    void** p;
    p = malloc(sizeof(*p) * nrows);
    if (p == NULL) {
        fprintf(stderr, "malloc_2d_array: error in allocating space.\n");
        return NULL;
    }
    for (int i = 0; i < nrows; ++i) {
        p[i] = malloc(size * ncols);
        if(p[i] == NULL) {
            fprintf(stderr, "malloc_2d_array: error in allocating array.\n");
            free(p);
            return NULL;
        }
    }
    return p;
}
