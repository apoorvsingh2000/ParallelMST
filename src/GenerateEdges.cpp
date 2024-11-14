#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;

int main(int argc, char* argv[]) {
    int E, V;
    string filename;

    if (argc < 4) {
        cout << "Usage: ./genEdge <vertices> <edges> <filename.txt>" << endl;
        exit(0);
    }

    V = stoi(argv[1]);
    E = stoi(argv[2]);
    filename = argv[3];

    if (E > (V * (V - 1)) / 2) {
        cout << "Wrong configuration of Vertices and Edges entered" << endl;
        cout << "E_max = V * (V - 1) / 2" << endl;
        exit(0);
    }

    // Seed for random number generation
    srand(time(0));

    // Generate all possible edges (u, v) where u < v
    vector<pair<int, int>> allEdges;
    for (int u = 0; u < V; u++) {
        for (int v = u + 1; v < V; v++) {
            allEdges.push_back({u, v});
        }
    }

    // Create a random number generator
    random_device rd;
    mt19937 g(rd());

    // Shuffle the edges randomly using std::shuffle
    shuffle(allEdges.begin(), allEdges.end(), g);

    // Open output file
    ofstream outFile(filename);
    if (!outFile) {
        cerr << "Error opening file!" << endl;
        exit(0);
    }

    // Write the first 'E' edges to the file with random weights
    for (int i = 0; i < E; i++) {
        int u = allEdges[i].first;
        int v = allEdges[i].second;
        int w = rand() % 100 + 1; // Random weight between 1 and 100
        outFile << u << " " << v << " " << w << endl;
    }

    outFile.close();
    cout << "Graph edges have been written to " << filename << endl;

    return 0;
}