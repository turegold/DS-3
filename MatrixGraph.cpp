#include "MatrixGraph.h"
#include <iostream>
#include <vector>
#include <string>

MatrixGraph::MatrixGraph(bool type, int size) : Graph(type, size)
{
    // Allocate a size x size adjacency matrix
    m_Mat = new int *[m_Size];
    for (int i = 0; i < m_Size; i++)
    {
        m_Mat[i] = new int[m_Size];
        // Initialize all entries to 0
        for (int j = 0; j < m_Size; j++)
        {
            m_Mat[i][j] = 0;
        }
    }
}

MatrixGraph::~MatrixGraph()
{
    // Release allocated adjacency matrix
    for (int i = 0; i < m_Size; i++)
    {
        delete[] m_Mat[i];
    }
    delete[] m_Mat;
}

void MatrixGraph::getAdjacentEdges(int vertex, map<int, int> *m)
{
    // Add outgoing edges
    for (int i = 0; i < m_Size; i++)
    {
        if (m_Mat[vertex][i] != 0)
        {
            (*m)[i] = m_Mat[vertex][i];
        }
    }

    // Add incoming edges
    for (int i = 0; i < m_Size; i++)
    {
        if (m_Mat[i][vertex] != 0)
        {
            (*m)[i] = m_Mat[i][vertex];
        }
    }
}

void MatrixGraph::getAdjacentEdgesDirect(int vertex, map<int, int> *m)
{
    // Add outgoing edges
    for (int i = 0; i < m_Size; i++)
    {
        if (m_Mat[vertex][i] != 0)
        {
            (*m)[i] = m_Mat[vertex][i];
        }
    }
}

void MatrixGraph::insertEdge(int from, int to, int weight)
{
    // Insert edge weight
    m_Mat[from][to] = weight;
}

bool MatrixGraph::printGraph(ofstream *fout)
{
    if (!fout || !fout->is_open())
    {
        return false;
    }

    // Print matrix
    (*fout) << "    ";
    for (int j = 0; j < m_Size; j++)
        (*fout) << "[" << j << "] ";
    (*fout) << "\n";

    for (int i = 0; i < m_Size; i++)
    {
        (*fout) << "[" << i << "] ";
        for (int j = 0; j < m_Size; j++)
        {
            (*fout) << m_Mat[i][j] << "   ";
        }
        (*fout) << "\n";
    }
    return true;
}