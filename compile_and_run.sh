#!/bin/sh
# Compiles and runs the project
#cd $(git rev-parse --show-cdup)
# Generate data_array.txt from data_array.wav
python 'src/python/generate_data_arr.py'
# Compiles and runs the c program. Creates to new_data_array.txt
make all -C src/c
# Generates new_data_array.wav from new_data_array.txt
python 'src/python/after_cancellation.py'
