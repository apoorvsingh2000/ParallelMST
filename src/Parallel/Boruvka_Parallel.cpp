#include <bits/stdc++.h>
#include <omp.h>

using namespace std;

int NUM_THREADS = 1;

// Class to represent Graph
class Graph {
    int V; // Number of vertices
    vector<vector<int>> graph; // Adjacency matrix to store weights of graph

    // Function to find the parent node of the given node of the graph
    // Not using path compression as that leads to race cases when parallelized
    int findParent(vector<int>& parent, int i) {
        while (i != parent[i])
            i = parent[i]; // Traverse to the root
        return i;
    }

    // Function to join two disjoint sets, does so by the context of ranks
    void unionSet(vector<int>& parent, vector<int>& rank, int& x_parent, int& y_parent) {
        //int x_parent = findParent(parent, x);
        //int y_parent = findParent(parent, y);
        int x_rank = rank[x_parent];
        int y_rank = rank[y_parent];

        if (x_rank < y_rank) {
            #pragma omp atomic write
            parent[x_parent] = y_parent; // atomic operation
        } 
        else if (y_rank < x_rank) {
            #pragma omp atomic write
            parent[y_parent] = x_parent; // atomic operation
        } 
        else {
            #pragma omp atomic write
            parent[y_parent] = x_parent; // atomic operation

            #pragma omp atomic
            rank[x_parent] += 1; // atomic operation
        }
    }

public:
    Graph(int vertices) {
        V = vertices;
    }

    // Add edges to the adj. matrix
    void addEdge(int u, int v, int w) {
        graph.push_back({u, v, w});
    }

    // Boruvka's Algorithm MAIN
    void BoruvkaMST() {
        // Initialize parent and rank vectors
        vector<int> parent(V);
        vector<int> rank(V, 0);

        // Parallel initialization of parent
        #pragma omp parallel num_threads(NUM_THREADS)
        {
            #pragma omp for
            for (int i = 0; i < V; i++) {
                parent[i] = i;
            }
        }

        // Declare variables to store the current number of trees present and the MST weight
        int MSTweight = 0;
        int numTrees = V;

        // Declare a dictionary to store the cheapest edge, in the form {u, v, w}
        // initialized to -1
        vector<vector<int>> cheapest(V, vector<int>(3, -1));

        // Create and initialize the locks
        omp_lock_t myLock[V], myParent[V], myNode[V];
        #pragma omp parallel num_threads(NUM_THREADS)
        {
            #pragma omp for
            for (int i = 0; i < V; i++) {
                omp_init_lock(&myLock[i]);
                omp_init_lock(&myParent[i]);
                omp_init_lock(&myNode[i]);
            }
        }

        // Loop until the number of trees becomes 1
        while (numTrees > 1) {
            // printf("trees: %d\n", numTrees);
            // Traverse through the edges in the graph and assign the lowest weights to all parent nodes.
            #pragma omp parallel num_threads(NUM_THREADS)
            {
                int size = graph.size();

                #pragma omp for
                for (int i = 0; i < size; i++) {
                    // Unzip the values of u, v, w
                    int u = graph[i][0], v = graph[i][1], w = graph[i][2];

                    int parent_u = findParent(parent, u);
                    int parent_v = findParent(parent, v);

                    // Check if the parents of the two trees are the same
                    if (parent_u != parent_v) {
                        // Update the cheapest edge if a smaller weight is found
                        vector<int> entry = {u, v, w};

                        omp_set_lock(&myLock[parent_u]);
                        if (cheapest[parent_u][2] == -1 || cheapest[parent_u][2] > w) {
                            cheapest[parent_u] = entry;
                        }
                        omp_unset_lock(&myLock[parent_u]);

                        omp_set_lock(&myLock[parent_v]);
                        if (cheapest[parent_v][2] == -1 || cheapest[parent_v][2] > w) {
                            cheapest[parent_v] = entry;
                        }
                        omp_unset_lock(&myLock[parent_v]);
                    }
                }
            }

            // Once the cheapest edges are found, join them and update corresponding rank values
            #pragma omp parallel num_threads(NUM_THREADS)
            {
                #pragma omp for
                for (int node = 0; node < V; node++) {
                    // If the cheapest weight has been found
                    if (cheapest[node][2] != -1) {
                        // Unzip the values of u, v, w
                        int u = cheapest[node][0], v = cheapest[node][1], w = cheapest[node][2];
                        int parent_u, parent_v;

                        // This is a critical section for finding and merging trees
                        // Set locks for finding the parent so that there is no race cases
                        #pragma omp critical
                        {   
                            // printf("locking %d\n", u);
                            omp_set_lock(&myNode[u]);
                            // printf("locked %d\n", u);

                            // printf("locking %d\n", v);
                            omp_set_lock(&myNode[v]);
                            // printf("locked %d\n", u);
                        }

                        // Find parent of u and v
                        parent_u = findParent(parent, u);
                        parent_v = findParent(parent, v);

                        // Set locks for the parent nodes
                        #pragma omp critical
                        if(parent_u != parent_v){
                            omp_set_lock(&myParent[parent_u]);
                            omp_set_lock(&myParent[parent_v]);
                        }

                        // Merge trees only if they have different parents
                        if (parent_u != parent_v) {
                            // Update the MST weight
                            MSTweight += w;

                            // Merge the two trees into one, update parent array
                            unionSet(parent, rank, parent_u, parent_v);

                            // Decrement the number of trees
                            numTrees -= 1;

                            // unlock the set locks for parent id
                            omp_unset_lock(&myParent[parent_v]);
                            omp_unset_lock(&myParent[parent_u]);

                        }

                        // printf("unlocking %d\n",v);
                        omp_unset_lock(&myNode[v]);
                        // printf("unlocking %d\n", u);
                        omp_unset_lock(&myNode[u]);
                    }
                }
            }

            // Reset the cheapest vector after merging trees
            fill(cheapest.begin(), cheapest.end(), vector<int>(3, -1));
        }

        // Print the final weight of the MST
        printf("Weight of MST is %d\n", MSTweight);
    }
};

// Function to parse the input text file and store edges in a vector
void parseEdges(const string& filename, vector<tuple<int, int, int>>& edges) {
    ifstream file(filename);  // Open the file

    if (!file.is_open()) {
        cerr << "Error: Could not open the file!" << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);  // Create a stringstream for the current line
        int u, v, w;
        ss >> u >> v >> w;  // Read u, v, w from the line

        // Add the edge (u, v, w) to the edges vector
        edges.push_back(make_tuple(u, v, w));
    }

    file.close();  // Close the file
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("./boruvka t v edges.txt\n");
        exit(0);
    }

    string ifile = "";
    int vertices = 0;

    NUM_THREADS = atoi(argv[1]);
    vertices = atoi(argv[2]);
    ifile = argv[3];

    vector<tuple<int,int,int>> edges;
    parseEdges(ifile, edges);

    // Example graph (4 vertices, 5 edges)
    Graph* g = new Graph(vertices);

    // Add edges to graph
    for (const auto& edge : edges) {
        int u, v, w;
        tie(u, v, w) = edge;  // Extract u, v, w from the tuple
        g->addEdge(u, v, w);
    }

    // Run Boruvka's MST algorithm
    g->BoruvkaMST();
}
