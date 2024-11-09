#include<bits/stdc++.h>
#include<omp.h>
#include<parallel/algorithm>

using namespace std;

int NUMTHREADS = 1;

class UnionSet {
    int* parent;
    int* rank;

public:
    UnionSet(int n) {
        parent = new int[n];
        rank = new int[n];

        #pragma omp parallel for
        for(int i = 0; i < n; i ++) {
            // set the parent to itself and rank to 1
            parent[i] = i;
            rank[i] = 1;
        }
    }

    int findParent(int);
    void Union(int&, int&);
};

int UnionSet :: findParent(int node) {
    while(node != parent[node]) {
        node = parent[node];
    }

    return node;
}

void UnionSet :: Union(int& u, int& v) {
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

class Graph {
    vector<vector<int>> edgeList;
    vector<vector<int>> Parent;

    int Vertices;

public:
    Graph(int V) {
        Vertices = V;
        Parent = vector<vector<int>> ();
        edgeList = vector<vector<int>> ();
    }

    void EnterEdges(int&, int&, int&);
    void SortList();
    void KruskalMST();
    void PrintMST();
};

void Graph :: EnterEdges(int& u, int& v, int& w) {
    // Store the data into adjacency list
    edgeList.push_back({w, u, v});

    return;
}

void Graph :: SortList() {
    /* TODO: Write parallel sorting codes*/
    // 1: Bubble Sort
    // 2: Quick Sort
    // 3: Merge Sort
    // 4: Bitonic Merge Sort

    // Sort in parallel using gnu parallel impl. (-D_GLIBCXX_PARALLEL)
    __gnu_parallel :: sort(edgeList.begin(), edgeList.end());

    return;
}

void Graph :: KruskalMST() {
    SortList();
    // for(auto e: edgeList) cout<<e[0]<<endl;
    UnionSet* DSU = new UnionSet(Vertices);
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

void Graph :: PrintMST() {
    printf("Edge \tWeight\n");
    for(auto& p : Parent){
        printf("%d - %d \t%d\n", p[0], p[1], p[2]);
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
    if (argc < 4) {
        printf("./kruskal_p t v edges.txt\n");
        exit(0);
    }

    string ifile = "../";
    int vertices = 0;

    NUMTHREADS = atoi(argv[1]);
    vertices = atoi(argv[2]);
    ifile += argv[3];

    vector<tuple<int,int,int>> edges;
    parseEdges(ifile, edges);

    // Example graph (4 vertices, 5 edges)
    Graph* g = new Graph(vertices);
    // Add edges to graph
    for (const auto& edge : edges) {
        int u, v, w;
        tie(u, v, w) = edge;  // Extract u, v, w from the tuple
        g -> EnterEdges(u, v, w);
    }

    // Get the start time
    auto start = std::chrono::high_resolution_clock::now();

    // Run Boruvka's MST algorithm
    g -> KruskalMST();

    // Get the end time
    auto end = std::chrono::high_resolution_clock::now();

    // g -> PrintMST();

    // Calculate the elapsed time in seconds
    std::chrono::duration<double> elapsed = end - start;
    printf("Elapsed time for Kruskal's MST calculation: %lf seconds\n", elapsed.count());

    return 0;
}
