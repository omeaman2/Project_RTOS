#!/bin/bash

gcc -Wall main.c RTES.c Input/input.c Noisecancel/noisecancel.c Output/output.c Recognize/recognize.c kissfft/kiss_fft.c -o project_rtes -lm
