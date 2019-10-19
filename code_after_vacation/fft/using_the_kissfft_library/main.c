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
#define WIN 512

int main(void) {
    char file_string[] = "./knock.wav";
    FILE *in;
    char buf[WIN * 2];
    int nfft = WIN;
    double intensity = 0;
    // config
    kiss_fft_cfg cfg;
    // kiss_fft's complex number type.
    kiss_fft_cpx cx_in[WIN];
    kiss_fft_cpx cx_out[WIN];
    short *sh;

    // Initialise the fft's state buffer.
    // cfg is used internally by the fft function.
    // Elaboration on params:
    // nfft: Is this the number of samples?
    // 0:    Do not use ifft.
    // 0:    Do not place the cfg in mem.
    // 0:    No length specify for mem, since we do not use it.
    cfg = kiss_fft_alloc(nfft, 0, 0, 0);

    // Initialise file.
    // read-only and read binary
    in = fopen(file_string, "rb");
    if (!in) {
        printf("unable to open file: %s\n",file_string);
        perror("Error");
        return 1;
    }
    // Read byte by byte, WIN * 2 items of data from in into buf.
    while (fread(buf, 1, WIN * 2, in)) 
    {
        for (int i = 0; i < WIN; i++) {
            sh = (short *) &buf[i * 2];
            // What is this 32768.0 value?
            cx_in[i].r = (float) (((double)*sh) / 32768.0);
            cx_in[i].i = 0.0;
        }

        // Execute the fast fourier transform.
        // Elaboration on params:
        // cfg: The config used internally by the fft.
        // cx_in: input signal which is an complex numbers array.
        // cx_out: output signal which is an complex numbers array.
        kiss_fft(cfg, cx_in, cx_out);

        int position = 511;
        intensity = sqrt(pow(cx_out[position].r, 2) + pow(cx_out[position].i, 2));
        /* printf("%9.4f\n", intensity); */

        // Print all values.
        for (int i = 0;i<WIN;i++) {
            printf("cx_out[i].r:%f\n", cx_out[i].r);
            printf("cx_out[i].i:%f\n", cx_out[i].i);
            intensity = sqrt(pow(cx_out[i].r,2) + pow(cx_out[i].i,2));
            printf("%d - %9.4f\n", i, intensity);
        }

    }
    // Free data used for config.
    //kiss_fft_free(cfg);
    // According to the comments in kiss_fft.c this should work.
    free(cfg);
    return 0;
}
