// This thing does not work currently as we cannot read a wave file without
// interpreting its header this way.
// Not like we need the wav file anyway.
// This file need to be converted so that we just use an internal array of
// complex numbers to execute the fft on.
#include <stdio.h>
#include <math.h>
#include "kissfft/kiss_fft.h"

// A window function is a mathematical funtion that is zero-valued outside the
// specified interval. Usually window functions have a bell shaped curve. The
// result of applying a window function is that a function multiplied by the
// window function will also be zero-valued outside the specified interval.
//#define WIN 512

// Compare and print differences in real || imaginary part of two signals.
void compare_signals(kiss_fft_cfg cfg, const kiss_fft_cpx *s1,
        const kiss_fft_cpx *s2, int n);

int main(void) {
    // Whatever this value is. Is this number of samples?
    int nfft = 256;

    // The kiss_fft config.
    kiss_fft_cfg kfft_state;
    // kiss_fft's complex number type.
    kiss_fft_cpx cx_in[nfft];
    kiss_fft_cpx cx_out[nfft];

    // Initialise the fft's state buffer.
    // kfft_state is used internally by the fft function.
    // Elaboration on params:
    // nfft: Is this the number of samples?
    // 0:    Do not use ifft.
    // 0:    Do not place the kfft_state in mem.
    // 0:    No length specify for mem, since we do not use it.
    kfft_state = kiss_fft_alloc(nfft, 0, 0, 0);

    // Put a signal into the the kissfft complex array.
    for (int i = 0; i < nfft; i++) {
        // Fill the real part.
        // I did not want to put only 1's in so I made this if up.
        /* if (i % 2 == 0) { */
        /*     cx_in[i].r = 0.0; */
        /* } */
        cx_in[i].r = 1.0;
        // No imaginary part.
        cx_in[i].i = 0.0;
    }

    /* // conjugate symmetry of real signal */ 
    /* for (int i= 0; i< nfft/2; ++i) { */
    /*     cx_in[nfft-i].r = cx_in[i].r; */
    /*     cx_in[nfft-i].i = - cx_in[i].i; */
    /* } */


    // Execute the fast fourier transform.
    // Elaboration on params:
    // kfft_state: The config used internally by the fft.
    // cx_in: input signal which is an complex numbers array.
    // cx_out: output signal which is an complex numbers array.
    kiss_fft(kfft_state, cx_in, cx_out);

    // Is there any difference between cx_in and cx_out?
    compare_signals(kfft_state, cx_in, cx_out, nfft);

    /* intensity = sqrt(pow(cx_out[i].r,2) + pow(cx_out[i].i,2)); */
    /* printf("%d - %9.4f\n", i, intensity); */

    // Free data used for config.
    //kiss_fft_free(kfft_state);
    // According to comments in kiss_fft.c regular free should work just fine.
    free(kfft_state);
    return 0;
}

void compare_signals(kiss_fft_cfg cfg, const kiss_fft_cpx *s1,
        const kiss_fft_cpx *s2, int n) {
    for (int i = 0; i < n; ++i) {
        // Compare real and imaginary part of the signals.
        if ((s1[i].r != s2[i].r) ) {
            printf("s1[%d].r = %f\ts2[%d].r = %f\tRe differs.\n", i,
                    s1[i].r, i, s2[i].r);
        }
        if (s1[i].i != s2[i].i) {
            printf("s1[%d].i = %f\ts2[%d].i = %f\tIm differs.\n",
                    i, s1[i].i, i, s2[i].i);
        }
    }
}
