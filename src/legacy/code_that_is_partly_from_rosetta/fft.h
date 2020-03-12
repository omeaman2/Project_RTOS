/* Inplace FFT with O(n) memory usage. Note: array size is assumed to be power
 * of 2 and not checked by code; you can just pad it with 0 otherwise.
   https://rosettacode.org/wiki/Fast_Fourier_transform#C
*/

/*
 * Note by Kaydo
 *
 * This does not contain an inverse fft function. We either have to create an
 * ifft ourselves or use a different library after all. Either way, this
 * library looks relatively simple so it might be useful for understanding the
 * transform.
*/


#include <stdio.h>
#include <math.h>
#include <complex.h>
 
// Calculates PI based on the unit circle.
// Four times and angle of 45° is equal to 180° == π radians.
double PI = atan2(1, 1) * 4;
// Fourier transform makes use of complex numbers.
typedef double complex cplx;
 
// Fourier function to be recursively called.
void _fft(cplx buf[], cplx out[], int n, int step)
{
    // Initially step == 1.
    // So initially we certainly go into this if statement with an n of 8.
	if (step < n) {
        // Why are out and buf switched?
		_fft(out, buf, n, step * 2);
		_fft(out + step, buf + step, n, step * 2);
 
		for (int idx = 0; idx < n; idx += 2 * step) {
            // t = e^(i·π·1/n · out[idx+step])
			cplx t = cexp(-I * PI * idx / n) * out[idx + step];
			buf[idx / 2]     = out[idx] + t;
			buf[(idx + n)/2] = out[idx] - t;
		}
	}
}
 
void fft(cplx buf[], int size)
{
	cplx out[size];
    // Deep copy buf to out.
	for (int i = 0; i < size; i++)
        out[i] = buf[i];
	_fft(buf, out, size, 1);
}
 
 
// Show the data.
void show(const char * s, cplx buf[]) {
	printf("%s", s);
	for (int i = 0; i < 8; i++)
		if (!cimag(buf[i]))
			printf("%g ", creal(buf[i]));
		else
			printf("(%g, %g) ", creal(buf[i]), cimag(buf[i]));
}
