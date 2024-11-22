#include <prims_parallel.h>

PrimsGraph_p :: PrimsGraph_p(int V, int s, int n){
    NUMTHREADS = n;
    selection = s;
    graph = vector<vector<int>> (V, vector<int> (V));
    Vertices = V;
    parent = vector<int> (V, -1);
    key = vector<int> (V, INT_MAX);
    MSTset = vector<bool> (V, false);
}

void PrimsGraph_p :: EnterEdges(int& u, int& v, int& w) {
    graph[u][v] = w;
    graph[v][u] = w;
    return;
}

pair<int, int> PrimsGraph_p::FindNextMin() {
    int length = key.size();

    int work = ceil((float)length / (float)NUMTHREADS);
    vector<vector<int>> Thread_values(NUMTHREADS);

    #pragma omp parallel num_threads(NUMTHREADS)
    {
        int tid = omp_get_thread_num();
        int start = tid * work;
        int end = min(start + work, length);  // Ensure 'end' does not exceed array bounds
        int priv_min = INT_MAX, priv_idx = -1;

        for (int i = start; i < end; i++) {
            if (priv_min > key[i] && MSTset[i] == false) {
                priv_min = key[i];
                priv_idx = i;
            }
        }
        Thread_values[tid] = {priv_min, priv_idx};
    }


    // Select the parallel algorithm to sort the edge list
    switch(selection) {
        case 1 : {
                    // Sort in parallel using gnu parallel impl. (-D_GLIBCXX_PARALLEL)
                    // Set the number of threads
                    omp_set_num_threads(NUMTHREADS);
                    __gnu_parallel :: sort(Thread_values.begin(), Thread_values.end());
                    break;
                }

        case 2 : parallelBubbleSort(Thread_values, NUMTHREADS);                 // Parallel Bubble sort
                 break;

        case 3 : parallelQuickSort(Thread_values, NUMTHREADS);               // Parallel Quick Sort
                 break;

        case 4 : parallelMergeSort(Thread_values, NUMTHREADS);               // Parallel Merge Sort
                 break;

        case 5 : bitonicSortWrapper(Thread_values, NUMTHREADS, 1);              // Naive Parallel Bitonic sort
                 break;

        case 6 : Wrapper(Thread_values, NUMTHREADS);                            // Optimized Bitonic Sorting
                 break;

        default : printf("Enter correct selection for sorting algorithm!\n");
                  exit(0);
    }

    // First element will contain the minimum value and index
    return {Thread_values[0][1], Thread_values[0][0]};
}

void PrimsGraph_p :: PrimMST() {
    // our first node to be entered into the MST will always be NODE 0
    key[0] = 0;
    parent[0] = -1;
    int MSTweight = 0;

    // *** Cannot parallelize the outer for loop ***
    for(int count = 0; count < Vertices; count ++) {
        // pick the minimum key from the key array as the next entry
        pair<int,int> p = FindNextMin();
        // Since we have entered this node into MST, mark as true
        int u = p.first;
        int w = p.second;

        MSTset[u] = true;
        MSTweight += w;

        // Update the adjacent weights for the nodes after u and also 
        // The parent values for them

        #pragma omp parallel num_threads(NUMTHREADS)
        #pragma omp for
        for(int v = 0; v < Vertices; v ++) {
            // check if there is an egde from u to v 
            // v should not be already included in the MST
            // the updated weight should be lesser than the current weight
            if(graph[u][v] && MSTset[v] == false && graph[u][v] < key[v]) {
                parent[v] = u;
                key[v] = graph[u][v];
            }
        }
    }
    printf("Weight of MST is %d\n", MSTweight);
    return;
}

void PrimsGraph_p :: PrintPrimsMST() {
    printf("Edge \tWeight\n");
    for(int i = 1; i < Vertices; i ++){
        printf("%d - %d \t%d\n", parent[i], i, graph[i][parent[i]]);
    }
}
