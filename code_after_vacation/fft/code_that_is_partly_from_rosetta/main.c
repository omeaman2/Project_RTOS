#include "fft.h"

#define SIZE 8

int main(void) {
    printf("%f\n", PI);
	cplx buf[SIZE] = {1, 1, 1, 1, 0, 0, 0, 0};
    // Data before fft.
	show("Data: ", buf);
	fft(buf, SIZE);
    // Data after fft.
	show("\nFFT : ", buf);
    return 0;
}
