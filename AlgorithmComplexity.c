#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define toSortLength 500

int selectionsort(int array[], int length);
int insertionsort(int array[], int length);

int bubblesort(int array[], int length);
int isSorted(int array[], int length);

int quicksort(int array[], int length);
int quicksortRecursiveHelper(int array[], int front, int rear);
int partition(int array[], int front, int rear, int* pivotIndexToReturn);

int mergesortMerge(int array[], int subArrayLIndex, int subArrayRIndex, int subArraysLength);
int mergesort(int array[], int length);
int mergesortRecursive(int array[], int begin, int end);

void copyIntArray(int copyDestination[], int copySource[], int copySourceLength);
void fillIntArrayRandomly(int array[], int length);
void swapIntegerValues(int* a, int* b);
int* getSmallestInteger(int array[], int length);

void printIntArray(int array[], int length) {
	for(int i = 0; i < length - 1; i++)
		printf("%d, ", array[i]);
	printf("%d\n", array[length - 1]);
}

int main() {
	int toSort[toSortLength];
	fillIntArrayRandomly(toSort, toSortLength);

	//printIntArray(toSort, toSortLength);
	printf("Complexity:\t%d\n", selectionsort(toSort, toSortLength));
	//printIntArray(toSort, toSortLength);

	printf("Is sorted?\t%d\n", isSorted(toSort, toSortLength));
	return 0;
}

int isSorted(int array[], int length) {
	for(int i = 0; i < length - 1; i++) {
		if(array[i] > array[i+1])
			return 0;
	}
	return 1;
}

int selectionsort(int array[], int length) {
    int complexity = 0;

    for(int i = 0; i < length; i++) {
        swapIntegerValues(getSmallestInteger(&array[i], length-i), &array[i]);
        complexity += length - i;
    }

    return complexity;
}

int insertionsort(int array[], int length) {
    int noOfComparisons = 0;
    
    for(int i = 1; i < length; i++) {
        while(array[i-1] > array[i] && i != 0) {
            swapIntegerValues(&array[i-1], &array[i]);
            i--;
            noOfComparisons++;
        }
    }

    return noOfComparisons;
}

/**
 * Quicksorts an array with fewer arguments required that that of the quicksortRecursiveHelper
**/
int quicksort(int array[], int length) {
    return quicksortRecursiveHelper(array, 0, length-1);
}

/**
 * Carries out quicksort on a subarray recursively
**/
int quicksortRecursiveHelper(int array[], int front, int rear) {
    if(front >= rear)
        return 0;

    int complexity = 0;
    int pivotIndex;
    complexity += partition(array, front, rear, &pivotIndex);
    complexity += quicksortRecursiveHelper(array, front, pivotIndex-1);
    complexity += quicksortRecursiveHelper(array, pivotIndex+1, rear);

    return complexity;
}

/**
 * Takes the first element of the subarray specified by array[], front and rear and rearranges the subarray so that all elements to the left
 * of that first element is less than or equal to it and all to the right are greater. The final index of this element is stored in the pointer argument
 * This function will return the number of times that an element of the array is accessed.
**/
int partition(int array[], int front, int rear, int* pivotIndexToReturn) {
    int pivotIndex = front;
    int indexToCheckAgainstPivot = rear;
    int noOfElementAccesses = 0;

    int temp;
    while(pivotIndex != indexToCheckAgainstPivot) {
        while(pivotIndex < indexToCheckAgainstPivot) {
            if(array[pivotIndex] >= array[indexToCheckAgainstPivot]) {
                swapIntegerValues(&array[pivotIndex], &array[indexToCheckAgainstPivot]);
                swapIntegerValues(&pivotIndex, &indexToCheckAgainstPivot);
                indexToCheckAgainstPivot++;
            } else
                indexToCheckAgainstPivot--;
            noOfElementAccesses++;
        }
        while(pivotIndex > indexToCheckAgainstPivot) {
            if(array[pivotIndex] <= array[indexToCheckAgainstPivot]) {
                swapIntegerValues(&array[pivotIndex], &array[indexToCheckAgainstPivot]);
                swapIntegerValues(&pivotIndex, &indexToCheckAgainstPivot);
                indexToCheckAgainstPivot--;
            } else
                indexToCheckAgainstPivot++;
            noOfElementAccesses++;
        }
    }

    *pivotIndexToReturn = pivotIndex;
    return noOfElementAccesses;
}

/**
 * Calls the mergesort recursive algorithm without needing the indices to be given as arguments
**/
int mergesort(int array[], int length) {
	return mergesortRecursive(array, 0, length - 1);
}

/**
 * Recursively carries out mergesort on an subarray of an array
**/
int mergesortRecursive(int array[], int begin, int end) {
	if(begin >= end)
		return 0;
	
    int complexity = 0;
    
	int middleIndex = begin + (end - begin) / 2;
	complexity += mergesortRecursive(array, begin, middleIndex);
	complexity += mergesortRecursive(array, middleIndex+1, end);
	complexity += mergesortMerge(array, begin, middleIndex+1, 1+end-begin);
	return complexity;
}

/**
 * Merges two, sorted subarrays so that the result is also sorted. The two subarrays must be consecutive to each other within a greater array
**/
int mergesortMerge(int array[], int subArrayLIndex, int subArrayRIndex, int subArraysLength) {
    int noOfElementAccesses = 0;

    int subArraysCopy[subArraysLength];
    copyIntArray(subArraysCopy, &array[subArrayLIndex], subArraysLength);
    noOfElementAccesses += subArraysLength;

    // The copy's indices will be different to the corresponding indices of the argument array[]
    int copysSubArrayLIndex = 0;
    int copysSubArrayRIndex = subArrayRIndex - subArrayLIndex;
    const int lastIndexOfCopysSubArrayL = copysSubArrayRIndex - 1;
    const int lastIndexOfCopysSubArrayR = subArraysLength - 1;

    int nextIndexToPlaceElement = subArrayLIndex;

    while(nextIndexToPlaceElement < subArrayLIndex + subArraysLength) {
        if(copysSubArrayLIndex > lastIndexOfCopysSubArrayL) {
            array[nextIndexToPlaceElement] = subArraysCopy[copysSubArrayRIndex];
            copysSubArrayRIndex++;
        } else if(copysSubArrayRIndex > lastIndexOfCopysSubArrayR) {
            array[nextIndexToPlaceElement] = subArraysCopy[copysSubArrayLIndex];
            copysSubArrayLIndex++;
        } else {
            if(subArraysCopy[copysSubArrayLIndex] <= subArraysCopy[copysSubArrayRIndex]) {
                array[nextIndexToPlaceElement] = subArraysCopy[copysSubArrayLIndex];
                copysSubArrayLIndex++;
            } else {
                array[nextIndexToPlaceElement] = subArraysCopy[copysSubArrayRIndex];
                copysSubArrayRIndex++;
            }
        }
        nextIndexToPlaceElement++;
        noOfElementAccesses++;
    }

    return noOfElementAccesses++;
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
                swapIntegerValues(&array[i], &array[i+1]);
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

/**
 * Swaps the dereferenced values of the two pointer arguments
**/
void swapIntegerValues(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int* getSmallestInteger(int array[], int length) {
    int* pointerToSmallest = &array[0];
    for(int i = 1; i < length; i++) {
        if(array[i] < *pointerToSmallest)
            pointerToSmallest = &array[i];
    }

    return pointerToSmallest;
}
