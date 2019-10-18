#include <stdio.h>
#include "micdata.c"
#include "math.h"

int main()
{
	int arraySize = sizeof(dataarray) / sizeof(int);
	unsigned long counter = 0;
	unsigned long medium = 0;
	unsigned long prevMedium = 0;
	printf("%d size", arraySize);
	for(int k = 0; k < (arraySize - 500); k+=500){
		counter = 0;
		for(int i = k; i < (k + 500); i++)
		{
			int data = abs(dataarray[i]);
			counter += data;

		}
		prevMedium = medium;
		medium = counter / 500;
		if(k != 0)
		{
			unsigned long tempSafeZone = prevMedium / 3;
			unsigned long safeZone = prevMedium + tempSafeZone;
			
			if(safeZone < medium) {
				printf("%u medium, %u prevmedium, %d timestamp \n ", medium, prevMedium, k);
			}
		}
	}
}
