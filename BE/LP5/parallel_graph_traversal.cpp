// parallel_graph_traversal.cpp
// Assignment: Parallel BFS and DFS using OpenMP
// Course: High Performance Computing
// Author: [Your Name]
// Date: [Date]

// Required headers
#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>

using namespace std;

// ----------------------
// Function: parallelBFS
// ----------------------
// Performs parallel Breadth-First Search using OpenMP
void parallelBFS(const vector<vector<int>>& adj, int start) {
    int n = adj.size();
    vector<bool> visited(n, false);
    queue<int> q;

    visited[start] = true;
    q.push(start);

    cout << "Parallel BFS Traversal:\n";

    while (!q.empty()) {
        int size = q.size();

        #pragma omp parallel for
        for (int i = 0; i < size; ++i) {
            int node;

            // Critical section to pop from queue
            #pragma omp critical
            {
                if (!q.empty()) {
                    node = q.front();
                    q.pop();
                    cout << "Visited: " << node << "\n";
                }
            }

            // Traverse neighbors
            #pragma omp parallel for
            for (int j = 0; j < adj[node].size(); ++j) {
                int neighbor = adj[node][j];

                // Critical section to mark and enqueue unvisited nodes
                #pragma omp critical
                {
                    if (!visited[neighbor]) {
                        visited[neighbor] = true;
                        q.push(neighbor);
                    }
                }
            }
        }
    }
}

// ----------------------
// Helper: parallelDFSUtil
// ----------------------
// Recursive utility for parallel DFS using OpenMP tasks
void parallelDFSUtil(const vector<vector<int>>& adj, int node, vector<bool>& visited) {
    // Critical section to avoid race conditions on visited[]
    #pragma omp critical
    {
        if (visited[node]) return;
        visited[node] = true;
        cout << "Visited: " << node << "\n";
    }

    // Launch parallel tasks for unvisited neighbors
    for (int neighbor : adj[node]) {
        #pragma omp task firstprivate(neighbor)
        {
            if (!visited[neighbor]) {
                parallelDFSUtil(adj, neighbor, visited);
            }
        }
    }

    #pragma omp taskwait
}

// ----------------------
// Function: parallelDFS
// ----------------------
// Performs parallel Depth-First Search using OpenMP
void parallelDFS(const vector<vector<int>>& adj, int start) {
    int n = adj.size();
    vector<bool> visited(n, false);

    cout << "Parallel DFS Traversal:\n";

    #pragma omp parallel
    {
        #pragma omp single
        parallelDFSUtil(adj, start, visited);
    }
}

// ----------------------
// Function: main
// ----------------------
// Entry point: defines a graph and runs both traversals
int main() {
    // Define an undirected graph as adjacency list
    int n = 6; // number of nodes
    vector<vector<int>> adj(n);

    // Edges
    adj[0] = {1, 2};
    adj[1] = {0, 3, 4};
    adj[2] = {0, 4};
    adj[3] = {1, 5};
    adj[4] = {1, 2};
    adj[5] = {3};

    // Perform traversals
    parallelBFS(adj, 0);
    cout << "\n";
    parallelDFS(adj, 0);

    return 0;
}