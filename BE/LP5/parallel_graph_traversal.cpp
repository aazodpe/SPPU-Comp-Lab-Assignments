#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <omp.h>
#include <chrono>
#include <cstring>

using namespace std;
using namespace chrono;

const int N = 50000; // Adjust graph size as needed

vector<int> adj[N]; // Adjacency list
bool visited[N];

// Utility to add an undirected edge
void addEdge(int u, int v) {
    adj[u].push_back(v);
    adj[v].push_back(u);
}

void generateRandomGraph(int V, int edges_per_node) {
    srand(time(0));
    for (int u = 0; u < V; u++) {
        for (int j = 0; j < edges_per_node; j++) {
            int v = rand() % V;
            if (v != u) {
                addEdge(u, v);
            }
        }
    }
}

// ---------------------- SERIAL BFS -----------------------
void serialBFS(int start) {
    memset(visited, false, sizeof(visited));
    queue<int> q;
    q.push(start);
    visited[start] = true;

    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : adj[u]) {
            if (!visited[v]) {
                visited[v] = true;
                q.push(v);
            }
        }
    }
}

// ---------------------- PARALLEL BFS -----------------------
void parallelBFS(int start) {
    memset(visited, false, sizeof(visited));
    queue<int> q;
    q.push(start);
    visited[start] = true;

    while (!q.empty()) {
        int size = q.size();

        #pragma omp parallel for
        for (int i = 0; i < size; i++) {
            int u;
            #pragma omp critical
            {
                u = q.front();
                q.pop();
            }

            for (int v : adj[u]) {
                if (!visited[v]) {
                    #pragma omp critical
                    {
                        if (!visited[v]) {
                            visited[v] = true;
                            q.push(v);
                        }
                    }
                }
            }
        }
    }
}

// ---------------------- SERIAL DFS -----------------------
void serialDFS(int u) {
    visited[u] = true;
    for (int v : adj[u]) {
        if (!visited[v])
            serialDFS(v);
    }
}

// ---------------------- PARALLEL DFS -----------------------
void parallelDFS(int start) {
    memset(visited, false, sizeof(visited));
    stack<int> s;
    s.push(start);

    while (!s.empty()) {
        int u = s.top(); s.pop();
        if (!visited[u]) {
            visited[u] = true;

            #pragma omp parallel for
            for (int i = 0; i < adj[u].size(); i++) {
                int v = adj[u][i];
                if (!visited[v]) {
                    #pragma omp critical
                    s.push(v);
                }
            }
        }
    }
}

// ---------------------- DRIVER CODE -----------------------
int main() {
    int V = 50000;
    generateRandomGraph(V, 20);
    vector<vector<int>> adj(V); // dynamic allocation
    for (int i = 0; i < V - 1; i++) {
        addEdge(i, i + 1);
    }

    cout << "Running Serial BFS..." << endl;
    auto start = high_resolution_clock::now();
    serialBFS(0);
    auto stop = high_resolution_clock::now();
    cout << "Serial BFS Time: " << duration_cast<microseconds>(stop - start).count() << " µs\n";

    cout << "Running Parallel BFS..." << endl;
    start = high_resolution_clock::now();
    parallelBFS(0);
    stop = high_resolution_clock::now();
    cout << "Parallel BFS Time: " << duration_cast<microseconds>(stop - start).count() << " µs\n";

    cout << "Running Serial DFS..." << endl;
    memset(visited, false, sizeof(visited));
    start = high_resolution_clock::now();
    serialDFS(0);
    stop = high_resolution_clock::now();
    cout << "Serial DFS Time: " << duration_cast<microseconds>(stop - start).count() << " µs\n";

    cout << "Running Parallel DFS..." << endl;
    memset(visited, false, sizeof(visited));
    start = high_resolution_clock::now();
    parallelDFS(0);
    stop = high_resolution_clock::now();
    cout << "Parallel DFS Time: " << duration_cast<microseconds>(stop - start).count() << " µs\n";
    return 0;
}

// compile with: g++ -fopenmp parallel_graph_traversal.cpp -o parallel_graph_traversal
// run with: ./parallel_graph_traversal
