#ifndef TRAINSHORTMICDATA_H
#define TRAINSHORTMICDATA_H
#include <inttypes.h>
#include <stddef.h>

// Used in allocation of internal state for fourier or inverse fourier
// transform.
#define FOURIER 0
#define INVERSE_FOURIER 1

// Max number of samples
#define MAX_NSAMPLES 3000

// Used in array initialisations.
#define MAX_NSEGMENTS 70

// Max size for the tokenizer used to create data_array.
#define MAX_TOKEN_SIZE 10

// maximum size for the data array.
#define MAX_DATA_ARRAY_SIZE 150000

// Epsilon used in comparison of signals.
#define EPSILON 0.1

// Set to use the heap on some places.
#define USE_MALLOC 1

// Number of samples, only used for testing.
#define NSAMPLES 16

// Recognition loop size. *8 because of the increased sample rate.
#define LOOP_SIZE 120*8

// File to read data_array from.
#define FILE_DATA_ARRAY "../../../resources/data_array.txt"

// File to save new_data_array.txt to.
#define FILE_NEW_DATA_ARRAY "../../../resources/new_data_array.txt"

/* Determines how much percent left and right of the highest frequency needs to
 * be cancelled in a noise segment. */
#define FREQ_CANCELLATION_PERCENTAGE 75.0

// Boolean functionality
enum boolean {
    TRUE = 1, 
    FALSE = 0,
};

// Error codes for use in functions
enum error_code {
    OK = 0x0000,
    NOT_OK = 0x0001,
};

// Data to simulate microphone input.
int16_t data_array[MAX_DATA_ARRAY_SIZE];

// Size of the simulate microphone input to be filled in main.c.
size_t data_array_size;


#endif /* TRAINSHORTMICDATA_H */
