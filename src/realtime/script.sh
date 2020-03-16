#!/bin/bash

# Convert the wav file to the dataheader for the program to use
python3 python/wav_to_header_and_csv.py -wavfile 'wav/train_short.wav' -csvfile 'csv/train_short.csv' -dataheader 'c/data.h'

cd c

# Compile the program
./make.sh

# Run the program
./a.out

cd ..

# Create a wav file from the csv file outputted by the program
python3 python/csv_to_wav.py -csvfile csv/output.csv -wavfile wav/output.wav -samplerate 44100
