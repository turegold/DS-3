#include <iostream>
#include <vector>
#include "GraphMethod.h"
#include <stack>
#include <queue>
#include <map>
#include <set>
#include <list>
#include <utility>
#include <algorithm>
#include <tuple>
#include <iomanip>

using namespace std;

const int INF = 1e9;

// Perform BFS traversal
bool BFS(Graph *graph, char option, int vertex, ofstream *fout)
{
    int size = graph->getSize();

    // Check valid range
    if (vertex < 0 || vertex >= size)
    {
        return false;
    }

    *fout << "========BFS========\n";
    if (option == 'O')
    {
        *fout << "Directed Graph BFS" << "\n";
    }
    else
    {
        *fout << "Undirected Graph BFS" << "\n";
    }
    *fout << "Start: " << vertex << "\n";

    vector<bool> visited(size, false);
    queue<int> Q;

    // Visit start vertex
    visited[vertex] = true;
    Q.push(vertex);

    bool first = true;

    while (!Q.empty())
    {
        int cur = Q.front();
        Q.pop();

        if (first)
        {
            *fout << cur, first = false;
        }
        else
        {
            *fout << " -> " << cur;
        }

        map<int, int> adj;
        if (option == 'O')
        {
            graph->getAdjacentEdgesDirect(cur, &adj);
        }
        else
        {
            graph->getAdjacentEdges(cur, &adj);
        }

        // Visit neighbor vertexs
        for (auto &p : adj)
        {
            int next = p.first;
            if (!visited[next])
            {
                visited[next] = true;
                Q.push(next);
            }
        }
    }

    *fout << "\n====================\n\n";
    return true;
}

// Perform DFS traversal
bool DFS(Graph *graph, char option, int vertex, ofstream *fout)
{
    int size = graph->getSize();

    // Check vertex is valid
    if (vertex < 0 || vertex >= size)
    {
        return false;
    }

    *fout << "========DFS========\n";
    if (option == 'O')
    {
        *fout << "Directed Graph DFS" << "\n";
    }
    else
    {
        *fout << "Undirected Graph DFS" << "\n";
    }
    *fout << "Start: " << vertex << "\n";

    vector<bool> visited(size, false);
    stack<int> S;
    vector<int> result;

    // Visit start vertex
    S.push(vertex);

    while (!S.empty())
    {
        int cur = S.top();
        S.pop();

        // Skip already visited vertex
        if (visited[cur])
        {
            continue;
        }

        visited[cur] = true;
        result.push_back(cur);

        map<int, int> adj;
        if (option == 'O')
        {
            graph->getAdjacentEdgesDirect(cur, &adj);
        }
        else
        {
            graph->getAdjacentEdges(cur, &adj);
        }

        // Collect neighbors to control visiting order
        vector<int> neighbors;
        for (auto &p : adj)
        {
            neighbors.push_back(p.first);
        }

        sort(neighbors.begin(), neighbors.end(), greater<int>());

        // Push unvisited negibors
        for (int next : neighbors)
        {
            if (!visited[next])
            {
                S.push(next);
            }
        }
    }

    // Print DFS order
    for (int i = 0; i < result.size(); i++)
    {
        *fout << result[i];
        if (i != result.size() - 1)
        {
            *fout << " -> ";
        }
    }
    *fout << "\n====================\n\n";

    return true;
}

// Find root vertex
int Find(vector<int> &parent, int x)
{
    if (parent[x] == x)
    {
        return x;
    }
    return parent[x] = Find(parent, parent[x]);
}

// Make same union
void Union(vector<int> &parent, int a, int b)
{
    a = Find(parent, a);
    b = Find(parent, b);
    if (a != b)
    {
        parent[b] = a;
    }
}

// Build a MST using Kruskal
bool Kruskal(Graph *graph, ofstream *fout)
{
    int size = graph->getSize();

    vector<tuple<int, int, int>> edges;

    // Collect all undirected edges
    for (int u = 0; u < size; u++)
    {
        map<int, int> adj;
        graph->getAdjacentEdges(u, &adj);

        for (auto &p : adj)
        {
            int v = p.first, w = p.second;
            if (u < v)
            {
                edges.push_back({w, u, v});
            }
        }
    }

    // Can't make MST
    if (edges.empty())
    {
        return false;
    }

    // Sort all edges by weight
    sort(edges.begin(), edges.end());

    vector<int> parent(size);
    for (int i = 0; i < size; i++)
    {
        parent[i] = i;
    }

    vector<vector<pair<int, int>>> mst(size);
    int total = 0, used = 0;

    // Process edges in increasing weight order
    for (auto &e : edges)
    {
        int w, u, v;
        tie(w, u, v) = e;

        if (Find(parent, u) != Find(parent, v))
        {
            Union(parent, u, v);
            used++;

            mst[u].push_back({v, w});
            mst[v].push_back({u, w});
            total += w;
        }
    }

    // Check MST has (size - 1) edges
    if (used != size - 1)
    {
        return false;
    }

    *fout << "========KRUSKAL========\n";
    for (int i = 0; i < size; i++)
    {
        *fout << "[" << i << "]";
        sort(mst[i].begin(), mst[i].end());
        for (auto &p : mst[i])
        {
            *fout << " " << p.first << "(" << p.second << ")";
        }
        *fout << "\n";
    }
    *fout << "Cost: " << total << "\n";
    *fout << "====================\n\n";

    return true;
}

// Compute shortest paths using Dijkstra
bool Dijkstra(Graph *graph, char option, int vertex, ofstream *fout)
{
    int size = graph->getSize();

    if (vertex < 0 || vertex >= size)
    {
        return false;
    }

    // Check negative edges
    for (int u = 0; u < size; u++)
    {
        map<int, int> adj;
        graph->getAdjacentEdgesDirect(u, &adj);
        for (auto &p : adj)
        {
            if (p.second < 0)
            {
                return false;
            }
        }
    }

    vector<int> dist(size, INF);
    vector<int> prev(size, -1);

    // Min-heap priority queue
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    dist[vertex] = 0;
    pq.push({0, vertex});

    while (!pq.empty())
    {
        auto top = pq.top();
        pq.pop();

        int cost = top.first;
        int cur = top.second;

        // Skip outdated entries
        if (cost > dist[cur])
        {
            continue;
        }

        // Get adjacency based on option
        map<int, int> adj;
        if (option == 'O')
        {
            graph->getAdjacentEdgesDirect(cur, &adj);
        }
        else
        {
            graph->getAdjacentEdges(cur, &adj);
        }

        // Relax edges
        for (auto &p : adj)
        {
            int next = p.first;
            int w = p.second;

            // If find shorter path
            if (dist[next] > dist[cur] + w)
            {
                dist[next] = dist[cur] + w;
                prev[next] = cur;
                pq.push({dist[next], next});
            }
        }
    }

    *fout << "========DIJKSTRA========\n";
    if (option == 'O')
    {
        *fout << "Directed Graph Dijkstra" << "\n";
    }
    else
    {
        *fout << "Undirected Graph Dijkstra" << "\n";
    }
    *fout << "Start: " << vertex << "\n";

    for (int v = 0; v < size; v++)
    {
        *fout << "[" << v << "] ";

        // Unreachable vertex
        if (dist[v] == INF)
        {
            *fout << "x\n";
            continue;
        }

        // Start vertex
        if (v == vertex)
        {
            *fout << v << " (0)\n";
            continue;
        }

        vector<int> path;
        int cur = v;

        while (cur != -1)
        {
            path.push_back(cur);
            cur = prev[cur];
        }

        reverse(path.begin(), path.end());

        for (int i = 0; i < path.size(); i++)
        {
            *fout << path[i];
            if (i != path.size() - 1)
                *fout << " -> ";
        }
        *fout << " (" << dist[v] << ")\n";
    }

    *fout << "====================\n\n";
    return true;
}

// Compute shortest path using Bellman-Ford
bool Bellmanford(Graph *graph, char option, int s, int e, ofstream *fout)
{
    int size = graph->getSize();

    // Check valid range
    if (s < 0 || s >= size || e < 0 || e >= size)
    {
        return false;
    }

    vector<tuple<int, int, int>> edges;

    // Collect all edges
    for (int u = 0; u < size; u++)
    {
        map<int, int> adj;
        if (option == 'O')
        {
            graph->getAdjacentEdgesDirect(u, &adj);
        }
        else
        {
            graph->getAdjacentEdges(u, &adj);
        }

        for (auto &p : adj)
        {
            edges.push_back({u, p.first, p.second});
        }
    }

    vector<int> dist(size, INF);
    vector<int> prev(size, -1);

    dist[s] = 0;

    // Relax edges up to (size - 1) times
    for (int i = 0; i < size - 1; i++)
    {
        bool updated = false;
        for (auto &ed : edges)
        {
            int u, v, w;
            tie(u, v, w) = ed;
            if (dist[u] != INF && dist[v] > dist[u] + w)
            {
                dist[v] = dist[u] + w;
                prev[v] = u;
                updated = true;
            }
        }
        // If there is no update, Early stop
        if (!updated)
        {
            break;
        }
    }

    // Check negative cycle
    for (auto &ed : edges)
    {
        int u, v, w;
        tie(u, v, w) = ed;
        if (dist[u] != INF && dist[v] > dist[u] + w)
        {
            return false;
        }
    }

    *fout << "========BELLMANFORD========\n";
    if (option == 'O')
    {
        *fout << "Directed Graph Bellman-Ford" << "\n";
    }
    else
    {
        *fout << "Undirected Graph Bellman-Ford" << "\n";
    }
    // If end vertex is unreachable
    if (dist[e] == INF)
    {
        *fout << "x\n";
        *fout << "Cost: x\n";
        *fout << "====================\n\n";
        return true;
    }

    vector<int> path;
    int cur = e;
    while (cur != -1)
    {
        path.push_back(cur);
        cur = prev[cur];
    }

    reverse(path.begin(), path.end());

    for (int i = 0; i < path.size(); i++)
    {
        *fout << path[i];
        if (i != path.size() - 1)
            *fout << " -> ";
    }
    *fout << "\nCost: " << dist[e] << "\n";
    *fout << "====================\n\n";

    return true;
}

// Compute all-pairs shortest paths using Floyd
bool FLOYD(Graph *graph, char option, ofstream *fout)
{
    int size = graph->getSize();
    vector<vector<int>> dist(size, vector<int>(size, INF));

    for (int u = 0; u < size; u++)
    {
        map<int, int> adj;
        if (option == 'O')
        {
            graph->getAdjacentEdgesDirect(u, &adj);
        }
        else
        {
            graph->getAdjacentEdges(u, &adj);
        }

        for (auto &p : adj)
        {
            dist[u][p.first] = p.second;
        }
    }

    // Consider self-loop
    for (int i = 0; i < size; i++)
    {
        dist[i][i] = min(dist[i][i], 0);
    }

    for (int k = 0; k < size; k++)
    {
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                if (dist[i][k] < INF && dist[k][j] < INF)
                {
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }

    // Check negative cycle
    for (int i = 0; i < size; i++)
    {
        if (dist[i][i] < 0)
        {
            return false;
        }
    }

    *fout << "========FLOYD========\n";
    if (option == 'O')
    {
        *fout << "Directed Graph Floyd" << "\n";
    }
    else
    {
        *fout << "Undirected Graph Floyd" << "\n";
    }

    *fout << "    ";
    for (int j = 0; j < size; j++)
    {
        *fout << "[" << j << "] ";
    }
    *fout << "\n";

    for (int i = 0; i < size; i++)
    {
        *fout << "[" << i << "] ";
        for (int j = 0; j < size; j++)
        {
            if (dist[i][j] >= INF)
            {
                *fout << "x   ";
            }
            else
            {
                *fout << dist[i][j] << "   ";
            }
        }
        *fout << "\n";
    }

    *fout << "====================\n\n";
    return true;
}

// Compute closeness centrality
bool Centrality(Graph *graph, ofstream *fout)
{
    int n = graph->getSize();

    vector<vector<int>> dist(n, vector<int>(n, INF));

    // Initialize
    for (int u = 0; u < n; u++)
    {
        map<int, int> adj;
        graph->getAdjacentEdges(u, &adj);
        for (auto &p : adj)
            dist[u][p.first] = p.second;
    }

    // Consider self-loop
    for (int i = 0; i < n; i++)
    {
        dist[i][i] = min(dist[i][i], 0);
    }

    // Run Floyd to compute all-pairs shortest paths
    for (int k = 0; k < n; k++)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (dist[i][k] < INF && dist[k][j] < INF)
                {
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }

    // Check negative cycle
    for (int i = 0; i < n; i++)
    {
        if (dist[i][i] < 0)
        {
            return false;
        }
    }

    vector<double> close(n);
    vector<bool> unreachable(n, false);
    vector<long long> denom(n);

    // Compute closeness centrality
    for (int i = 0; i < n; i++)
    {
        long long sum = 0;
        bool unreachable_flag = false;
        for (int j = 0; j < n; j++)
        {
            if (i == j)
            {
                continue;
            }
            if (dist[i][j] == INF)
            {
                unreachable_flag = true;
                break;
            }
            sum += dist[i][j];
        }

        if (unreachable_flag)
        {
            unreachable[i] = true;
        }
        else
        {
            denom[i] = sum;
            close[i] = double(n - 1) / sum;
        }
    }

    // Find maximum closeness value
    double maximum_closeness_value = 0;
    for (int i = 0; i < n; i++)
    {
        if (!unreachable[i])
        {
            maximum_closeness_value = max(maximum_closeness_value, close[i]);
        }
    }

    *fout << "========CENTRALITY========\n";
    for (int i = 0; i < n; i++)
    {
        *fout << "[" << i << "] ";
        if (unreachable[i])
        {
            *fout << "x\n";
        }
        else
        {
            *fout << (n - 1) << "/" << denom[i];
            if (abs(close[i] - maximum_closeness_value) < 1e-12)
            {
                *fout << " <- Most Central";
            }
            *fout << "\n";
        }
    }
    *fout << "====================\n\n";
    return true;
}