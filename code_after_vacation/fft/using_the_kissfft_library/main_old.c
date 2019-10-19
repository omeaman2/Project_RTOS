#include <stdio.h>
#include <math.h>
#include "kissfft/kiss_fft.h"

#define WIN 512

int main()
{
    char *wav = "./knock.wav";
    FILE *in;
    char buf[WIN * 2];
    int nfft = WIN, i, fx;
    double intensity = 0;
    kiss_fft_cfg cfg;
    kiss_fft_cpx cx_in[WIN];
    kiss_fft_cpx cx_out[WIN];
    short *sh;

    cfg = kiss_fft_alloc(nfft, 0, 0, 0);
    in = fopen(wav, "r");
    if (!in) {
        printf("unable to open file: %s\n", wav);
        perror("Error");
        return 1;
    }
    fx = 0;
    while (fread(buf, 1, WIN * 2, in)) 
    {
        for (i = 0;i<WIN;i++) {
            sh = (short *)&buf[i * 2];
            cx_in[i].r = (float) (((double)*sh) / 32768.0);
            cx_in[i].i = 0.0;
        }

        kiss_fft(cfg, cx_in, cx_out);
        //Display the value of a position
        int position = 511;
        intensity = sqrt(pow(cx_out[position].r, 2) + pow(cx_out[position].i, 2));
        printf("%9.4f\n", intensity);

        //Display all values
        /*
        for (i = 0;i<WIN;i++) {
            //printf("Joe: cx_out[i].r:%f\n", cx_out[i].r);
            //printf("Joe: cx_out[i].i:%f\n", cx_out[i].i);
            intensity = sqrt(pow(cx_out[i].r,2) + pow(cx_out[i].i,2));
            printf("%d - %9.4f\n", i, intensity);
        }
        */

    }
    free(cfg);
    scanf("%d");

    return 0;
}
