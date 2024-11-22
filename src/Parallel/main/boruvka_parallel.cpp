#include <boruvka_parallel.h>


// Function to find the parent node of the given node of the graph
// Not using path compression as that leads to race cases when parallelized
int BoruvkaGraph_p :: findParent(vector<int>& parent, int i) {
    if (i != parent[i]) {
        int root = findParent(parent, parent[i]); // Recursively find the root
        #pragma omp atomic write
        parent[i] = root; // Path compression: Point i directly to the root
    }
    return parent[i];
}

// Function to join two disjoint sets, does so by the context of ranks
void BoruvkaGraph_p :: unionSet(vector<int>& parent, vector<int>& rank, int& x_parent, int& y_parent) {
    //int x_parent = findParent(parent, x);
    //int y_parent = findParent(parent, y);
    int x_rank = rank[x_parent];
    int y_rank = rank[y_parent];

    if (x_rank < y_rank) {
        // #pragma omp atomic write
        parent[x_parent] = y_parent; // atomic operation
    } 
    else if (y_rank < x_rank) {
        // #pragma omp atomic write
        parent[y_parent] = x_parent; // atomic operation
    } 
    else {
        // #pragma omp atomic write
        parent[y_parent] = x_parent; // atomic operation

        // #pragma omp atomic
        rank[x_parent] += 1; // atomic operation
    }
}

BoruvkaGraph_p :: BoruvkaGraph_p(int vertices, int n) {
    NUMTHREADS = n;
    V = vertices;

    graph = vector<vector<int>> ();
    Parent = vector<vector<int>> ();
}

// Add edges to the adj. matrix
void BoruvkaGraph_p :: EnterEdges(int u, int v, int w) {
    graph.push_back({u, v, w});
}

// Boruvka's Algorithm MAIN
void BoruvkaGraph_p :: BoruvkaMST() {
    // Initialize parent and rank vectors
    vector<int> parent(V);
    vector<int> rank(V, 0);

    // Parallel initialization of parent
    #pragma omp parallel for num_threads(NUMTHREADS)
    for (int i = 0; i < V; i++) {
        parent[i] = i;
    }

    // Declare variables to store the current number of trees present and the MST weight
    int MSTweight = 0;
    int numTrees = V;

    // Declare a dictionary to store the cheapest edge, in the form {u, v, w}
    // initialized to -1
    vector<vector<int>> cheapest(V, vector<int>(3, -1));

    // Create and initialize the lock
    omp_lock_t myLock[V];

    #pragma omp parallel for num_threads(NUMTHREADS)
    for (int i = 0; i < V; i++) {
        omp_init_lock(&myLock[i]);
    }

    // Loop until the number of trees becomes 1
    while (numTrees > 1) {
        // printf("trees: %d\n", numTrees);
        // Traverse through the edges in the graph and assign the lowest weights to all parent nodes.
        int size = graph.size();

        #pragma omp parallel for num_threads(NUMTHREADS)
        for (int i = 0; i < size; i++) {
            // Unzip the values of u, v, w
            int u = graph[i][0], v = graph[i][1], w = graph[i][2];

            int parent_u = findParent(parent, u);
            int parent_v = findParent(parent, v);

            // Check if the parents of the two trees are the same
            if (parent_u != parent_v) {
                // Update the cheapest edge if a smaller weight is found
                vector<int> entry = {u, v, w};

                omp_set_lock(&myLock[parent_u]); // Works faster than omp critical
                // #pragma omp critical
                if (cheapest[parent_u][2] == -1 || cheapest[parent_u][2] > w) {
                    cheapest[parent_u] = entry;
                }
                omp_unset_lock(&myLock[parent_u]);

                omp_set_lock(&myLock[parent_v]); // Works faster than omp critical
                // #pragma omp critical
                if (cheapest[parent_v][2] == -1 || cheapest[parent_v][2] > w) {
                    cheapest[parent_v] = entry;
                }
                omp_unset_lock(&myLock[parent_v]);
            }
        }
   
        // Once the cheapest edges are found, join them and update corresponding rank values
        for (int node = 0; node < V; node++) {
            // If the cheapest weight has been found
            if (cheapest[node][2] != -1) {
                // Unzip the values of u, v, w
                int u = cheapest[node][0], v = cheapest[node][1], w = cheapest[node][2];
                int parent_u, parent_v;

                // Critical section for finding and merging trees
                // Find parent of u and v
                parent_u = findParent(parent, u);
                parent_v = findParent(parent, v);

                // Merge trees only if they have different parents
                if (parent_u != parent_v) {
                    // Update the MST weight
                    MSTweight += w;

                    // Merge the two trees into one
                    unionSet(parent, rank, parent_u, parent_v);

                    // Add edge to final list
                    Parent.push_back({u, v, w});

                    // Decrement the number of trees
                    numTrees -= 1;
                }
            }
        }

        // Reset the cheapest vector after merging trees
        fill(cheapest.begin(), cheapest.end(), vector<int>(3, -1));
    }

    // Print the final weight of the MST
    printf("Weight of MST is %d\n", MSTweight);
}

void BoruvkaGraph_p :: PrintBoruvkaMST() {
    printf("Edge \tWeight\n");
    for(auto& p : Parent){
        printf("%d - %d \t%d\n", p[0], p[1], p[2]);
    }
}
