#include "ListGraph.h"
#include <iostream>
#include <utility>

ListGraph::ListGraph(bool type, int size) : Graph(type, size)
{
    // Create adjacency list
    m_List = new map<int, int>[size];
}

ListGraph::~ListGraph()
{
    // Delete Graph
    delete[] m_List;
}

void ListGraph::getAdjacentEdges(int vertex, map<int, int> *m) // Definition of getAdjacentEdges(No Direction == Undirected)
{
    // Add outgoing edges
    for (auto &cur_vertex : m_List[vertex])
    {
        int to = cur_vertex.first;
        int weight = cur_vertex.second;
        (*m)[to] = weight;
    }

    // Add incoming edges to treat the graph as undirected
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
    // Add outgoing edges
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

        if (m_List[i].empty())
        {
            (*fout) << " ->" << "\n";
            continue;
        }

        // Print all edges in ascending order of destination vertex
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