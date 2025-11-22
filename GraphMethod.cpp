#include <iostream>
#include <vector>
#include "GraphMethod.h"
#include <stack>
#include <queue>
#include <map>
#include <set>
#include <list>
#include <utility>
#include <iomanip>

using namespace std;

const int INF = 1e9;

bool BFS(Graph *graph, char option, int vertex, ofstream *fout)
{
    int size = graph->getSize();

    *fout << "========BFS========" << endl;

    if (option == 'O')
    {
        *fout << "Directed Graph BFS" << endl;
    }
    else
    {
        *fout << "Undirected Graph BFS" << endl;
    }

    *fout << "Start: " << vertex << endl;

    vector<bool> visited(size, false);
    queue<int> Q;

    // Mark starting vertex as visited
    visited[vertex] = true;
    Q.push(vertex);
    bool first = true;

    while (!Q.empty())
    {
        int cur = Q.front();
        Q.pop();

        if (first)
        {
            *fout << cur;
            first = false;
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

        // Visit all adjacent vertexes
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

    *fout << endl
          << "====================" << endl
          << endl;
    return true;
}

bool DFS(Graph *graph, char option, int vertex, ofstream *fout)
{
    int size = graph->getSize();

    *fout << "========DFS========" << endl;

    if (option == 'O')
        *fout << "Directed Graph DFS" << endl;
    else
        *fout << "Undirected Graph DFS" << endl;

    *fout << "Start: " << vertex << endl;

    vector<bool> visited(size, false);
    stack<int> S;
    vector<int> result;

    S.push(vertex);

    while (!S.empty())
    {
        int cur = S.top();
        S.pop();

        // Skip nodes that have already been visited
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

        // push in reverse-sorted order
        vector<int> neighbors;
        for (auto &p : adj)
        {
            neighbors.push_back(p.first);
        }

        // Reverse sort so smaller is processed first
        sort(neighbors.begin(), neighbors.end(), greater<int>());

        for (int next : neighbors)
        {
            if (!visited[next])
            {
                S.push(next);
            }
        }
    }

    // Print DFS result order
    for (int i = 0; i < result.size(); i++)
    {
        *fout << result[i];
        if (i != result.size() - 1)
        {
            *fout << " -> ";
        }
    }
    *fout << endl;

    *fout << "====================" << endl
          << endl;

    return true;
}

// Find the root
int Find(vector<int> &parent, int x)
{
    if (parent[x] == x)
    {
        return x;
    }
    return parent[x] = Find(parent, parent[x]);
}

// Merge the sets containing nodes a and b
void Union(vector<int> &parent, int a, int b)
{
    a = Find(parent, a);
    b = Find(parent, b);
    if (a != b)
    {
        parent[b] = a;
    }
}

bool Kruskal(Graph *graph, ofstream *fout)
{
    int size = graph->getSize();

    vector<tuple<int, int, int>> edges;

    // Collect all edges from the graph
    for (int u = 0; u < size; u++)
    {
        map<int, int> adj;
        graph->getAdjacentEdges(u, &adj);

        for (auto &p : adj)
        {
            int v = p.first;
            int w = p.second;

            // Avoid duplicate edges
            if (u < v)
            {
                edges.push_back({w, u, v});
            }
        }
    }

    // If no edges, can't make MST
    if (edges.empty())
    {
        return false;
    }

    // Sort edges by weight in ascending order
    sort(edges.begin(), edges.end());

    // Initialize Union-Find
    vector<int> parent(size);
    for (int i = 0; i < size; i++)
    {
        parent[i] = i;
    }

    vector<vector<pair<int, int>>> mst_adj(size);
    int total_cost = 0;
    int used_edge = 0;

    // Process edges in increasing weight order
    for (auto &e : edges)
    {
        int w, u, v;
        tie(w, u, v) = e;

        if (Find(parent, u) != Find(parent, v))
        {
            Union(parent, u, v);
            used_edge++;

            mst_adj[u].push_back({v, w});
            mst_adj[v].push_back({u, w});
            total_cost += w;
        }
    }

    // Check MST maked
    if (used_edge != size - 1)
    {
        return false;
    }

    // Print
    *fout << "========KRUSKAL========" << endl;

    for (int u = 0; u < size; u++)
    {
        *fout << "[" << u << "]";
        sort(mst_adj[u].begin(), mst_adj[u].end());

        for (auto &p : mst_adj[u])
        {
            *fout << " " << p.first << "(" << p.second << ")";
        }
        *fout << endl;
    }

    *fout << "Cost: " << total_cost << endl;
    *fout << "====================" << endl
          << endl;

    return true;
}

bool Dijkstra(Graph *graph, char option, int vertex, ofstream *fout)
{
    int size = graph->getSize();

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
    priority_queue<pair<int, int>,
                   vector<pair<int, int>>,
                   greater<pair<int, int>>>
        pq;

    dist[vertex] = 0;
    pq.push({0, vertex});

    // Dijkstra algorithm loop
    while (!pq.empty())
    {
        pair<int, int> top = pq.top();
        pq.pop();

        int cost = top.first;
        int cur = top.second;

        // Skip outdated entries
        if (cost > dist[cur])
        {
            continue;
        }

        map<int, int> adj;

        if (option == 'O')
            graph->getAdjacentEdgesDirect(cur, &adj);
        else
            graph->getAdjacentEdges(cur, &adj);

        // Relax edges
        for (auto &p : adj)
        {
            int next = p.first;
            int weight = p.second;

            if (dist[next] > dist[cur] + weight)
            {
                dist[next] = dist[cur] + weight;
                prev[next] = cur;
                pq.push({dist[next], next});
            }
        }
    }

    *fout << "========DIJKSTRA========" << endl;

    if (option == 'O')
        *fout << "Directed Graph Dijkstra" << endl;
    else
        *fout << "Undirected Graph Dijkstra" << endl;

    *fout << "Start: " << vertex << endl;

    // Print result for each vertex
    for (int v = 0; v < size; v++)
    {
        *fout << "[" << v << "] ";

        if (dist[v] == INF)
        {
            // Unreachable vertex
            *fout << "x" << endl;
            continue;
        }

        // Starting vertex
        if (v == vertex)
        {
            *fout << v << " (0)" << endl;
            continue;
        }

        // Restore shortest path by tracing predecessors
        vector<int> path;
        int cur = v;

        while (cur != -1)
        {
            path.push_back(cur);
            cur = prev[cur];
        }

        reverse(path.begin(), path.end());

        // Print restored path
        for (int i = 0; i < path.size(); i++)
        {
            *fout << path[i];
            if (i != path.size() - 1)
                *fout << " -> ";
        }

        *fout << " (" << dist[v] << ")" << endl;
    }

    *fout << "====================" << endl;
    *fout << endl;

    return true;
}

bool Bellmanford(Graph *graph, char option, int s_vertex, int e_vertex, ofstream *fout)
{
    int size = graph->getSize();

    // Build a list of all edges in the graph (u, v, weight)
    vector<tuple<int, int, int>> edges;

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
            int v = p.first;
            int w = p.second;
            edges.push_back({u, v, w});
        }
    }

    // Initialize Bellmanford
    vector<int> dist(size, INF);
    vector<int> prev(size, -1);

    dist[s_vertex] = 0;

    // Relax all edges
    for (int i = 0; i < size - 1; i++)
    {
        bool updated = false;

        for (auto &e : edges)
        {
            int u, v, w;
            tie(u, v, w) = e;

            if (dist[u] != INF && dist[v] > dist[u] + w)
            {
                dist[v] = dist[u] + w;
                prev[v] = u;
                updated = true;
            }
        }

        if (!updated)
        {
            break;
        }
    }

    // Check for a negative cycle
    for (auto &e : edges)
    {
        int u, v, w;
        tie(u, v, w) = e;

        if (dist[u] != INF && dist[v] > dist[u] + w)
        {
            return false;
        }
    }

    *fout << "========BELLMANFORD========" << endl;

    if (option == 'O')
        *fout << "Directed Graph Bellman-Ford" << endl;
    else
        *fout << "Undirected Graph Bellman-Ford" << endl;

    // If destination is unreachable, print x.
    if (dist[e_vertex] == INF)
    {
        *fout << "x" << endl;
        *fout << "Cost: x" << endl;
        *fout << "====================" << endl
              << endl;
        return true;
    }

    // Restore path from s_vertex to e_vertex
    vector<int> path;
    int cur = e_vertex;

    while (cur != -1)
    {
        path.push_back(cur);
        cur = prev[cur];
    }

    reverse(path.begin(), path.end());

    // Print path
    for (int i = 0; i < path.size(); i++)
    {
        *fout << path[i];
        if (i != path.size() - 1)
            *fout << " -> ";
    }
    *fout << endl;

    // Print cost
    *fout << "Cost: " << dist[e_vertex] << endl;
    *fout << "====================" << endl;
    *fout << endl;

    return true;
}

bool FLOYD(Graph *graph, char option, ofstream *fout)
{
    int size = graph->getSize();
    const int INF = 1e9;

    // Initialize distance matrix.
    vector<vector<int>> dist(size, vector<int>(size, INF));

    for (int i = 0; i < size; i++)
    {
        dist[i][i] = 0;
    }

    // Load initial edge weights from the graph into dist matrix
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
            int v = p.first;
            int w = p.second;
            dist[u][v] = w;
        }
    }

    // Floyd algorithm
    for (int k = 0; k < size; k++)
    {
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                if (dist[i][k] == INF || dist[k][j] == INF)
                {
                    continue;
                }

                if (dist[i][j] > dist[i][k] + dist[k][j])
                {
                    dist[i][j] = dist[i][k] + dist[k][j];
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

    // Print result
    *fout << "========FLOYD========" << endl;

    if (option == 'O')
        *fout << "Directed Graph Floyd" << endl;
    else
        *fout << "Undirected Graph Floyd" << endl;

    *fout << "    ";
    for (int j = 0; j < size; j++)
    {
        *fout << "[" << j << "] ";
    }
    *fout << endl;

    // Print matrix
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
        *fout << endl;
    }

    *fout << "====================" << endl;
    *fout << endl;

    return true;
}

bool Centrality(Graph *graph, ofstream *fout)
{
    int n = graph->getSize();

    // Initialize distance matrix
    vector<vector<int>> dist(n, vector<int>(n, INF));

    for (int i = 0; i < n; i++)
    {
        dist[i][i] = 0;
    }

    // Load edge weights from the graph
    for (int u = 0; u < n; u++)
    {
        map<int, int> adj;
        graph->getAdjacentEdges(u, &adj);

        for (auto &p : adj)
        {
            int v = p.first;
            int w = p.second;
            dist[u][v] = w;
        }
    }

    // Floyd algorithm
    for (int k = 0; k < n; k++)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (dist[i][k] == INF || dist[k][j] == INF)
                {
                    continue;
                }

                if (dist[i][j] > dist[i][k] + dist[k][j])
                {
                    dist[i][j] = dist[i][k] + dist[k][j];
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

    // Compute closeness centrality for each vertex
    vector<double> closeness(n, 0);
    vector<int> denom(n, 0);     // denominator
    vector<int> numer(n, n - 1); // numerator

    for (int i = 0; i < n; i++)
    {
        long long sum = 0;
        bool unreachable = false;

        for (int j = 0; j < n; j++)
        {
            if (i == j)
            {
                continue;
            }

            if (dist[i][j] == INF)
            {
                unreachable = true;
                break;
            }
            sum += dist[i][j];
        }

        // If any vertex is unreachable, or sum = 0
        if (unreachable || sum == 0)
        {
            closeness[i] = 0;
            denom[i] = 1;
            numer[i] = 0;
        }
        else
        {
            denom[i] = sum;
            closeness[i] = double(n - 1) / sum;
        }
    }

    // Find the maximum closeness value
    double max_centrality = 0;
    for (int i = 0; i < n; i++)
    {
        max_centrality = max(max_centrality, closeness[i]);
    }

    // Print result
    *fout << "========CENTRALITY========" << endl;

    for (int i = 0; i < n; i++)
    {
        *fout << "[" << i << "] " << numer[i] << "/" << denom[i];

        if (abs(closeness[i] - max_centrality) < 1e-12 && max_centrality > 0)
        {
            *fout << " <- Most Central";
        }

        *fout << endl;
    }

    *fout << "====================" << endl;
    *fout << endl;

    return true;
}