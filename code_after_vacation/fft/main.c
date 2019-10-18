#include "fft.h"

int main(void) {
    printf("%f\n", PI);
	cplx buf[] = {1, 1, 1, 1, 0, 0, 0, 0};
	show("Data: ", buf);
	fft(buf, 8);
	show("\nFFT : ", buf);
    return 0;
}
