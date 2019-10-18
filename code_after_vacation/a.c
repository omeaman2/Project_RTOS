#include<stdio.h>
#include<stdint.h>
#include<math.h>

#define N 44100

void main(){
	int16_t buf[N] = {0};
	int n;
	double Fs = 44100.0;

	for (n=0; n<N; ++n){
		buf[n] = 16383.0 * sin(n*1000.0*2.0*M_PI/Fs);
	}

	FILE *pipeout;
	pipeout = popen("ffmpeg -y -f s16le -ar 44100 -ac 1 -i - beep.wav", "w");
	fwrite(buf, 2, N, pipeout);
	pclose(pipeout);
}
