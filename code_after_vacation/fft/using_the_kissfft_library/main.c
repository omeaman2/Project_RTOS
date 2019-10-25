#include <stdio.h>
#include <math.h>
#include "kissfft/kiss_fft.h"

// Used in allocation of internal state for fourier or inverse fourier
// transform.
#define FOURIER 0
#define INVERSE_FOURIER 1

// A window function is a mathematical funtion that is zero-valued outside the
// specified interval. Usually window functions have a bell shaped curve. The
// result of applying a window function is that a function multiplied by the
// window function will also be zero-valued outside the specified interval.
//#define WIN 512

// Deprecated
// Compare and print differences in real || imaginary part of two signals.
void compare_signals(const kiss_fft_cpx *s1, const kiss_fft_cpx *s2, int n);

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
void print_signal(const kiss_fft_cpx *s, int nfft);

// Do inverse Fourier transform and restore the original signal from a fourier
// transformed signal.
void ifft_and_restore(const kiss_fft_cfg* state, const kiss_fft_cpx *in,
        kiss_fft_cpx *out, int nfft);

int main(void) {
    // Number of samples?
    int nfft = 8;

    // The kiss_fft config.
    kiss_fft_cfg kfft_state;
    // kiss_fft's complex number type.
    kiss_fft_cpx cx_in[nfft];
    kiss_fft_cpx cx_out[nfft];
    kiss_fft_cpx cx_iout[nfft];

    // Initialise the fft's state buffer.
    // kfft_state is used internally by the fft function.
    // Elaboration on params:
    // nfft: Is this the number of samples?
    // FOURIER:    Do not use ifft.
    // 0:          Do not place the kfft_state in mem.
    // 0:          No length specify for mem, since we do not use it.
    kfft_state = kiss_fft_alloc(nfft, FOURIER, 0, 0);

    // Create signal
    for (int i = 0; i < nfft; ++i) {
        // Fill the real part.
        // I did not want to put only 1's in so I made this if statement up.
        /* if (i % 2 == 0) { */
        /*     cx_in[i].r = 0.0; */
        /* } */
        /* cx_in[i].r = 1.0; */
        cx_in[i].r = (double) i;
        // No imaginary part.
        /* cx_in[i].i = 0.0; */
        cx_in[i].i = (double) i;
    }

    /* // Conjugate symmetry of real signal. */
    /* for (int i = 0; i < nfft/2; ++i) { */
    /*     cx_in[nfft-i].r = cx_in[i].r; */
    /*     cx_in[nfft-i].i = - cx_in[i].i; */
    /* } */


    // Perform fast fourier transform on a complex input buffer.
    // Elaboration on params:
    // kfft_state: The config used internally by the fft.
    // cx_in: input buffer which is an complex numbers array.
    // cx_out: output buffer which is an complex numbers array.
    kiss_fft(kfft_state, cx_in, cx_out);

    // Compare original signal with fourier transform.
    /* printf("Comparing cx_in and cx_out\n"); */
    /* epsilon_compare_signals(cx_in, cx_out, nfft); */

    /* intensity = sqrt(pow(cx_out[i].r,2) + pow(cx_out[i].i,2)); */
    /* printf("%d - %9.4f\n", i, intensity); */

    // Free data used for config.
    //kiss_fft_free(kfft_state);
    // According to comments in kiss_fft.c regular free should work just fine.
    free(kfft_state);

    // Allocate inverse fourier state.
    kfft_state = kiss_fft_alloc(nfft, INVERSE_FOURIER, 0, 0);
    ifft_and_restore(&kfft_state, cx_out, cx_iout, nfft);

    // Compare fourier transformed signal with inverse fourier transformed
    // signal.
    /* printf("Comparing cx_in and cx_iout\n"); */
    /* epsilon_compare_signals(cx_in, cx_iout, nfft); */

    printf("\ncx_in\n");
    print_signal(cx_in, nfft);
    printf("\ncx_out\n");
    print_signal(cx_out, nfft);
    printf("\ncx_iout\n");
    print_signal(cx_iout, nfft);

    free(kfft_state);
    return 0;
}

// Deprecated.
void compare_signals(const kiss_fft_cpx *s1, const kiss_fft_cpx *s2, int n) {
    int there_is_a_difference = 0;
    for (int i = 0; i < n; ++i) {
        // Compare real and imaginary part of the signals.
        if ((s1[i].r != s2[i].r) ) {
            there_is_a_difference = 1;
            printf("s1[%d].r = %f\ts2[%d].r = %f\tRe differs.\n", i,
                    s1[i].r, i, s2[i].r);
        }
        if (s1[i].i != s2[i].i) {
            there_is_a_difference = 1;
            printf("s1[%d].i = %f\ts2[%d].i = %f\tIm differs.\n",
                    i, s1[i].i, i, s2[i].i);
        }
    }
    if (there_is_a_difference) {
        putchar('\n');
    } else {
        printf("No difference between the signals.\n");
    }
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
        return 1;
    }
    return 0;
}

void print_signal(const kiss_fft_cpx *s, int nfft) {
    for (int i = 0; i < nfft; ++i) {
        printf("s[%d].r = %f\ts[%d].i = %f\n", i, s[i].r, i, s[i].i);
    }
}

void ifft_and_restore(const kiss_fft_cfg* state, const kiss_fft_cpx *in,
        kiss_fft_cpx *out, int nfft) {
    // Perform the kiss fft inverse fourier transform.
    kiss_fft(*state, in, out);
    // We have to divide by nfft to actually restore the original signal, as
    // the numbers at every index of the signal are multiplied by nfft. I am
    // not sure why the numbers are multiplied.
    for (int i = 0; i < nfft; ++i) {
        out[i].r /= nfft;
        out[i].i /= nfft;
    }
}
