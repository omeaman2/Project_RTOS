#include "noisecancel.h"

void ifft_and_restore(const kiss_fft_cfg* state, const kiss_fft_cpx *in,
                      kiss_fft_cpx *out, const int n);

void vRTES_Noisecancel(void *pvParameters) {
    RTES_TaskParameters param = *(RTES_TaskParameters*) pvParameters;
    RTES_Buffer_t *in_buffer = param.in_buffer;
    RTES_Buffer_t *out_buffer = param.out_buffer;
    //const char * const pcTaskName = param.pcTaskName;
    TickType_t *taskPeriod = param.xTaskPeriod;

    TickType_t xTimeTaskStarted;
    for (;;) {
        xTimeTaskStarted = xTaskGetTickCount();

        do_noisecancel(in_buffer, out_buffer);

        vTaskDelayUntil(&xTimeTaskStarted, *taskPeriod);
    }
}

void do_noisecancel(RTES_Buffer_t *in_buffer, RTES_Buffer_t *out_buffer) {
    const int state_fourier = 0, state_inverse_fourier = 1;

    kiss_fft_cpx cx_cancelling_segment[in_buffer->used];
    
    /* State buffers used by kissfft */ 
    kiss_fft_cfg kfft_fourier_state = kiss_fft_alloc(in_buffer->used,
    state_fourier, 0, 0);
    kiss_fft_cfg kfft_inverse_fourier_state = kiss_fft_alloc(
    in_buffer->used, state_inverse_fourier, 0, 0);

   /* Get the noise segment */
    kiss_fft_cpx cx_noise_segment[in_buffer->used];
    for (size_t i = 0; i < in_buffer->used; i++) {
        /* Get sample from in_buffer */
        RTES_Sample_t sample = readFromBuffer(in_buffer, i); 
        removeFromBuffer(in_buffer);

        cx_noise_segment[i].r = sample.data; /* Real */
        cx_noise_segment[i].i = 0.0; /* Imaginary */
    }

    /* Compute Fourier to get the noise frequencies */ 
    kiss_fft_cpx cx_noise_segment_fourier[in_buffer->used];
    kiss_fft(kfft_fourier_state, cx_noise_segment, 
             cx_noise_segment_fourier);

    /* Invert all frequencies */
    for (size_t i = 0; i < in_buffer->used; i++) {
        cx_noise_segment_fourier[i].r = -cx_noise_segment_fourier[i].r;
        cx_noise_segment_fourier[i].i = -cx_noise_segment_fourier[i].i;
    }

    /* Compute inverse fourier to generate anti noise */
    ifft_and_restore(&kfft_inverse_fourier_state, cx_noise_segment_fourier,
    cx_cancelling_segment, in_buffer->used);

    /* Copy all data to output buffer */
    for (size_t i = 0; i < in_buffer->used; i++) {
        RTES_Sample_t sample = { cx_cancelling_segment[i].r };
        insertIntoBuffer(out_buffer, sample);
    }

    /* Free all used resources */
    free(kfft_fourier_state);
    free(kfft_inverse_fourier_state);
}

void ifft_and_restore(const kiss_fft_cfg* state, const kiss_fft_cpx *in,
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
}
