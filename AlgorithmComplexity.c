#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define TRUE 1
#define FALSE 0
#define MAX_THREADS 4

#define toSortLength 50000

typedef struct {
    int* array;
    int length;
} IntegerArray;

typedef struct {
    IntegerArray arr;
    int* arrayOfIndicesAccessed;
    int threadNo;
} LockingHelper;

unsigned long selectionsort(int array[], int length);
unsigned long insertionsort(int array[], int length);

unsigned long bubblesortThreadHelper(IntegerArray a);
void* bubblesort(LockingHelper a);

unsigned long quicksort(int array[], int length);
unsigned long quicksortRecursiveHelper(int array[], int front, int rear);
unsigned long partition(int array[], int front, int rear, int* pivotIndexToReturn);

unsigned long mergesortMerge(int array[], int subArrayLIndex, int subArrayRIndex, int subArraysLength);
unsigned long mergesort(int array[], int length);
unsigned long mergesortRecursive(int array[], int begin, int end);

void copyIntArray(int copyDestination[], int copySource[], int copySourceLength);
void fillIntArrayRandomly(int array[], int length, int maxInteger);
void swapIntegerValues(int* a, int* b);
int* getSmallestInteger(int array[], int length);
int isSorted(int array[], int length);;

void printIntArray(int array[], int length) {
	for(int i = 0; i < length - 1; i++)
		printf("%d, ", array[i]);
	printf("%d\n", array[length - 1]);
}

int main() {
	int toSort[toSortLength];
	fillIntArrayRandomly(toSort, toSortLength, 1000);
    IntegerArray arr = {&toSort[0], toSortLength};
	//printIntArray(toSort, toSortLength);
	printf("Complexity:\t%lu\n", bubblesortThreadHelper(arr));
	//printIntArray(toSort, toSortLength);

	printf("Is sorted?\t%d\n", isSorted(arr.array, arr.length));
	return EXIT_SUCCESS;
}

int isSorted(int array[], int length) {
	for(int i = 0; i < length - 1; i++) {
		if(array[i] > array[i+1])
			return FALSE;
	}
	return TRUE;
}

unsigned long selectionsort(int array[], int length) {
    unsigned long complexity = 0;

    for(int i = 0; i < length; i++) {
        swapIntegerValues(getSmallestInteger(&array[i], length-i), &array[i]);
        complexity += length - i;
    }

    return complexity;
}

unsigned long insertionsort(int array[], int length) {
    unsigned long noOfComparisons = 0;
    
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
unsigned long quicksort(int array[], int length) {
    return quicksortRecursiveHelper(array, 0, length-1);
}

/**
 * Carries out quicksort on a subarray recursively
**/
unsigned long quicksortRecursiveHelper(int array[], int front, int rear) {
    if(front >= rear)
        return 0;

    unsigned long complexity = 0;
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
unsigned long partition(int array[], int front, int rear, int* pivotIndexToReturn) {
    int pivotIndex = front;
    int indexToCheckAgainstPivot = rear;
    unsigned long noOfElementAccesses = 0;

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
unsigned long mergesort(int array[], int length) {
	return mergesortRecursive(array, 0, length - 1);
}

/**
 * Recursively carries out mergesort on an subarray of an array
**/
unsigned long mergesortRecursive(int array[], int begin, int end) {
	if(begin >= end)
		return 0;
	
    unsigned long complexity = 0;
    
	int middleIndex = begin + (end - begin) / 2;
	complexity += mergesortRecursive(array, begin, middleIndex);
	complexity += mergesortRecursive(array, middleIndex+1, end);
	complexity += mergesortMerge(array, begin, middleIndex+1, 1+end-begin);
	return complexity;
}

/**
 * Merges two, sorted subarrays so that the result is also sorted. The two subarrays must be consecutive to each other within a greater array
**/
unsigned long mergesortMerge(int array[], int subArrayLIndex, int subArrayRIndex, int subArraysLength) {
    unsigned long noOfElementAccesses = 0;

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

unsigned long bubblesortThreadHelper(IntegerArray a) {
    LockingHelper helpers[MAX_THREADS];
    int arrayOfIndicesAccessed[MAX_THREADS];
    for(int i = 0; i < MAX_THREADS; i++)
        helpers[i] = (LockingHelper) {a, arrayOfIndicesAccessed, i};

    pthread_t threadIDs[MAX_THREADS];
    for(int i = 0; i < MAX_THREADS; i++)
        pthread_create(&threadIDs[i], NULL, (void* (*)(void*)) bubblesort, (void*) &helpers[i]);
    
    void* threadComplexities[MAX_THREADS];
    for(int i = 0; i < MAX_THREADS; i++)
        pthread_join(threadIDs[i], &threadComplexities[i]);


    unsigned long totalComplexity = 0;
    for(int i = 0; i < MAX_THREADS; i++) {
        totalComplexity += *((unsigned long*) threadComplexities[i]);
        free(threadComplexities[i]);
    }
    return totalComplexity;
}

/**
 * Bubblesorts an integer array. Returns the number of element comparisons made
**/
void* bubblesort(LockingHelper helper) {
    IntegerArray* a = helper.arr;
    int* i = &helper.arrayOfIndicesAccessed[helper.threadNo];

	int isSorted = FALSE;
	unsigned long noOfComparisons = 0;
	
	int temp;
	while(!isSorted) {
		isSorted = TRUE;
		for(int i = 0; i < a.length - 1; i++) {
			if(a.array[i] > a.array[i + 1]) {
                swapIntegerValues(&a.array[i], &a.array[i+1]);
				isSorted = FALSE;
			}
			noOfComparisons++;
		}
	}

    unsigned long* toReturn = malloc(sizeof(unsigned long));
    *toReturn = noOfComparisons;
	return (void*) toReturn;
}

/**
 * Fills an integer array with random elements using the current time as its seed
 * maxInteger specifies the largest possible integer that you want in your array.
 * When this argument is zero or negative, rand()'s default will be used
**/
void fillIntArrayRandomly(int array[], int length, int maxInteger) {
	int randomSeed = time(NULL);
	srand(randomSeed);
    if(maxInteger <= 0) {
	    for(int i = 0; i < length; i++)
		    array[i] = rand();
    } else {
        for(int i = 0; i < length; i++)
		    array[i] = rand() % (maxInteger + 1);
    }
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
