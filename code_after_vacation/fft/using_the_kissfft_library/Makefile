compile:
	gcc main.c kissfft/kiss_fft.c kissfft/kiss_fft.h trainshortmicdata.h -o fft -lm

all:	
	gcc main.c kissfft/kiss_fft.{c,h} trainshortmicdata.h -o fft -lm
	./fft
gdb:
	gcc -g3 -lm main.c kissfft/kiss_fft.{c,h} trainshortmicdata.h -o fft
clean:
	rm fft
