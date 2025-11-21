#include <iostream>
#include <vector>
#include "GraphMethod.h"
#include <stack>
#include <queue>
#include <map>
#include <set>
#include <list>
#include <utility>

using namespace std;

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

    // 방문처리
    visited[vertex] = true;
    Q.push(vertex);
    bool first = true;

    while (!Q.empty())
    {
        // 하나 꺼내기
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

        for (auto &p : adj)
        {
            int next = p.first;
            // 방문 안했으면 방문 처리
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
    int n = graph->getSize();

    // 2) DFS 시작 헤더 출력
    *fout << "========DFS========" << endl;

    if (option == 'O')
        *fout << "Directed Graph DFS" << endl;
    else
        *fout << "Undirected Graph DFS" << endl;

    *fout << "Start: " << vertex << endl;

    // 3) DFS 수행
    vector<bool> visited(n, false);
    stack<int> S;
    vector<int> result;

    S.push(vertex);

    while (!S.empty())
    {
        int cur = S.top();
        S.pop();

        // 이미 방문 했으면 넘어감
        if (visited[cur])
        {
            continue;
        }

        visited[cur] = true;
        result.push_back(cur);

        // 인접 정점들 가져오기
        map<int, int> adj;

        if (option == 'O')
        {
            graph->getAdjacentEdgesDirect(cur, &adj);
        }
        else
        {
            graph->getAdjacentEdges(cur, &adj);
        }

        // DFS는 큰 번호부터 push해야 작은 번호가 먼저 pop됨 (오름차순 방문)
        vector<int> neighbors;
        for (auto &p : adj)
        {
            neighbors.push_back(p.first);
        }

        // 주변 정점을 오름차순으로 정렬
        sort(neighbors.begin(), neighbors.end(), greater<int>());

        for (int next : neighbors)
        {
            if (!visited[next])
            {
                S.push(next);
            }
        }
    }

    // 4) 방문 순서 출력
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

int Find(vector<int> &parent, int x)
{
    if (parent[x] == x)
    {
        return x;
    }
    return parent[x] = Find(parent, parent[x]);
}

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
    int n = graph->getSize();

    vector<tuple<int, int, int>> edges;

    // 1) 모든 간선 수집
    for (int u = 0; u < n; u++)
    {
        map<int, int> adj;
        graph->getAdjacentEdges(u, &adj);

        for (auto &p : adj)
        {
            int v = p.first;
            int w = p.second;

            // 중복 제거
            if (u < v)
            {
                edges.push_back({w, u, v});
            }
        }
    }

    if (edges.empty())
    {
        return false;
    }

    // 2) weight 기준 정렬
    sort(edges.begin(), edges.end());

    // 3) Union-Find 준비
    vector<int> parent(n);
    for (int i = 0; i < n; i++)
    {
        parent[i] = i;
    }

    vector<vector<pair<int, int>>> mst_adj(n);
    int total_cost = 0;
    int used_edge = 0;

    // 4) Kruskal 수행
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

    // MST가 완성되었는지 확인
    if (used_edge != n - 1)
    {
        return false;
    }

    // 5) 출력
    *fout << "========KRUSKAL========" << endl;

    for (int u = 0; u < n; u++)
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
    int n = graph->getSize();
    const int INF = 1e9;

    // ------------------------------
    // [1] 음수 가중치가 있으면 Dijkstra 불가능 → false 반환
    //     (그래프 전체에서 음수 간선이 있는지 검사)
    // ------------------------------
    for (int u = 0; u < n; u++)
    {
        map<int, int> adj;
        graph->getAdjacentEdgesDirect(u, &adj); // 음수 검사에는 방향 중요하지 않음

        for (auto &p : adj)
        {
            // 음수 가중치 존재
            if (p.second < 0)
            {
                return false;
            }
        }
    }

    // ------------------------------
    // [2] Dijkstra 준비
    // ------------------------------
    vector<int> dist(n, INF); // 최단 거리
    vector<int> prev(n, -1);  // 경로 복원용
    priority_queue<pair<int, int>,
                   vector<pair<int, int>>,
                   greater<pair<int, int>>>
        pq;

    // 시작점 설정
    dist[vertex] = 0;
    pq.push({0, vertex});

    // ------------------------------
    // [4] Dijkstra 실행
    // ------------------------------
    while (!pq.empty())
    {
        auto [cost, cur] = pq.top();
        pq.pop();

        if (cost > dist[cur])
            continue;

        // 인접 정점들 가져오기
        map<int, int> adj;

        if (option == 'O')
            graph->getAdjacentEdgesDirect(cur, &adj); // 방향 O
        else
            graph->getAdjacentEdges(cur, &adj); // 방향 X (무방향)

        // Relaxation
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

    // ------------------------------
    // [5] 결과 출력 시작
    // ------------------------------
    *fout << "========DIJKSTRA========" << endl;

    if (option == 'O')
        *fout << "Directed Graph Dijkstra" << endl;
    else
        *fout << "Undirected Graph Dijkstra" << endl;

    *fout << "Start: " << vertex << endl;

    // ------------------------------
    // [6] 각 정점에 대해 경로 출력
    // ------------------------------
    for (int v = 0; v < n; v++)
    {
        *fout << "[" << v << "] ";

        if (dist[v] == INF)
        {
            // 도달 불가능
            *fout << "x" << endl;
            continue;
        }

        // dist[v] == 0 → 시작 정점
        if (v == vertex)
        {
            *fout << v << " (0)" << endl;
            continue;
        }

        // ------------------------------
        // 경로 복원 (역순)
        // ------------------------------
        vector<int> path;
        int cur = v;

        while (cur != -1)
        {
            path.push_back(cur);
            cur = prev[cur];
        }

        reverse(path.begin(), path.end());

        // ------------------------------
        // 경로 출력
        // ------------------------------
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
}

bool FLOYD(Graph *graph, char option, ofstream *fout)
{
}

bool Centrality(Graph *graph, ofstream *fout)
{
}