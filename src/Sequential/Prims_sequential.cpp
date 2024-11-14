#include <prims_sequential.h>

PrimsGraph :: PrimsGraph(int V, int s){
    selection = s;
    graph = vector<vector<int>> (V, vector<int> (V));
    Vertices = V;
    parent = vector<int> (V, -1);
    key = vector<int> (V, INT_MAX);
    MSTset = vector<bool> (V, false);
}

void PrimsGraph :: EnterEdges(int& u, int& v, int& w) {
    graph[u][v] = w;
    graph[v][u] = w;
    return;
}

pair<int,int> PrimsGraph :: FindNextMin() {
    int min = INT_MAX, min_idx;

    for(int v = 0; v < Vertices; v ++) {
        if(MSTset[v] == false && key[v] < min) { 
            min = key[v];
            min_idx = v;
        }
    }

    return {min_idx, min};
}

void PrimsGraph :: PrimMST() {
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

void PrimsGraph :: PrintPrimsMST() {
    printf("Edge \tWeight\n");
    for(int i = 1; i < Vertices; i ++){
        printf("%d - %d \t%d\n", parent[i], i, graph[i][parent[i]]);
    }
}
