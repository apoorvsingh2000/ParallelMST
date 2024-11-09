#include<bits/stdc++.h>

using namespace std;

class Graph {
    // Store all edges
    vector<vector<int>> graph;
    // Parent node for the current
    vector<int> parent;
    // Weight to the node
    vector<int> key;
    // Whether included in MST or not
    vector<bool> MSTset;

    int Vertices;
public:
    Graph(int V){
        graph = vector<vector<int>> (V, vector<int> (V));
        Vertices = V;
        parent = vector<int> (V, -1);
        key = vector<int> (V, INT_MAX);
        MSTset = vector<bool> (V, false);
    }

    void EnterEdges(int&, int&, int&);
    pair<int,int> FindNextMin();
    void PrimMST();
    void PrintMST();
};

void Graph :: EnterEdges(int& u, int& v, int& w) {
    graph[u][v] = w;
    graph[v][u] = w;
    return;
}

pair<int,int> Graph :: FindNextMin() {
    int min = INT_MAX, min_idx;

    for(int v = 0; v < Vertices; v ++) {
        if(MSTset[v] == false && key[v] < min) { 
            min = key[v];
            min_idx = v;
        }
    }

    return {min_idx, min};
}

void Graph :: PrimMST() {
    // our first node to be entered into the MST will always be NODE 0
    key[0] = 0;
    parent[0] = -1;
    int MSTweight = 0;

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

void Graph :: PrintMST() {
    printf("Edge \tWeight\n");
    for(int i = 1; i < Vertices; i ++){
        printf("%d - %d \t%d\n", parent[i], i, graph[i][parent[i]]);
    }
}

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
        printf("./prims_s v edges.txt\n");
        exit(0);
    }

    string ifile = "../";
    int vertices = 0;

    vertices = atoi(argv[1]);
    ifile += argv[2];

    vector<tuple<int,int,int>> edges;
    parseEdges(ifile, edges);

    // Example graph (4 vertices, 5 edges)
    Graph* g = new Graph(vertices);
    // Add edges to graph
    for (const auto& edge : edges) {
        int u, v, w;
        tie(u, v, w) = edge;  // Extract u, v, w from the tuple
        g->EnterEdges(u, v, w);
    }

    // Run Boruvka's MST algorithm
    g->PrimMST();

    g->PrintMST();

    return 0;
}
