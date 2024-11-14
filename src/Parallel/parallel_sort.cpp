// parallel_sort.cpp
#include "parallel_sort.h"


// ========================================
// Parallel Bubble Sort
// ========================================
void parallelBubbleSort(std::vector<std::vector<int>>& edgeList, int NUMTHREADS) {
    int n = edgeList.size();

    for (int phase = 0; phase < n; phase++) {
        if (phase % 2 == 0) {
            // Even phase: Compare and swap pairs (1, 2), (3, 4), ...
            #pragma omp parallel for num_threads(NUMTHREADS)
            for (int i = 1; i < n; i += 2) {
                if (edgeList[i - 1] > edgeList[i]) {
                    std::swap(edgeList[i - 1], edgeList[i]);
                }
            }
        } else {
            // Odd phase: Compare and swap pairs (2, 3), (4, 5), ...
            #pragma omp parallel for num_threads(NUMTHREADS)
            for (int i = 1; i < n - 1; i += 2) {
                if (edgeList[i] > edgeList[i + 1]) {
                    std::swap(edgeList[i], edgeList[i + 1]);
                }
            }
        }
    }
}

// ========================================
// Parallel Quick Sort
// ========================================
void parallelQuickSort(std::vector<std::vector<int>>& edgeList, int low, int high, int NUMTHREADS) {
    if (low < high) {
        int pivot = edgeList[high][0]; // Choose pivot
        int i = low - 1;
        for (int j = low; j < high; j++) {
            if (edgeList[j][0] < pivot) {
                i++;
                std::swap(edgeList[i], edgeList[j]);
            }
        }
        std::swap(edgeList[i + 1], edgeList[high]);
        int pi = i + 1;

        #pragma omp parallel sections num_threads(NUMTHREADS)
        {
            #pragma omp section
            parallelQuickSort(edgeList, low, pi - 1, NUMTHREADS); // Sort left half
            #pragma omp section
            parallelQuickSort(edgeList, pi + 1, high, NUMTHREADS); // Sort right half
        }
    }
}

// ========================================
// Parallel Merge Sort
// ========================================
void parallelMerge(std::vector<std::vector<int>>& edgeList, int left, int mid, int right) {
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

void parallelMergeSort(std::vector<std::vector<int>>& edgeList, int left, int right, int NUMTHREADS) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        #pragma omp parallel sections num_threads(NUMTHREADS)
        {
            #pragma omp section
            parallelMergeSort(edgeList, left, mid, NUMTHREADS);
            #pragma omp section
            parallelMergeSort(edgeList, mid + 1, right, NUMTHREADS);
        }

        parallelMerge(edgeList, left, mid, right);
    }
}

// ========================================
// Parallel Bitonic Sort
// ========================================
// Function to compare and swap edges based on weights
void bitonicCompare(std::vector<std::vector<int>>& edgeList, int i, int j, bool ascending) {
    if (ascending == (edgeList[i][0] > edgeList[j][0])) {
        std::swap(edgeList[i], edgeList[j]);
    }
}

// Recursive function for Bitonic Merge
void bitonicMerge(std::vector<std::vector<int>>& edgeList, int low, int count, bool ascending) {
    if (count > 1) {
        int mid = count / 2;

        // Compare and swap pairs
        #pragma omp parallel for
        for (int i = low; i < low + mid; i++) {
            bitonicCompare(edgeList, i, i + mid, ascending);
        }

        // Recursively merge the two halves
        bitonicMerge(edgeList, low, mid, ascending);
        bitonicMerge(edgeList, low + mid, mid, ascending);
    }
}

// Recursive function for Bitonic Sort
void bitonicSort(std::vector<std::vector<int>>& edgeList, int low, int count, bool ascending) {
    if (count > 1) {
        int mid = count / 2;

        // Sort first half in ascending order
        #pragma omp task shared(edgeList)
        bitonicSort(edgeList, low, mid, true);

        // Sort second half in descending order
        #pragma omp task shared(edgeList)
        bitonicSort(edgeList, low + mid, mid, false);

        #pragma omp taskwait // Wait for both halves to finish

        // Merge the two halves
        bitonicMerge(edgeList, low, count, ascending);
    }
}

// Wrapper function for Bitonic Sort
void bitonicSortWrapper(std::vector<std::vector<int>>& edgeList, int NUMTHREADS, bool ascending) {
    int n = edgeList.size();

    // Ensure the size is a power of 2 by padding with large weights
    int powerOfTwo = 1;
    while (powerOfTwo < n) {
        powerOfTwo *= 2;
    }
    for (int i = n; i < powerOfTwo; i++) {
        edgeList.push_back({INT_MAX, -1, -1}); // Add dummy edges
    }

    // Start the parallel region
    #pragma omp parallel num_threads(NUMTHREADS)
    {
        #pragma omp single
        bitonicSort(edgeList, 0, edgeList.size(), ascending);
    }

    // Remove the dummy edges
    edgeList.resize(n);
}
