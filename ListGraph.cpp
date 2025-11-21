#include "ListGraph.h"
#include <iostream>
#include <utility>

ListGraph::ListGraph(bool type, int size) : Graph(type, size)
{
    // adjacency list 생성
    m_List = new map<int, int>[size];
}

ListGraph::~ListGraph()
{
    // 동적 배열 해제
    delete[] m_List;
}

void ListGraph::getAdjacentEdges(int vertex, map<int, int> *m) // Definition of getAdjacentEdges(No Direction == Undirected)
{
    // 1. 정방향 간선 추가
    for (auto &cur_vertex : m_List[vertex])
    {
        int to = cur_vertex.first;
        int weight = cur_vertex.second;
        (*m)[to] = weight;
    }

    // 2. 역방향 간선도 찾아서 추가 (무방향 그래프에서 인접 정점을 구하기 위해)
    for (int i = 0; i < m_Size; i++)
    {
        if (m_List[i].find(vertex) != m_List[i].end())
        {
            (*m)[i] = m_List[i][vertex];
        }
    }
}

void ListGraph::getAdjacentEdgesDirect(int vertex, map<int, int> *m) // Definition of getAdjacentEdges(Directed graph)
{
    // Vertex에서 나가는 정방향 간선을 추가
    for (auto &cur_vertex : m_List[vertex])
    {
        (*m)[cur_vertex.first] = cur_vertex.second;
    }
}

void ListGraph::insertEdge(int from, int to, int weight) // Definition of insertEdge
{
    m_List[from][to] = weight;
}

bool ListGraph::printGraph(ofstream *fout) // Definition of print Graph
{
    if (!fout || !fout->is_open())
    {
        return false;
    }

    for (int i = 0; i < m_Size; i++)
    {
        (*fout) << "[" << i << "]";

        // 아무 간선도 없으면 바로 줄바꿈
        if (m_List[i].empty())
        {
            (*fout) << " ->" << "\n";
            continue;
        }

        // map<int,int> 자동으로 key(도착정점) 오름차순 정렬
        for (auto &edge : m_List[i])
        {
            int to = edge.first;
            int weight = edge.second;

            (*fout) << " -> (" << to << "," << weight << ")";
        }

        (*fout) << "\n";
    }

    return true;
}