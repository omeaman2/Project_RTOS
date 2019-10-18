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
 
double PI = atan2(1, 1) * 4;
typedef double complex cplx;
 
void _fft(cplx buf[], cplx out[], int n, int step)
{
	if (step < n) {
		_fft(out, buf, n, step * 2);
		_fft(out + step, buf + step, n, step * 2);
 
		for (int i = 0; i < n; i += 2 * step) {
			cplx t = cexp(-I * PI * i / n) * out[i + step];
			buf[i / 2]     = out[i] + t;
			buf[(i + n)/2] = out[i] - t;
		}
	}
}
 
void fft(cplx buf[], int n)
{
	cplx out[n];
	for (int i = 0; i < n; i++) out[i] = buf[i];
 
	_fft(buf, out, n, 1);
}
 
 
void show(const char * s, cplx buf[]) {
	printf("%s", s);
	for (int i = 0; i < 8; i++)
		if (!cimag(buf[i]))
			printf("%g ", creal(buf[i]));
		else
			printf("(%g, %g) ", creal(buf[i]), cimag(buf[i]));
}
