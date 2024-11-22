#ifndef BORUVKA_SEQUENTIAL_H
#define BORUVKA_SEQUENTIAL_H

#include <bits/stdc++.h>

using namespace std;

// Class to represent Graph
class BoruvkaGraph {
    int V; // Number of vertices
    vector<vector<int>> graph; // Adjacency matrix to store weights of graph
    vector<vector<int>> Parent;

    // Function to find the parent node of the given node of the graph
    int findParent(vector<int>& parent, int i);

    // Function to join two disjoint sets, does so by the context of ranks
    void unionSet(vector<int>& parent, vector<int>& rank, int& x_parent, int& y_parent);

public:
    BoruvkaGraph(int vertices);

    void EnterEdges(int&, int&, int&);

    void PrintBoruvkaMST();

    void BoruvkaMST();
};

#endif // BORUVKA_SEQUENTIAL_H