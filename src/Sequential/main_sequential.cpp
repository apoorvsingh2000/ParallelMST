#include <bits/stdc++.h>
#include <boruvka_sequential.h>
#include <prims_sequential.h>
#include <kruskals_sequential.h>

using namespace std;

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
        printf("./sequential <vertices> <edges> <selection> <edges_file>\n");
        exit(0);
    }

    string ifile = "";
    int vertices = 0;
    int edge = 0;
    int selection = 1;

    vertices = atoi(argv[1]);
    edge = atoi(argv[2]);
    selection = atoi(argv[3]);
    ifile += argv[4];

    vector<tuple<int,int,int>> edges;
    parseEdges(ifile, edges);

    // Declare graphs for the three methods
    BoruvkaGraph* g_b = new BoruvkaGraph(vertices);
    PrimsGraph* g_p = new PrimsGraph(vertices, selection);
    KruskalsGraph* g_k = new KruskalsGraph(vertices, selection);

    // Add edges to graph
    for (const auto& edge : edges) {
        int u, v, w;
        tie(u, v, w) = edge;  // Extract u, v, w from the tuple
        g_b -> EnterEdges(u, v, w);
        g_p -> EnterEdges(u, v, w);
        g_k -> EnterEdges(u, v, w);
    }

    // Get the start time
    auto start = std::chrono::high_resolution_clock::now();

    // Run Prim's MST algorithm
    g_p -> PrimMST();

    // Get the end time
    auto end = std::chrono::high_resolution_clock::now();

    // Calculate the elapsed time in seconds
    std::chrono::duration<double> elapsed = end - start;
    printf("Elapsed time for Prim's Sequential MST calculation for %d vertices, %d edges: %lf seconds\n\n", vertices, edge, elapsed.count());

    // Get the start time
    start = std::chrono::high_resolution_clock::now();

    // Run Kruskal's MST algorithm
    g_k -> KruskalMST();

    // Get the end time
    end = std::chrono::high_resolution_clock::now();

    // Calculate the elapsed time in seconds
    elapsed = end - start;
    printf("Elapsed time for Kruskal's Sequential MST calculation for %d vertices, %d edges: %lf seconds\n\n", vertices, edge, elapsed.count());

    // Get the start time
    start = std::chrono::high_resolution_clock::now();

    // Run Boruvka's MST algorithm
    g_b -> BoruvkaMST();

    // Get the end time
    end = std::chrono::high_resolution_clock::now();

    // Calculate the elapsed time in seconds
    elapsed = end - start;
    printf("Elapsed time for Boruvka's Sequential MST calculation for %d vertices, %d edges: %lf seconds\n\n", vertices, edge, elapsed.count());


    return 0;
}
