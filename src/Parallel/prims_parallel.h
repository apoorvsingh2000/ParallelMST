#ifndef PRIMS_PARALLEL_H
#define PRIMS_PARALLEL_H

#include<bits/stdc++.h>
#include<omp.h>
#include <parallel/algorithm>
#include<parallel_sort.h>
#include <parallel/settings.h>
#include <cmath>

using namespace std;

class PrimsGraph_p {
    int NUMTHREADS;
    // selection for sorting algo
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
    PrimsGraph_p(int V, int s, int n);

    void EnterEdges(int&, int&, int&);

    pair<int,int> FindNextMin();

    void PrimMST();
    
    void PrintPrimsMST();
};

#endif // PRIMS_PARALLEL_H