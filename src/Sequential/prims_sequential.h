#ifndef PRIMS_SEQUENTIAL_H
#define PRIMS_SEQUENTIAL_H

#include<bits/stdc++.h>
#include<sequential_sort.h>

using namespace std;

class PrimsGraph {
    int selection;
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
    PrimsGraph(int V, int s);

    void EnterEdges(int&, int&, int&);

    pair<int,int> FindNextMin();

    void PrimMST();

    void PrintPrimsMST();
};

#endif // PRIMS_SEQUENTIAL_H