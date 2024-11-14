// parallel_sort.h
#ifndef PARALLEL_SORT_H
#define PARALLEL_SORT_H

#include <vector>
#include <algorithm>
#include <omp.h>
#include <climits> // For INT_MAX

// Function declarations for parallel sorting algorithms
void parallelBubbleSort(std::vector<std::vector<int>>& edgeList, int NUMTHREADS);
void parallelQuickSort(std::vector<std::vector<int>>& edgeList, int low, int high, int NUMTHREADS);
void parallelMergeSort(std::vector<std::vector<int>>& edgeList, int left, int right, int NUMTHREADS);
void bitonicSortWrapper(std::vector<std::vector<int>>& edgeList, int NUMTHREADS, bool ascending);
void Wrapper(std::vector<std::vector<int>>& edgeList, int NUMTHREADS);

#endif // PARALLEL_SORT_H
