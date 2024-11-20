#ifndef KRUSKALS_PARALLEL_H
#define KRUSKALS_PARALLEL_H

#include<bits/stdc++.h>
#include<omp.h>
#include<parallel/algorithm>
#include <parallel_sort.h>
#include <parallel/settings.h>
#include <cmath>

using namespace std;

class UnionSet_p {
    int* parent;
    int* rank;

public:
    UnionSet_p(int n);
    
    int findParent(int);

    void Union(int&, int&);
};

class KruskalsGraph_p {
    // selection for sorting algo
    int NUMTHREADS;
    int selection;
    vector<vector<int>> edgeList;
    vector<vector<int>> Parent;

    int Vertices;

public:
    KruskalsGraph_p(int V, int s, int n);

    void EnterEdges(int&, int&, int&);

    void SortList();

    void KruskalMST();

    void PrintKruskalMST();
};

#endif // KRUSKALS_PARALLEL_H