// parallel_sort.cpp
#include "parallel_sort.h"


// ========================================
// Parallel Bubble Sort
// ========================================
void parallelBubbleSort(std::vector<std::vector<int>>& edgeList, int NUMTHREADS) {
    int n = edgeList.size();

    #pragma omp parallel num_threads(NUMTHREADS)
    {
        for(int i = 0; i < n; i ++) {
            int first = i % 2;

            #pragma omp for
            for(int j = first; j < n - 1; j += 2) {
                if(edgeList[j][0] > edgeList[j + 1][0]) {
                    std :: swap(edgeList[j], edgeList[j + 1]);
                }
            }
        }
    }
}

// ========================================
// Parallel Quick Sort
// ========================================
void parallelQuickSort(std::vector<std::vector<int>>& edgeList, int NUMTHREADS) {
    int n = edgeList.size();
    int scalingFactor = 100;

    int baseCutoff = n / scalingFactor;

    int cutoff = std::max(1000, std::min(baseCutoff / NUMTHREADS, 10000));

    #pragma omp parallel num_threads(NUMTHREADS)
    {
        #pragma omp single nowait
        {
            quickSort_parallel_internal(edgeList, 0, edgeList.size() - 1, cutoff);
        }
    }
}

void quickSort_parallel_internal(std::vector<std::vector<int>>& edgeList, int left, int right, int cutoff) {
    if (left >= right) return; // Base case for recursion

    int i = left, j = right;
    std::vector<int> pivot = edgeList[left + (right - left) / 2];

    // Partitioning
    while (i <= j) {
        while (i <= right && edgeList[i][0] < pivot[0]) i++;
        while (j >= left && edgeList[j][0] > pivot[0]) j--;
        if (i <= j) {
            std::swap(edgeList[i], edgeList[j]);
            i++;
            j--;
        }
    }

    // Sequential recursion for small partitions
    if ((right - left) < cutoff) {
        if (left < j)
            quickSort_parallel_internal(edgeList, left, j, cutoff);
        if (i < right)
            quickSort_parallel_internal(edgeList, i, right, cutoff);
    } else {
        // Parallel tasks for larger partitions
        #pragma omp task shared(edgeList)
        quickSort_parallel_internal(edgeList, left, j, cutoff);

        #pragma omp task shared(edgeList)
        quickSort_parallel_internal(edgeList, i, right, cutoff);

        #pragma omp taskwait
    }
}


// ========================================
// Parallel Merge Sort
// ========================================
void parallelMerge(std::vector<std::vector<int>>& edgeList, int left, int mid, int right) {
    std::vector<std::vector<int>> temp(right - left + 1); // Temporary buffer
    int i = left, j = mid + 1, k = 0;

    // Merge two sorted halves into the temporary array
    while (i <= mid && j <= right) {
        if (edgeList[i][0] <= edgeList[j][0]) { // Compare based on edgeList[i][0]
            temp[k++] = edgeList[i++];
        } else {
            temp[k++] = edgeList[j++];
        }
    }

    // Copy remaining elements from both halves
    while (i <= mid) temp[k++] = edgeList[i++];
    while (j <= right) temp[k++] = edgeList[j++];

    // Copy back from temporary array to original array
    std::copy(temp.begin(), temp.end(), edgeList.begin() + left);
}

void parallelMergeSort_internal(std::vector<std::vector<int>>& edgeList, int left, int right, int cutoff) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        if ((right - left) > cutoff) {
            // Create tasks for parallel execution of subproblems
            #pragma omp task shared(edgeList)
            parallelMergeSort_internal(edgeList, left, mid, cutoff);

            #pragma omp task shared(edgeList)
            parallelMergeSort_internal(edgeList, mid + 1, right, cutoff);

            #pragma omp taskwait // Wait for both tasks to complete
        } else {
            // Fallback to sequential merge sort for small partitions
            parallelMergeSort_internal(edgeList, left, mid, cutoff);
            parallelMergeSort_internal(edgeList, mid + 1, right, cutoff);
        }

        // Merge the two halves
        parallelMerge(edgeList, left, mid, right);
    }
}

void parallelMergeSort(std::vector<std::vector<int>>& edgeList, int NUMTHREADS) {
    int n = edgeList.size();              // Input size
    int scalingFactor = 100;              // Tunable scaling factor

    // Dynamically calculate base cutoff based on input size
    int baseCutoff = n / scalingFactor;

    // Adjust base cutoff based on number of threads
    int cutoff = std::max(1000, std::min(baseCutoff / NUMTHREADS, 10000));

    #pragma omp parallel num_threads(NUMTHREADS)
    {
        #pragma omp single nowait
        {
            parallelMergeSort_internal(edgeList, 0, edgeList.size() - 1, cutoff);
        }
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
void bitonicSort(std::vector<std::vector<int>>& edgeList, int low, int count, bool ascending, int cutoff) {
    if (count > 1) {
        int mid = count / 2;

        if(count > cutoff) {
            // Sort first half in ascending order 
            #pragma omp task shared(edgeList) 
            bitonicSort(edgeList, low, mid, true, cutoff);

            // Sort second half in descending order
            #pragma omp task shared(edgeList)
            bitonicSort(edgeList, low + mid, mid, false, cutoff);
            
            #pragma omp taskwait // Wait for both halves to finish
        }
        else {
            bitonicSort(edgeList, low, mid, true, cutoff);
            bitonicSort(edgeList, low + mid, mid, false, cutoff);
        }

        // Merge the two halves
        bitonicMerge(edgeList, low, count, ascending);
    }
}

// Wrapper function for Bitonic Sort
void bitonicSortWrapper(std::vector<std::vector<int>>& edgeList, int NUMTHREADS, bool ascending) {
    int n = edgeList.size();              // Input size
    int scalingFactor = 100;              // Tunable scaling factor

    // Dynamically calculate base cutoff based on input size
    int baseCutoff = n / scalingFactor;

    // Adjust base cutoff based on number of threads
    int cutoff = std::max(1000, std::min(baseCutoff / NUMTHREADS, 10000));

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
        bitonicSort(edgeList, 0, edgeList.size(), ascending, cutoff);
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
