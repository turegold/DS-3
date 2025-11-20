#ifndef _LIST_H_
#define _LIST_H_

#include "Graph.h"

class ListGraph : public Graph
{
private:
	map<int, int> *m_List; // (도착점, 가중치)를 저장하는 map을 저장하는 배열을 가리키는 포인터
	vector<int> *kw_graph;

public:
	ListGraph(bool type, int size);
	~ListGraph();

	void getAdjacentEdges(int vertex, map<int, int> *m);
	void getAdjacentEdgesDirect(int vertex, map<int, int> *m);
	void insertEdge(int from, int to, int weight);
	bool printGraph(ofstream *fout);
};

#endif