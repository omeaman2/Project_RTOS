compile:
	gcc -lm main.c kissfft/kiss_fft.{c,h} trainshortmicdata.h -o fft

all:	
	gcc -lm main.c kissfft/kiss_fft.{c,h} trainshortmicdata.h -o fft
	./fft
clean:
	rm fft
