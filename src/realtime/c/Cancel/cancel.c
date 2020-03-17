#include "cancel.h"

void doFFT(sample_t input[], sample_t output[], size_t size, double cancelPercentage);

/***** Copied from main.c non-realtime *****/
// Used in allocation of internal state for fourier or inverse fourier
// transform.
#define FOURIER 0
#define INVERSE_FOURIER 1

// Error codes for use in functions
enum error_code {
    OK = 0x0000,
    NOT_OK = 0x0001,
};

// Do inverse Fourier transform and restore the original signal from a fourier
// transformed signal.
int ifft_and_restore(const kiss_fft_cfg* state, const kiss_fft_cpx *in,
        kiss_fft_cpx *out, const int n);

/* Return index of highest absolute real frequency. */
size_t highest_frequency_real(const kiss_fft_cpx *s, const size_t size);
/* Return index of highest absolute imaginary frequency. */
size_t highest_frequency_imag(const kiss_fft_cpx *s, const size_t size);
/* Cancel x% around the highest absolute frequency in a complex numbered
 * fourier transformed signal.*/
int cancel_interval(kiss_fft_cpx *s, const size_t size, double percent);
/***** End of copied from main.c non-realtime *****/

void vTaskCancel(void *pvParameters) {
    cancelSettings_t *settings = (cancelSettings_t*) pvParameters;

    TickType_t xTimeTaskStarted;
    for (;;) {
        xTimeTaskStarted = xTaskGetTickCount();

        doCancel(settings);

        vTaskDelayUntil(&xTimeTaskStarted, settings->base.xTaskPeriod);
    }
}

void doCancel(cancelSettings_t *settings) {
    size_t size = settings->inBuffer->used;
    if (size == 0) return;

    /* Prepare input and output arrays */    
    sample_t *input = getNewEmptyArray(size);
    sample_t *output = getNewEmptyArray(size);

    /* Copies the contents of inBuffer to array input */
    copyArrayFromBuffer(input, settings->inBuffer, size, 0);
    
    /* Perform FFT on the array input, put result in array output */
    doFFT(input, output, size, settings->cancelPercentage);
    
    /* Copy array output to outBuffer */
    copyBufferFromArray(settings->outBuffer, output, size);

    /* Clear inBuffer and free the input & output arrays */
    removeFromBuffer(settings->inBuffer, size);
    free(input);
    free(output);
}

void doFFT(sample_t input[], sample_t output[], size_t size, double cancelPercentage) {
    kiss_fft_cpx *cx_cancelling_segment = (kiss_fft_cpx*)malloc(size * sizeof(kiss_fft_cpx));

    /* State buffers used by kissfft */
    kiss_fft_cfg fft_state = kiss_fft_alloc(size, FOURIER, 0, 0);
    kiss_fft_cfg ifft_state = kiss_fft_alloc(size, INVERSE_FOURIER, 0, 0);

    /* 1. Get the noise segment from the input array */
	kiss_fft_cpx *cx_noise_segment = (kiss_fft_cpx*)malloc(size * sizeof(kiss_fft_cpx));
    for (size_t i = 0; i < size; i++) {
        cx_noise_segment[i].r = input[i]; // Real
        cx_noise_segment[i].i = 0.0; // Imaginary
    }

    /* 2. Compute fourier to get the noise frequencies */
	kiss_fft_cpx* cx_noise_segment_fourier = (kiss_fft_cpx*)malloc(size * sizeof(kiss_fft_cpx));
    kiss_fft(fft_state, cx_noise_segment, cx_noise_segment_fourier);

    /* 3. Perform algorithm to cancel only the highest absolute frequencies of the
           fourier transformed signal. */
    cancel_interval(cx_noise_segment_fourier, size, cancelPercentage);

    /* 4. Compute inverse fourier to generate cancelling noise */
    ifft_and_restore(&ifft_state, cx_noise_segment_fourier, 
                     cx_cancelling_segment, size);

    /* Copy the cancelling noise to the output array */
    for (size_t i = 0; i < size; i++) {
        output[i] = cx_cancelling_segment[i].r;
    }
    
    /* Free all used resources */
    free(fft_state);
    free(ifft_state);
	free(cx_cancelling_segment);
	free(cx_noise_segment);
	free(cx_noise_segment_fourier);
}

/***** Functions copied from main.c non-realtime *****/

int ifft_and_restore(const kiss_fft_cfg* state, const kiss_fft_cpx *in,
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
    return OK;
}

/* Return index of highest absolute real frequency. */
size_t highest_frequency_real(const kiss_fft_cpx *s, const size_t size) {
    size_t current_highest = 0;
    for (int i = 1; i < size; ++i) {
        if (fabs(s[i].r) > fabs(s[current_highest].r)) {
            current_highest = i;
        }
    }
    return current_highest;
}

/* Return index of highest absolute imaginary frequency. */
size_t highest_frequency_imag(const kiss_fft_cpx *s, const size_t size) {
    size_t current_highest = 0;
    for (int i = 1; i < size; ++i) {
        if (fabs(s[i].i) > fabs(s[current_highest].i)) {
            current_highest = i;
        }
    }
    return current_highest;
}

/* Set x% around the absolute highest frequency to zero. */
int cancel_interval(kiss_fft_cpx *s, const size_t size, double percent) {
    size_t interval, hfreq_idx_re, hfreq_idx_im;
    int beg_re, beg_im, end_re, end_im;

    if (percent < 0 || percent > 100) return NOT_OK;
    interval = percent/100.0 * size;

    /* Get the index of the highest frequency of the real and imaginary parts.*/
    hfreq_idx_re = highest_frequency_real(s, size);
    hfreq_idx_im = highest_frequency_imag(s, size);

    /* Get begin and end intervals. */
    beg_re = hfreq_idx_re - interval;
    beg_im = hfreq_idx_im - interval;
    end_re = hfreq_idx_re + interval;
    end_im = hfreq_idx_im + interval;

    if (beg_re < 0)    beg_re = 0;
    if (beg_im < 0)    beg_im = 0;
    if (end_re > size) end_re = size;
    if (end_im > size) end_im = size;

    /* Set frequencies to zero */
    for (int i = beg_re; i < end_re; ++i) {
        s[i].r = 0.0;
    }
    for (int i = beg_im; i < end_im; ++i) {
        s[i].i = 0.0;
    }

    /* /1* Invert frequencies *1/ */
    /* for (int i = beg_re; i < end_re; ++i) { */
    /*     s[i].r = -s[i].r; */
    /* } */
    /* for (int i = beg_im; i < end_im; ++i) { */
    /*     s[i].i = -s[i].i; */
    /* } */

    return OK;
}
