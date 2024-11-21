#include <kruskals_parallel.h>

UnionSet_p :: UnionSet_p(int n) {
    parent = new int[n];
    rank = new int[n];

    #pragma omp parallel for
    for(int i = 0; i < n; i ++) {
        // set the parent to itself and rank to 1
        parent[i] = i;
        rank[i] = 1;
    }
}

int UnionSet_p :: findParent(int node) {
    if(parent[node] == node)
        return node;

    return parent[node] = findParent(parent[node]);
}

void UnionSet_p :: Union(int& u, int& v) {
    int u_parent = findParent(u);
    int v_parent = findParent(v);

    if(u_parent == v_parent) {
        return;
    }   

    int u_rank = rank[u_parent];
    int v_rank = rank[v_parent];

    if(u_rank > v_rank) {
        parent[v_parent] = u_parent;
    }
    else if(u_rank < v_rank) {
        parent[u_parent] = v_parent;
    }
    else {
        parent[v_parent] = u_parent;
        rank[u_parent] += 1;
    }

    return;
}

KruskalsGraph_p :: KruskalsGraph_p(int V, int s, int n) {
    NUMTHREADS = n;
    selection = s;
    Vertices = V;
    Parent = vector<vector<int>> ();
    edgeList = vector<vector<int>> ();
}

void KruskalsGraph_p :: EnterEdges(int& u, int& v, int& w) {
    // Store the data into adjacency list
    edgeList.push_back({w, u, v});

    return;
}

void KruskalsGraph_p :: SortList() {
    // Select the parallel algorithm to sort the edge list
    switch(selection) {
        case 1 : {
                    // Sort in parallel using gnu parallel impl. (-D_GLIBCXX_PARALLEL)
                    // Set the number of threads
                    omp_set_num_threads(NUMTHREADS);
                    __gnu_parallel :: sort(edgeList.begin(), edgeList.end());
                    break;
                }
        case 2 : parallelBubbleSort(edgeList, NUMTHREADS);
                 break;

        case 3 : parallelQuickSort(edgeList, NUMTHREADS);
                 break;

        case 4 : parallelMergeSort(edgeList, NUMTHREADS);
                 break;
                 
        case 5 : bitonicSortWrapper(edgeList, NUMTHREADS, 1);
                 break;
        
        case 6 : Wrapper(edgeList, NUMTHREADS);
                 break;

        default : printf("Enter correct selection for sorting algorithm!\n");
                  exit(0);
    }

    return;
}

void KruskalsGraph_p :: KruskalMST() {
    SortList();
    // for(auto e: edgeList) cout<<e[0]<<endl;
    UnionSet_p* DSU = new UnionSet_p(Vertices);
    int MSTweight = 0;

    for(auto& edge : edgeList) {
        int w = edge[0], u = edge[1], v = edge[2];
        int u_parent = DSU -> findParent(u);
        int v_parent = DSU -> findParent(v);

        if(u_parent != v_parent) {
            DSU -> Union(u, v);
            MSTweight += w;
            Parent.push_back({u, v, w});
        }
    }
    printf("Weight of MST is %d\n", MSTweight);
    return;
}

void KruskalsGraph_p :: PrintKruskalMST() {
    printf("Edge \tWeight\n");
    for(auto& p : Parent){
        printf("%d - %d \t%d\n", p[0], p[1], p[2]);
    }
}
