#include <stdio.h>
#include <math.h>
#include "kissfft/kiss_fft.h"
#include "../../initial noise detection/Cfiles/trainshortmicdata.c"

// Used in allocation of internal state for fourier or inverse fourier
// transform.
#define FOURIER 0
#define INVERSE_FOURIER 1

// Number of samples
#define NFFT 8

// A window function is a mathematical funtion that is zero-valued outside the
// specified interval. Usually window functions have a bell shaped curve. The
// result of applying a window function is that a function multiplied by the
// window function will also be zero-valued outside the specified interval.
//#define WIN 512

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
void print_signal(const kiss_fft_cpx *s);

// Do inverse Fourier transform and restore the original signal from a fourier
// transformed signal.
void ifft_and_restore(const kiss_fft_cfg* state, const kiss_fft_cpx *in,
        kiss_fft_cpx *out);

// Create a signal as input to the fast fourier transform.
void create_signal(kiss_fft_cpx *cx_in);

int main(void) {
    // The kiss_fft config.
    kiss_fft_cfg kfft_state;
    // kiss_fft's complex number type.
    kiss_fft_cpx cx_in[NFFT];
    kiss_fft_cpx cx_out[NFFT];
    kiss_fft_cpx cx_iout[NFFT];

    // Initialise the fft's state buffer.
    // kfft_state is used internally by the fft function.
    // Elaboration on params:
    // FOURIER:    Do not use ifft.
    // 0:          Do not place the kfft_state in mem.
    // 0:          No length specify for mem, since we do not use it.
    kfft_state = kiss_fft_alloc(NFFT, FOURIER, 0, 0);

    // Fill cx_in with data.
    create_signal(cx_in);

    // Perform fast fourier transform on a complex input buffer.
    // Elaboration on params:
    // kfft_state: The config used internally by the fft.
    // cx_in: input buffer which is an complex numbers array.
    // cx_out: output buffer which is an complex numbers array.
    kiss_fft(kfft_state, cx_in, cx_out);

    // Compare original signal with fourier transform.
    /* printf("Comparing cx_in and cx_out\n"); */
    /* epsilon_compare_signals(cx_in, cx_out, NFFT); */

    /* intensity = sqrt(pow(cx_out[i].r,2) + pow(cx_out[i].i,2)); */
    /* printf("%d - %9.4f\n", i, intensity); */

    // Free data used for config.
    //kiss_fft_free(kfft_state);
    // According to comments in kiss_fft.c regular free should work just fine.
    free(kfft_state);

    // Allocate inverse fourier state.
    kfft_state = kiss_fft_alloc(NFFT, INVERSE_FOURIER, 0, 0);
    ifft_and_restore(&kfft_state, cx_out, cx_iout);

    // Compare fourier transformed signal with inverse fourier transformed
    // signal.
    /* printf("Comparing cx_in and cx_iout\n"); */
    /* epsilon_compare_signals(cx_in, cx_iout, NFFT); */

    printf("\ncx_in\n");
    print_signal(cx_in);
    printf("\ncx_out\n");
    print_signal(cx_out);
    printf("\ncx_iout\n");
    print_signal(cx_iout);

    free(kfft_state);
    return 0;
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

void print_signal(const kiss_fft_cpx *s) {
    for (int i = 0; i < NFFT; ++i) {
        printf("s[%d].r = %f\ts[%d].i = %f\n", i, s[i].r, i, s[i].i);
    }
}

void ifft_and_restore(const kiss_fft_cfg* state, const kiss_fft_cpx *in,
        kiss_fft_cpx *out) {
    // Perform the kiss fft inverse fourier transform.
    kiss_fft(*state, in, out);
    // We have to divide by NFFT to actually restore the original signal, as
    // the numbers at every index of the signal are multiplied by NFFT. I am
    // not sure why the numbers are multiplied.
    for (int i = 0; i < NFFT; ++i) {
        out[i].r /= NFFT;
        out[i].i /= NFFT;
    }
}

void create_signal(kiss_fft_cpx *cx_in) {
    // Create signal
    for (int i = 0; i < NFFT; ++i) {
        // Fill the real part.
        cx_in[i].r = 1.0;
        // No imaginary part.
        /* cx_in[i].i = (double) i; */
    }

    /* // Conjugate symmetry of real signal. */
    /* for (int i = 0; i < NFFT/2; ++i) { */
    /*     cx_in[NFFT-i].r = cx_in[i].r; */
    /*     cx_in[NFFT-i].i = - cx_in[i].i; */
    /* } */
}
