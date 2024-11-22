// parallel_sort.h
#ifndef SEQUENTIAL_SORT_H
#define SEQUENTIAL_SORT_H

#include <vector>
#include <algorithm>
#include <climits> // For INT_MAX

// Function declarations for parallel sorting algorithms
void SequentialBubbleSort(std::vector<std::vector<int>>& edgeList);
void SequentialQuickSort(std::vector<std::vector<int>>& edgeList, int low, int high);
void SequentialMergeSort(std::vector<std::vector<int>>& edgeList, int left, int right);
void SequentialBitonicSortWrapper(std::vector<std::vector<int>>& edgeList, bool ascending);

#endif // SEQUENTIAL_SORT_H
