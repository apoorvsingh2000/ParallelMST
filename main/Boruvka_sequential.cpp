#include <bits/stdc++.h>

using namespace std;

// Class to represent Graph
class Graph {
    int V; // Number of vertices
    vector<vector<int>> graph; // Adjacency matrix to store weights of graph

    // Function to find the parent node of the given node of the graph
    int findParent(vector<int>& parent, int i){
        // Base case
        if(parent[i] == i)
            return i;

        // Recursive call to find parent
        return parent[i] = findParent(parent, parent[i]);
    }

    // Function to join two disjoint sets, does so by the context of ranks
    void unionSet(vector<int>& parent, vector<int>& rank, int x, int y){
        // Retrieve the parents of the given nodes to join
        int x_parent = findParent(parent, x);
        int y_parent = findParent(parent, y);

        // Get the corresponding rank values of the parent nodes (i.e. the depth)
        int x_rank = rank[x_parent];
        int y_rank = rank[y_parent];

        if(x_rank < y_rank){
            // Set the parent of x_parent to its opposite half
            parent[x_parent] = y_parent;
        }
        else if(y_rank < x_rank){
            // Set the parent of y_parent to its opposite half
            parent[y_parent] = x_parent;
        }
        else{ 
            // Both ranks are equal, increment the rank of one, here x_parent
            parent[y_parent] = x_parent;
            rank[x_parent] += 1;
        }
    }

public:
    Graph(int vertices){
        // Set the number of vertices
        V = vertices;
        
        // Resize the matrix to V x V and set all weights to -1
        //graph = vector<vector<int>>();
    }

    // Add edges to the adj. matrix
    void addEdge(int u, int v, int w){
        graph.push_back({u, v, w});
    }

    // Boruvka's Algorithm MAIN
    void BoruvkaMST(){
        // Inintialize parent and rank vector
        vector<int> parent(V);

        // Rank of all nodes will initially be 0
        vector<int> rank(V, 0);

        // Initialize the parent of each node to itself
        for(int i = 0; i < V; i ++){
            parent[i] = i;
        }

        // Declare variables to store the current number of trees present and the MST score
        int MSTweight = 0;
        int numTrees = V;

        // Declare a dictionary to store the cheapest edge, in the form {u, v, w}
        // initialized to -1
        vector<vector<int>> cheapest (V, vector<int> (3, -1));

        // Loop till the number of trees become 1
        while(numTrees > 1){
            // Traverse through the edges in the graph and assign lowest weights to all parent nodes.
            for(int i = 0; i < graph.size(); i ++){
                // Unzip the values of u, v and w
                int u = graph[i][0], v = graph[i][1], w = graph[i][2];

                int parent_u = findParent(parent, u);
                int parent_v = findParent(parent, v);

                // check if the parents of the two trees are the same
                // If the same than joining these two nodes will lead to a cycle.
                if(parent_u != parent_v){
                    // check if the current weight is lower than the previous cheapest weight
                    if(cheapest[parent_u][2] == -1 || cheapest[parent_u][2] > w){
                        // update the cheapest edge
                        cheapest[parent_u] = {u, v, w};
                    }
                    if(cheapest[parent_v][2] == -1 || cheapest[parent_v][2] > w){
                        // update cheapest weight
                        cheapest[parent_v] = {u, v, w};
                    }
                }
            }

            // Once the cheapest edges are found, join them and update corrsponding rank values
            for(int node = 0; node < V; node ++){
                // if the cheapest weight has been found
                if(cheapest[node][2] != -1){
                    // unzip the values of u, v, w
                    int u = cheapest[node][0], v = cheapest[node][1], w = cheapest[node][2];

                    // find parent of u and v
                    int parent_u = findParent(parent, u);
                    int parent_v = findParent(parent, v);

                    if(parent_u != parent_v){
                        // Update the weight
                        MSTweight += w;

                        // Merge the two trees into one
                        unionSet(parent, rank, parent_u, parent_v);

                        // The number of trees decreases by 1
                        numTrees -= 1;
                    }
                }
            }

            // reset the cheapest vector as we have merged two of the constituent trees
            cheapest.resize(V, vector<int>(3, -1));
        }

        printf("Weight of MST is %d\n", MSTweight);
    }
};


int main(){
    Graph g(4);
    g.addEdge(0, 1, 10);
    g.addEdge(0, 2, 6);
    g.addEdge(0, 3, 5);
    g.addEdge(1, 3, 15);
    g.addEdge(2, 3, 4);
    
    g.BoruvkaMST();
}