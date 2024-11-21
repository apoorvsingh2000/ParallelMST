// sequential_sort.cpp
#include "sequential_sort.h"


// ========================================
// Bubble Sort
// ========================================
void SequentialBubbleSort(std::vector<std::vector<int>>& edgeList) {
    int n = edgeList.size();

    for (int phase = 0; phase < n; phase++) {
        if (phase % 2 == 0) {
            // Even phase: Compare and swap pairs (1, 2), (3, 4), ...
            for (int i = 1; i < n; i += 2) {
                if (edgeList[i - 1] > edgeList[i]) {
                    std::swap(edgeList[i - 1], edgeList[i]);
                }
            }
        } else {
            // Odd phase: Compare and swap pairs (2, 3), (4, 5), ...
            for (int i = 1; i < n - 1; i += 2) {
                if (edgeList[i] > edgeList[i + 1]) {
                    std::swap(edgeList[i], edgeList[i + 1]);
                }
            }
        }
    }
}


// ========================================
// Quick Sort
// ========================================
void SequentialQuickSort(std::vector<std::vector<int>>& edgeList, int low, int high) {
    if (low < high) {
        int pivot = edgeList[low + (high - low) / 2][0]; // Choose pivot
        int i = low - 1;
        for (int j = low; j < high; j++) {
            if (edgeList[j][0] < pivot) {
                i++;
                std::swap(edgeList[i], edgeList[j]);
            }
        }
        std::swap(edgeList[i + 1], edgeList[high]);
        int pi = i + 1;

        // Recursively sort left and right halves
        SequentialQuickSort(edgeList, low, pi - 1);
        SequentialQuickSort(edgeList, pi + 1, high);
    }
}


// ========================================
// Merge Sort
// ========================================
void merge(std::vector<std::vector<int>>& edgeList, int left, int mid, int right) {
    std::vector<std::vector<int>> temp(right - left + 1);
    int i = left, j = mid + 1, k = 0;

    while (i <= mid && j <= right) {
        if (edgeList[i] <= edgeList[j]) {
            temp[k++] = edgeList[i++];
        } else {
            temp[k++] = edgeList[j++];
        }
    }

    while (i <= mid) temp[k++] = edgeList[i++];
    while (j <= right) temp[k++] = edgeList[j++];

    for (int i = left, k = 0; i <= right; i++, k++) {
        edgeList[i] = temp[k];
    }
}

void SequentialMergeSort(std::vector<std::vector<int>>& edgeList, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        // Recursively sort left and right halves
        SequentialMergeSort(edgeList, left, mid);
        SequentialMergeSort(edgeList, mid + 1, right);

        // Merge the sorted halves
        merge(edgeList, left, mid, right);
    }
}


// ========================================
// Bitonic Sort
// ========================================
// Function to compare and swap edges based on weights
void bitonicCompareSequential(std::vector<std::vector<int>>& edgeList, int i, int j, bool ascending) {
    if (ascending == (edgeList[i][0] > edgeList[j][0])) {
        std::swap(edgeList[i], edgeList[j]);
    }
}

void bitonicMergeSequential(std::vector<std::vector<int>>& edgeList, int low, int count, bool ascending) {
    if (count > 1) {
        int mid = count / 2;

        for (int i = low; i < low + mid; i++) {
            bitonicCompareSequential(edgeList, i, i + mid, ascending);
        }

        bitonicMergeSequential(edgeList, low, mid, ascending);
        bitonicMergeSequential(edgeList, low + mid, mid, ascending);
    }
}

void bitonicSortSequential(std::vector<std::vector<int>>& edgeList, int low, int count, bool ascending) {
    if (count > 1) {
        int mid = count / 2;

        // Sort first half in ascending order
        bitonicSortSequential(edgeList, low, mid, true);

        // Sort second half in descending order
        bitonicSortSequential(edgeList, low + mid, mid, false);

        // Merge the two halves
        bitonicMergeSequential(edgeList, low, count, ascending);
    }
}

// Wrapper function for Bitonic Sort Sequential
void SequentialBitonicSortWrapper(std::vector<std::vector<int>>& edgeList, bool ascending) {
    int n = edgeList.size();

    int powerOfTwo = 1;
    while (powerOfTwo < n) {
        powerOfTwo *= 2;
    }
    for (int i = n; i < powerOfTwo; i++) {
        edgeList.push_back({INT_MAX, -1, -1});
    }

    bitonicSortSequential(edgeList, 0, edgeList.size(), ascending);

    edgeList.resize(n); // Remove padding
}
