#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <set>
#include <utility>

using namespace std;

int main(int argc, char* argv[]) {
    int E, V;
    string filename;

    if(argc < 4) {
        cout << "Usage: ./genEdge <vertices> <edges> <filename.txt>" << endl;
        exit(0);
    }

    V = stoi(argv[1]);
    E = stoi(argv[2]);
    filename = argv[3];

    if(E > (V * (V - 1)) / 2) {
        cout << "Wrong configuration of Vertices and Edges entered" << endl;
        cout << "E_max = V * (V - 1) / 2" << endl;
        exit(0);
    }

    // Seed for random number generation
    srand(time(0));

    // Open output file
    ofstream outFile(filename);
    if (!outFile) {
        cerr << "Error opening file!" << endl;
        exit(0);
    }

    // Track generated edges to avoid duplicates
    set<pair<int, int>> edges;

    // Generate 'E' unique edges
    while (edges.size() < E) {
        int u = rand() % V;
        int v = rand() % V;
        
        // Ensure u != v and check for duplicates (both u-v and v-u)
        if (u != v) {
            pair<int, int> edge1 = {u, v};
            pair<int, int> edge2 = {v, u};
            if (edges.find(edge1) == edges.end() && edges.find(edge2) == edges.end()) {
                edges.insert(edge1);
                int w = rand() % 100 + 1; // Random weight between 1 and 100
                outFile << u << " " << v << " " << w << endl;
            }
        }
    }

    outFile.close();
    cout << "Graph edges have been written to " << filename << endl;

    return 0;
}
