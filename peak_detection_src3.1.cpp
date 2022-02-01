#define _CRT_SECURE_NO_WARNINGS		//Disable errors - only for visual studio

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
FILE * log;

void ParseCSV2float(char *fileName, int column, float *outputArray, int *outputArraySize)
{
	char *pch = NULL;
	int i = 0;
	int DelimCount = 0;
	char Buffer[512];
	FILE * file;

	file = fopen(fileName, "r+");
	rewind(file);

	//This code parses each column of a CSV file	    
	while (!feof(file))
	{
		fgets(Buffer, sizeof(Buffer), file);   //Reads the file line-by-line
		pch = strtok(Buffer, ",");
		while (pch != NULL)
		{
			DelimCount++;
			if (DelimCount == column)
			{
				outputArray[i] = atof(pch);
				//printf("float number is %f\n", outputArray[i]);
				i++;
				DelimCount = 0;
				break;
			}
			pch = strtok(NULL, ",");

		}
	}
	*outputArraySize = i;
	//printf("outputArraySize is: %d\n", outputArraySize);
	fclose(file);
}

void PeakDetector(float *DataArray, unsigned int DataSize, unsigned int SampleSize, float threshold, float *PeakVal, int *index, int *NumOfPeaks)
{
	int counter = 0, StartIndex = 0;
	int PeakIndex = 0;
	int i = 0;
	float max = 0.0, Peak2 = 0.0, min = 0.0;

	//Adding the peak detection piece 
	//start counting from the first element in the window sample to the last one indicated by SampleSize + StartIndex
	//counting starts at 0 and StartIndex = 0 in the very first run 

	max = DataArray[0];	//initialize "max" with the first element in the input DataArray

	//find minimum number in the array to use as a reset value after every cycle, check below
	while (i < (sizeof(DataArray) / sizeof(float)))
	{
		if (min > DataArray[i])
		{
			min = DataArray[i];
		}
		i++;
	}
	i = 0;

	for (counter = StartIndex; counter < (SampleSize + StartIndex); counter++)
	{
		//Find maximum number in the window sample
		if (max < DataArray[counter])
		{
			max = DataArray[counter];
			PeakIndex = counter;		//keep track of the index of each peak
		}

		//check if counter reached the end of the first window, then update StartIndex
		//with the new value of counter+1, which is the start of the 1st element in the new window
		//of samples
		if (counter == ((SampleSize + StartIndex) - 1))
		{
			StartIndex = counter + 1;
			//printf("max is: %d\n", max);
			//Now display peaks above threshold
			//check if the peak number is above the threshold
			if (max >= threshold)
			{
				PeakVal[i] = max;
				index[i] = PeakIndex;
				//printf("-- peak detected %f at index: %d\n", PeakVal[i], index[i]);
				i++;
			}
			if (max < threshold)
			{
				//printf("No peaks detected\n");
				
			}
			//printf("----> max detected: %d\n", max);

			//problem here with -ve numbers -> solved
			//Solution: max must be reset to the value of the minimum element in the array 
			//after each run through a window (of samples)
			max = min;	
					
			//check if counter reached the last element of the array
			if (counter >= (DataSize-1))
			{
				break;
			}
		}
	}
	*NumOfPeaks = i;
}

void main()
{
	int max = 0, min = 0;
	int i = 0, j = 0;
	float threshold = 0.00;
	int ind[MAXSIZE], NumPeaks = 0;
	float peak[MAXSIZE];
	int SampleSize = 2;
	int DataSize = 0;
	int counter = 0, StartIndex = 0;
	int Peak2 = 0, PeakIndex = 0;
	float fNumBuffer[315];
	float *peaks = 0;
	int *indeces = 0;

	char AccFileName[] = "log-05Sep2019-13-05-16.csv";
	
	//The function returns peak values, index of each peak and total number of peaks
	SampleSize = 10;
	threshold = 0.5;
	int ArraySize = 0;
	ParseCSV2float(AccFileName, 1, fNumBuffer, &ArraySize);	//parse float numbers from CSV file
	PeakDetector(fNumBuffer, ArraySize, SampleSize, threshold, peak, ind, &NumPeaks);

	printf("Array size: %d\nNumber of peaks %d\n", ArraySize, NumPeaks);
	for (i = 0; i < NumPeaks; i++)
	{
		printf("Peak detected %f at index %d\n", peak[i], ind[i]);
	}
	_getch();
	fclose(log);
}
