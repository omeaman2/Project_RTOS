compile:
	gcc -lm main.c kissfft/kiss_fft.c kissfft/kiss_fft.h constants.h -o main

all:	
	gcc -lm main.c kissfft/kiss_fft.{c,h} constants.h -o main
	./main
gdb:
	gcc -g3 -lm main.c kissfft/kiss_fft.{c,h} constants.h -o main
clean:
	rm main
