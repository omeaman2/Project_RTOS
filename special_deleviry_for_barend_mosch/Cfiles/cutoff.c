#include <stdio.h>
#include "trainmicdata.c"
#include "math.h"

int arraySize = sizeof(dataarray) / sizeof(int);

int main()
{
	unsigned long counter = 0;
	unsigned long medium = 0;
	unsigned long prevMedium = 0;
	unsigned int used = 0;
	printf("%d size \n", arraySize);
	for(int k = 0; k < (arraySize - 3000); k+=3000){
		counter = 0;
		used = 0;
		for(int i = k; i < (k + 3000); i++)
		{
			int data = abs(dataarray[i]);
			if( data > 500 ){
				counter += data;
				used++;
			}
		}
		prevMedium = medium;
		if(used != 0){
			medium = counter / used;
		} else {
			medium = 0;
		}
		if(k != 0)
		{
			unsigned long safeZone = (prevMedium / 2);
			unsigned long tempMedium = prevMedium + safeZone;
			
			if(tempMedium < medium && prevMedium > 500) {
				k = recognizeEnd(k, prevMedium - safeZone);
			}
		}
	}
}

int recognizeEnd(int start, unsigned long startMedium){
	unsigned long medium = 0;
	unsigned long counter = 0;
	printf("start noise %d", start);
	for(int k = start; k < (arraySize - 250); k++){
		counter = 0;
		for(int i = k; i < (k + 250); i++)
		{
			counter += abs(dataarray[i]);
		}

		medium = counter / 250;
		
		if(medium <= startMedium) {
			printf(" end noise %d \n", k);
			return k;
		}
	}
	return arraySize;
}
