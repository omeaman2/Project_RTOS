# Example usage:
# python3 wav_to_header_and_csv.py -wavfile '../wav/train_short.wav'
#  -csvfile '../csv/train_short.csv' -dataheader '../c/data.h'

from scipy.io import wavfile
import numpy as np
import os
import datetime
import argparse

# Handle command line arguments
parser = argparse.ArgumentParser()
parser.add_argument("-wavfile", help="wav file to convert to c header",
                    type=str, required=True)
parser.add_argument("-csvfile", help="name for the new csv file",
                    type=str, required=True)
parser.add_argument("-dataheader", help="name for the new c header",
                    type=str, required=True)
args = parser.parse_args()

# Read the wavfile as numpy array
wav_file = wavfile.read(args.wavfile)

# Source for numpy to csv string: https://stackoverflow.com/a/13861407
# Converts the numpy array to an array of strings
samples_array_strings = np.char.mod('%d', wav_file[1])
# Converts the array of strings to a single csv string
samples_csv_string =  ",".join(samples_array_strings)

# Construct the header file
header = """
// Created by {file} 
//  from {wavfile} 
//  on {datetime}

#ifndef DATA_H
#define DATA_H

#include <stddef.h>
#include <stdint.h>

typedef {datatype} sample_t;

static const uint32_t sample_rate = {sample_rate};
static const size_t number_of_samples = {number_of_samples};

static const sample_t data[] = {{
    {samples}
}};

static inline sample_t read_data(size_t index) {{ 
    return data[index % number_of_samples]; 
}};

#endif /* DATA_H */
""".format(file=os.path.basename(__file__), wavfile=args.wavfile,
           datetime=datetime.datetime.now(),
           datatype='int32_t', samples=samples_csv_string, 
           number_of_samples=wav_file[1].size, sample_rate=wav_file[0])

# Construct the csv file
csv = "{samples}".format(samples=samples_csv_string)

# Write the header file
f = open(args.dataheader, "w")
f.write(header)
f.close()

# Write the csv file
f = open(args.csvfile, "w")
f.write(csv)
f.close()

