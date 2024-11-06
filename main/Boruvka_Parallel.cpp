#include <bits/stdc++.h>
#include <omp.h>

using namespace std;

int NUM_THREADS = 2;

class Graph {
    int V; // Number of vertices
    vector<vector<int>> graph; // List of edges {u, v, w}

    // Optimized find function with path compression in a critical section
    int findParent(vector<int>& parent, int i) {
        if (parent[i] != i) {
            #pragma omp critical
            {
                if (parent[i] != i) // Double-check after entering critical section
                    parent[i] = findParent(parent, parent[i]);
            }
        }
        return parent[i];
    }

    // Union by rank with atomic operations for thread safety
    void unionSet(vector<int>& parent, vector<int>& rank, int x, int y) {
        int x_parent = findParent(parent, x);
        int y_parent = findParent(parent, y);

        if (x_parent != y_parent) {
            #pragma omp critical
            {
                // Perform union by rank
                if (rank[x_parent] < rank[y_parent]) {
                    parent[x_parent] = y_parent;
                } else if (rank[x_parent] > rank[y_parent]) {
                    parent[y_parent] = x_parent;
                } else {
                    parent[y_parent] = x_parent;
                    rank[x_parent]++;
                }
            }
        }
    }

public:
    Graph(int vertices) : V(vertices) {}

    // Add edge to the graph
    void addEdge(int u, int v, int w) {
        graph.push_back({u, v, w});
    }

    // Main Boruvka's MST Algorithm
    void BoruvkaMST() {
        vector<int> parent(V), rank(V, 0);
        vector<vector<int>> cheapest(V, vector<int>(3, -1));
        int MSTweight = 0, numTrees = V;

        // Initialize parent array
        #pragma omp parallel for num_threads(NUM_THREADS)
        for (int i = 0; i < V; i++) parent[i] = i;

        while (numTrees > 1) {
            // Initialize cheapest array for each tree
            fill(cheapest.begin(), cheapest.end(), vector<int>{-1, -1, INT_MAX});

            // Find the cheapest edge for each component
            #pragma omp parallel for num_threads(NUM_THREADS)
            for (int i = 0; i < graph.size(); i++) {
                int u = graph[i][0], v = graph[i][1], w = graph[i][2];
                int set_u = findParent(parent, u);
                int set_v = findParent(parent, v);

                if (set_u != set_v) {
                    #pragma omp critical
                    {
                        if (w < cheapest[set_u][2]) cheapest[set_u] = {u, v, w};
                        if (w < cheapest[set_v][2]) cheapest[set_v] = {u, v, w};
                    }
                }
            }

            // Add the selected cheapest edges to MST
            #pragma omp parallel for reduction(+:MSTweight) num_threads(NUM_THREADS)
            for (int i = 0; i < V; i++) {
                if (cheapest[i][2] != INT_MAX) {
                    int u = cheapest[i][0], v = cheapest[i][1], w = cheapest[i][2];
                    int set_u = findParent(parent, u), set_v = findParent(parent, v);

                    if (set_u != set_v) {
                        MSTweight += w;
                        unionSet(parent, rank, set_u, set_v);
                        #pragma omp atomic
                        numTrees--;
                    }
                }
            }
        }

        // Output MST weight
        cout << "Weight of MST is " << MSTweight << endl;
    }
};

int main(int argc, char* argv[]) {
    // Set NUM_THREADS from command-line argument, or detect based on available cores
    if (argc > 1) {
        NUM_THREADS = stoi(argv[1]);
    } else {
        NUM_THREADS = omp_get_max_threads();
    }

    // Example graph for testing (can replace with larger inputs)
    Graph g(4);
    g.addEdge(0, 1, 10);
    g.addEdge(0, 2, 6);
    g.addEdge(0, 3, 5);
    g.addEdge(1, 3, 15);
    g.addEdge(2, 3, 4);

    g.BoruvkaMST();
    return 0;
}
