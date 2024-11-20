// parallel_sort.h
#ifndef PARALLEL_SORT_H
#define PARALLEL_SORT_H

#include <vector>
#include <algorithm>
#include <omp.h>
#include <climits> // For INT_MAX

// Function declarations for parallel sorting algorithms
void parallelBubbleSort(std::vector<std::vector<int>>& edgeList, int NUMTHREADS);

void parallelQuickSort(std::vector<std::vector<int>>& edgeList, int NUMTHREADS);
void quickSort_parallel_internal(std::vector<std::vector<int>>& edgeList, int left, int right, int cutoff);

void parallelMergeSort(std::vector<std::vector<int>>& edgeList, int NUMTHREADS);
void parallelMerge(std::vector<std::vector<int>>& edgeList, int left, int mid, int right);
void parallelMergeSort_internal(std::vector<std::vector<int>>& edgeList, int left, int right, int cutoff);

void bitonicSortWrapper(std::vector<std::vector<int>>& edgeList, int NUMTHREADS, bool ascending);
void bitonicCompare(std::vector<std::vector<int>>& edgeList, int i, int j, bool ascending);
void bitonicMerge(std::vector<std::vector<int>>& edgeList, int low, int count, bool ascending);
void bitonicSort(std::vector<std::vector<int>>& edgeList, int low, int count, bool ascending, int cutoff);

void Wrapper(std::vector<std::vector<int>>& edgeList, int NUMTHREADS); // Optimized Bitonic
void ascendingSwap(int index1, int index2, std::vector<std::vector<int>>& ar);
void decendingSwap(int index1, int index2, std::vector<std::vector<int>>& ar);
void bitonicSortFromBitonicSequence(int startIndex, int lastIndex, int dir, std::vector<std::vector<int>>& ar);
void bitonicSequenceGenerator(int startIndex, int lastIndex, std::vector<std::vector<int>>& edgeList, int NUMTHREADS);

#endif // PARALLEL_SORT_H
