#include "MatrixGraph.h"
#include <iostream>
#include <vector>
#include <string>

MatrixGraph::MatrixGraph(bool type, int size) : Graph(type, size)
{
    // 2차원 배열 (size x size ) 동적 할당
    m_Mat = new int *[m_Size];
    for (int i = 0; i < m_Size; i++)
    {
        m_Mat[i] = new int[m_Size];
        for (int j = 0; j < m_Size; j++)
        {
            m_Mat[i][j] = 0; // 초기값 0으로 세팅
        }
    }
}

MatrixGraph::~MatrixGraph()
{
    // 2차원 배열 메모리 해제
    for (int i = 0; i < m_Size; i++)
    {
        delete[] m_Mat[i];
    }
    delete[] m_Mat;
}

void MatrixGraph::getAdjacentEdges(int vertex, map<int, int> *m)
{
    // 1. 정방향 간선
    for (int i = 0; i < m_Size; i++)
    {
        if (m_Mat[vertex][i] != 0)
        {
            (*m)[i] = m_Mat[vertex][i];
        }
    }

    // 2. 역방향 간선 탐색
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
    m_Mat[from][to] = weight;
}

bool MatrixGraph::printGraph(ofstream *fout)
{
    if (!fout || !fout->is_open())
        return false;

    // Column index header
    (*fout) << "    ";
    for (int j = 0; j < m_Size; j++)
        (*fout) << "[" << j << "] ";
    (*fout) << "\n";

    // Actual matrix
    for (int i = 0; i < m_Size; i++)
    {
        (*fout) << "[" << i << "] ";
        for (int j = 0; j < m_Size; j++)
            (*fout) << m_Mat[i][j] << "   ";
        (*fout) << "\n";
    }
    return true;
}