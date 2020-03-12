#!/bin/sh
# Compiles and runs the project
cd $(git rev-parse --show-cdup)
# Generate data_array.txt from data_array.wav
python 'code_after_vacation/initial noise detection/python/generate_data_arr.py'
# Compiles and runs the c program. Creates to new_data_array.txt
make all -C code_after_vacation/fft/using_the_kissfft_library/
# Generates new_data_array.wav from new_data_array.txt
python 'code_after_vacation/output/after_cancellation.py'
