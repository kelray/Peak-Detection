#define _CRT_SECURE_NO_WARNINGS		//Disable errors

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>


#define MAXSIZE 1024

//unsigned int LocalSampleSize = 10;
int FindMax[] = { 1,20,3,4,50,60, 87,58,20,1,20,3, 4,50,6,88,58,20, 1,20,3,4,50,60, 87,58,20,1,20,3, 4,50,6,89,58,20 };
float FlArray[] = { 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.4, 1.3, 1.2, 1.1, 1.0, 0.9, 0.8, 0.7, 0.6, 0.5, 0.4, 0.3, 0.2, 0.1, -0.1 };
float FlArray2[] = { 0.13,0.83,0.17,0.52,0.69,0.83,0.47,0.46,0.82,0.29,0.21,0.28,0.23,0.14,0.44,0.02,0.26,0.77,0.47,0.02,0.22,0.84,0.02,0.20,0.13,0.20,0.77,0.17,0.31,0.45,0.63,0.34,0.15,0.10,0.15,0.38,0.09,0.45,0.59,0.61,0.84,0.93,0.39,0.91,0.15,0.04,0.81,0.69,0.88,0.56 };
int Array1[] = { 63, 80, 97, 120, 175, 191, 204, 245, 246, 251, 266, 272, 285, 294, 300, 302, 310, 352, 363, 370, 372, 387, 425, 440, 446, 463, 486, 492, 495, 498, 530, 634, 656, 670, 698, 727, 747, 755, 820, 843, 856, 862, 866, 876, 892, 920, 924, 970, 975, 997 };
int Array2[] = { 3, 12, 16, 31, 41, 58, 166, 188, 202, 219, 256, 277, 302, 340, 380, 385, 388, 402, 418, 422, 461, 510, 520, 530, 563, 591, 606, 619, 641, 654, 666, 684, 688, 708, 732, 747, 790, 795, 834, 854, 856, 903, 911, 912, 939, 951, 959, 968, 969, 984 };
int Array3[] = { -50, -49, -48, -47, -46, -44, -43, -41, -39, -37, -36, -34, -31, -27, -26, -22, -20, -17, -16, -18, -14, -13, -11, -10, -8, -7, -5, -3, -2, 3, 9, 12, 15, 16, 17, 19, 23, 25, 27, 28, 30, 32, 34, 35, 41, 43, 45, 46, 47, 48 };
int Array4[] = { -39, 44, 3, -17, -4, 38, -21, -13, -1, 22, 49, -48, 46, -32, -33, 43, 5, 27, -10, 19, 33, -40, 47, 29, -28, -26, 12, 6, -30, 41, 7, 11, 45, -27, 30, 4, 10, 13, 39, 40, -19, -47, 2, -25, 28, -3, -50, 16, -22, -14 };

//v.2
//void FindPeakV2(int *DataArray, unsigned int DataSize, unsigned int LocalSampleSize, int threshold, int *PeakVal, int *index)

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
	//it ends here
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
		//max = DataArray[counter];
		//Find maximum number in the window sample
		if (max < DataArray[counter])
		{
			max = DataArray[counter];
			PeakIndex = counter;		//keep track of the index of each peak
			//*index = counter;
			//index[i] = counter;
			//printf("1----The problematic element: %d, %d\n", max, counter);
		}
		//if (counter == 19)
			//printf("1----The problematic element: %d, %d\n", max, counter);

		//printf("-- max detected: %d\n", max);

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
				//if (counter == 19)
				//	printf("Another problematic line: %d, %d\n", max, counter);

				//Peak2 = max;
				//*PeakVal = max;
				PeakVal[i] = max;
				//index[i] = counter;
				index[i] = PeakIndex;
				//printf("-- peak detected %f at index: %d, counter: %d\n", PeakVal[i], index[i], counter);
				printf("-- peak detected %f at index: %d\n", PeakVal[i], index[i]);

				i++;
			}
			if (max < threshold)
			{
				//printf("No peaks detected\n");
				//*index = 0;
				//*PeakVal = 0;
				//max = 0;
			}
			//end of displaying part

			//printf("----> max detected: %d\n", max);

			//problem here with -ve numbers -> solved
			//Solution: max must be reset to the value of the minimum element in the array 
			//after each run through a window (of samples)
			max = min;	
					
			//check if counter reached the last element of the array
			if (counter >= (DataSize-1))//((sizeof(FindMax) / sizeof(int)) - 1))
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
	//int FindMax[] = { 1,20,3,4,50,6,87,58,20 };
	int i = 0, j = 0;
	//int threshold = 80;
	float threshold = 0.00;

	//int peak = 0, ind = 0, NumPeaks = 0;
	//int *peak = 0, *ind = 0, NumPeaks = 0;
	int ind[MAXSIZE], NumPeaks = 0;
	float peak[MAXSIZE];
	//int *peak[], *ind[], NumPeaks = 0;
	int SampleSize = 2;//sizeof(FindMax) / sizeof(int);
	int DataSize = 0;
	int counter = 0, StartIndex = 0;
	int Peak2 = 0, PeakIndex = 0;
	float fNumBuffer[315];
	float *peaks = 0;
	int *indeces = 0;

	char AccFileName[] = "log-05Sep2019-13-05-16.csv";
	//Open a file to and write array of numbers to it
	/*log = fopen("log.csv", "w+");
	for (j = 0; j < (sizeof(Array4) / sizeof(int)); j++)
	{
		fprintf(log, "%d,\n", Array4[j]);
	}*/
	
	//V.1
	//FindPeak(FindMax, SampleSize, threshold, &peak, &ind);
	//PeakDetector(FindMax, (sizeof(FindMax)/sizeof(int)), SampleSize, threshold, &peak, &ind, &NumPeaks);
	//printf("Peak detected of value %d at index: %d\n", peak, ind);

	//V1.1 with peak detection for an array of int numbers:
	//The function returns peak values, index of each peak and total number of peaks
	SampleSize = 10;
	threshold = 0.5;
	int ArraySize = 0;
	ParseCSV2float(AccFileName, 1, fNumBuffer, &ArraySize);	//parse float numbers from CSV file
	//ParseCSV2float("TestData1.csv", 2, fNumBuffer, &ArraySize);	//parse float numbers from CSV file
	
	//for (i = 0; i < ArraySize; i++)
		//printf("Data from CSV[%d]: %f\n", i, fNumBuffer[i]);

	PeakDetector(fNumBuffer, ArraySize, SampleSize, threshold, peak, ind, &NumPeaks);

	//PeakDetector(FlArray2, (sizeof(FlArray2)/sizeof(float)), SampleSize, threshold, peak, ind, &NumPeaks);
	
	//The moment of truth:
	//printf("Number of peaks %d\n", NumPeaks);
	//fseek(log, 0, SEEK_SET);
	printf("Array size: %d\nNumber of peaks %d\n", ArraySize, NumPeaks);
	for (i = 0; i < NumPeaks; i++)
	{
		printf("Peak detected %f at index %d\n", peak[i], ind[i]);
	}

	//using dynamic memory allocation
	//peaks = (float *)calloc(ArraySize, sizeof(float));
	
	//Finding maximum number in a specific window:
	//start counting from the start index, until you reach SampleSize+StartIndex value
	//at the beginning the StartIndex = 0, and it doesn't get updated until the counter
	//equals to (SampleSize + StartIndex) - 1, then the StartIndex is updated with the index of the
	//first element in the new window, then the counter repeats the same cycle but with a new
	//start condition = StartIndex, the loop breaks when the counter reaches the last element of the
	//input array
	/*for (counter = StartIndex; counter < (SampleSize+StartIndex); counter++)
	{
		if (max < FindMax[counter])
		{
			max = FindMax[counter];
		}

		if (counter == ((SampleSize + StartIndex) - 1))
		{
			StartIndex = counter + 1;
			printf("max is: %d\n", max);
			if (counter >= ((sizeof(FindMax) / sizeof(int))-1))
			{
				break;
			}
		}		
	}*/

	//Adding the peak detection piece 
	/*for (counter = StartIndex; counter < (SampleSize + StartIndex); counter++)
	{
		if (max < FindMax[counter])
		{
			max = FindMax[counter];
			PeakIndex = counter;
		}

		
		if (counter == ((SampleSize + StartIndex) - 1))
		{
			StartIndex = counter + 1;
			//printf("max is: %d\n", max);
			//Now display peaks above threshold
			if (max >= threshold)
			{
				Peak2 = max;
				printf("peak detected %d at index: %d\n", Peak2, PeakIndex);
			}
			if (max < threshold)
			{
				printf("No peaks detected\n");
				//*index = 0;
				//*PeakVal = 0;
			}
			//end of displaying part

			if (counter >= ((sizeof(FindMax) / sizeof(int)) - 1))
			{
				break;
			}
		}
	}*/

	//printf("Testing FindMaxNum(): %d\n", FindMaxNum(FindMax, (sizeof(FindMax) / sizeof(int))));

	_getch();
	fclose(log);
}