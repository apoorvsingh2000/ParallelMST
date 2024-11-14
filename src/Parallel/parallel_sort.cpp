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
        //#pragma omp task shared(edgeList) if(mid > 1024)
        bitonicMerge(edgeList, low, mid, ascending);

        //#pragma omp task shared(edgeList) if(mid > 1024)
        bitonicMerge(edgeList, low + mid, mid, ascending);

        //#pragma omp taskwait
    }
}

// Recursive function for Bitonic Sort
void bitonicSort(std::vector<std::vector<int>>& edgeList, int low, int count, bool ascending) {
    if (count > 1) {
        int mid = count / 2;

        // Sort first half in ascending order 
        // #pragma omp task shared(edgeList) if(mid > 16384) // Cap at 2^14
        bitonicSort(edgeList, low, mid, true);

        // Sort second half in descending order
        // #pragma omp task shared(edgeList) if((mid) > 16384) // Cap at 2^14
        bitonicSort(edgeList, low + mid, mid, false);
        

        // #pragma omp taskwait // Wait for both halves to finish

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

// =====================================
// Optimized code for parallel BITONIC
// =====================================
void ascendingSwap(int index1, int index2, std::vector<std::vector<int>>& ar) // Swap two values such that they appear in ascending order in the array
{
    if (ar[index2][0] < ar[index1][0])
    {
        auto temp = ar[index2];
        ar[index2] = ar[index1];
        ar[index1] = temp;
    }
}
void decendingSwap(int index1, int index2, std::vector<std::vector<int>>& ar) // Swap two values such that they appear in decending order in the array
{
    if (ar[index1][0] < ar[index2][0])
    {
        auto temp = ar[index2];
        ar[index2] = ar[index1];
        ar[index1] = temp;
    }
}
void bitonicSortFromBitonicSequence(int startIndex, int lastIndex, int dir, std::vector<std::vector<int>>& ar) // Form a increaseing or decreasing array when a bitonic input is given to the function
{
    if (dir == 1)
    {
        int counter = 0; // Counter to keep track of already swapped elements ,, parallelising this area results in poor performance due to overhead ,,need to fix
        int noOfElements = lastIndex - startIndex + 1;
        for (int j = noOfElements / 2; j > 0; j = j / 2)
        {
            counter = 0;
            for (int i = startIndex; i + j <= lastIndex; i++)
            {
                if (counter < j)
                {
                    ascendingSwap(i, i + j, ar);
                    counter++;
                }
                else
                {
                    counter = 0;
                    i = i + j - 1;
                }
            }
        }
    }
    else // Descending sort
    {
        int counter = 0;
        int noOfElements = lastIndex - startIndex + 1;
        for (int j = noOfElements / 2; j > 0; j = j / 2)
        {
            counter = 0;
            for (int i = startIndex; i <= (lastIndex - j); i++)
            {
                if (counter < j)
                {
                    decendingSwap(i, i + j, ar);
                    counter++;
                }
                else
                {
                    counter = 0;
                    i = i + j - 1;
                }
            }
        }
    }
}
void bitonicSequenceGenerator(int startIndex, int lastIndex, std::vector<std::vector<int>>& edgeList, int NUMTHREADS) // Generate a bitonic sequence from a random order
{
    int noOfElements = lastIndex - startIndex + 1;
    for (int j = 2; j <= noOfElements; j = j * 2)
    {
        #pragma omp parallel for num_threads(NUMTHREADS) //parallel implementation results in most performance gains here
        for (int i = 0; i < noOfElements; i = i + j)
        {
            if (((i / j) % 2) == 0)
            {
                bitonicSortFromBitonicSequence(i, i + j - 1, 1, edgeList);
            }
            else
            {
                bitonicSortFromBitonicSequence(i, i + j - 1, 0, edgeList);
            }
        }
    }
}

void Wrapper(std::vector<std::vector<int>>& edgeList, int NUMTHREADS) {
    int n = edgeList.size();

    // Ensure the size is a power of 2 by padding with large weights
    int powerOfTwo = 1;
    while (powerOfTwo < n) {
        powerOfTwo *= 2;
    }
    for (int i = n; i < powerOfTwo; i++) {
        edgeList.push_back({INT_MAX, -1, -1}); // Add dummy edges
    }

    bitonicSequenceGenerator(0, edgeList.size() - 1, edgeList, NUMTHREADS);

    // Remove the dummy edges
    edgeList.resize(n);
}
