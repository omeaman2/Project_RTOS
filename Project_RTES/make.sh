#!/bin/bash

gcc -o project_rtes -Wall main.c RTES.c Input/input.c Noisecancel/noisecancel.c Output/output.c Recognize/recognize.c
