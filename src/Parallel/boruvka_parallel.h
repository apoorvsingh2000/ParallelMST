// boruvka_parallel.h
#ifndef BORUVKA_PARALLEL_H
#define BORUVKA_PARALLEL_H

#include <bits/stdc++.h>
#include <omp.h>

using namespace std;

// Class to represent BoruvkaGraph
class BoruvkaGraph_p {
    int NUMTHREADS;
    int V; // Number of vertices
    vector<vector<int>> graph; // Adjacency matrix to store weights of graph
    vector<vector<int>> Parent;

    int findParent(vector<int>& parent, int i);

    void unionSet(vector<int>& parent, vector<int>& rank, int& x_parent, int& y_parent);

public:
    BoruvkaGraph_p(int vertices, int n);

    // Add edges to the adj. matrix
    void EnterEdges(int u, int v, int w);

    void PrintBoruvkaMST();
    
    // Boruvka's Algorithm MAIN
    void BoruvkaMST();
};

#endif // BORUVKA_PARALLEL_H