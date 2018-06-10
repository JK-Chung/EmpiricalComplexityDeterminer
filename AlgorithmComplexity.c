#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int bubblesort(int array[], int length);
int isSorted(int array[], int length);

int mergesortMerge(int* subArrayL, int* subArrayR, int* endOfSubArrays);
int mergesort(int array[], int length);
int mergesortRecursiveHelper(int array[], int begin, int end);

void copyIntArray(int copyDestination[], int copySource[], int copySourceLength);
void fillIntArrayRandomly(int array[], int length);

void printIntArray(int array[], int length) {
	for(int i = 0; i < length - 1; i++)
		printf("%d, ", array[i]);
	printf("%d\n", array[length - 1]);
}

int main() {
	const int length = 5;
	int toSort[length];
	int test[5] = {1,3,5,2,4};
	fillIntArrayRandomly(toSort, length);
	printIntArray(test, length);
	
	
	mergesortMerge(&test[0], &test[3], &test[4]);
	printIntArray(test, length);
	printf("Is sorted?\t%d\n", isSorted(test, length));
	return 0;
}

int isSorted(int array[], int length) {
	for(int i = 0; i < length - 1; i++) {
		if(array[i] > array[i+1])
			return 0;
	}
	return 1;
}

int mergesort(int array[], int length) {
	mergesortRecursiveHelper(array, 0, length - 1);
}

int mergesortRecursiveHelper(int array[], int begin, int end) {
	if(begin >= end)
		return 0;
	
	int middleIndex = begin + (end - begin) / 2;
	printf("%d\t%d\t%d\t%d\n",begin,middleIndex,middleIndex+1,end);
	mergesortRecursiveHelper(array, begin, middleIndex);
	mergesortRecursiveHelper(array, middleIndex+1, end);
	printIntArray(array,5);
	mergesortMerge(&array[begin], &array[middleIndex+1], &array[end]);
	printIntArray(array,5);
	return 0; //temporary placeholder
}

/**
 * Merges two, sorted subarrays so that the result is also sorted. The two subarrays must be consecutive to each other within a greater array
 * This creates a copy of subArrayA so that subArrayA (within the greater array) can be modified. This is done to avoid copying both subarrays for better performance
**/
int mergesortMerge(int* subArrayL, int* subArrayR, int* endOfSubArrays) {
	if(subArrayL == subArrayR || subArrayR == endOfSubArrays)
		return 0;
	
	const int lengthOfSubArrayL = 1 + (subArrayR - subArrayL) / sizeof(int);
	
	int subArrayLCopy[lengthOfSubArrayL];
	copyIntArray(subArrayLCopy, subArrayL, lengthOfSubArrayL);
	
	// The following is only to make code easier to read
	int* nextPlaceToPutElement = subArrayL;
	subArrayL = subArrayLCopy;
	int* endOfSubArrayL = subArrayLCopy + (sizeof(int) * (lengthOfSubArrayL - 1));
	
	while(nextPlaceToPutElement <= endOfSubArrays) {
		if(subArrayL <= endOfSubArrayL && subArrayR <= endOfSubArrays) {
			if(*subArrayL <= *subArrayR) {
				*nextPlaceToPutElement = *subArrayL;
				subArrayL += sizeof(int);
			} else {
				*nextPlaceToPutElement = *subArrayR;
				subArrayR += sizeof(int);
			}
			printf("YES\n");
		} else if(subArrayL > endOfSubArrayL) {
			*nextPlaceToPutElement = *subArrayR;
			subArrayR += sizeof(int);
		} else {
			*nextPlaceToPutElement = *subArrayL;
			subArrayL += sizeof(int);
		}
		nextPlaceToPutElement += sizeof(int);
	}
	return 0; //placeholder
}

/**
 * Bubblesorts an integer array. Returns the number of element comparisons made
**/
int bubblesort(int array[], int length) {
	int isSorted = 0;
	int noOfComparisons = 0;
	
	int temp;
	while(!isSorted) {
		isSorted = 1;
		for(int i = 0; i < length - 1; i++) {
			if(array[i] > array[i + 1]) {
				temp = array[i];
				array[i] = array[i+1];
				array[i+1] = temp;
				
				isSorted = 0;
			}
			noOfComparisons++;
		}
	}
	return noOfComparisons;
}

/**
 * Fills an integer array with random elements using the current time as its seed
**/
void fillIntArrayRandomly(int array[], int length) {
	int randomSeed = time(NULL);
	srand(randomSeed);
	for(int i = 0; i < length; i++)
		array[i] = rand();
}

/**
 * Copies one int array over to another. This does not check if the destination array can hold all elements of the source.
 * If the source does not completely fill the destination array, the rest of the destination is left unchanged 
**/
void copyIntArray(int copyDestination[], int copySource[], int copySourceLength) {
	for(int i = 0; i < copySourceLength; i++)
		copyDestination[i] = copySource[i];
}