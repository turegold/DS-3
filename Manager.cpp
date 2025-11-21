#include "Manager.h"
#include "GraphMethod.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

Manager::Manager()
{
	graph = nullptr;
	fout.open("log.txt", ios::trunc);
	load = 0; // Anything is not loaded
}

Manager::~Manager()
{
	if (load) // if graph is loaded, delete graph
		delete graph;
	if (fout.is_open()) // if fout is opened, close file
		fout.close();	// close log.txt File
}

void Manager::run(const char *command_txt)
{
	ifstream fin;						 // Command File Input File Stream
	fin.open(command_txt, ios_base::in); // File open with read mode

	if (!fin)
	{ // If command File cannot be read, Print error
		fout << "command file open error" << endl;
		return; // Return
	}

	string cmd;

	while (fin >> cmd)
	{
		if (cmd == "LOAD")
		{
			string filename;
			fin >> filename;
			LOAD(filename.c_str());
		}
		else if (cmd == "PRINT")
		{
			PRINT();
		}
		else if (cmd == "BFS")
		{
			char option;
			int vertex;
			fin >> option >> vertex;
			mBFS(option, vertex);
		}
		else if (cmd == "DFS")
		{
			char option;
			int vertex;
			fin >> option >> vertex;
			mDFS(option, vertex);
		}
		else if (cmd == "DIJKSTRA")
		{
			char option;
			int vertex;
			fin >> option >> vertex;
			mDIJKSTRA(option, vertex);
		}
		else if (cmd == "KRUSKAL")
		{
			mKRUSKAL();
		}
		else if (cmd == "BELLMANFORD")
		{
			char option;
			int s, e;
			fin >> option >> s >> e;
			mBELLMANFORD(option, s, e);
		}
		else if (cmd == "FLOYD")
		{
			char option;
			fin >> option;
			mFLOYD(option);
		}
		else if (cmd == "CENTRALITY")
		{
			mCentrality();
		}
		else
		{
			// 잘못된 명령어 → 에러 출력
			printErrorCode(200);
		}
	}

	fin.close();
	return;
}

bool Manager::LOAD(const char *filename)
{

	ifstream fin(filename);
	if (!fin.is_open())
	{
		printErrorCode(100); // file open error
		return false;
	}

	// 기존 그래프가 있으면 삭제
	if (load && graph != nullptr)
	{
		delete graph;
		graph = nullptr;
		load = 0;
	}

	char typeChar;
	int size;

	fin >> typeChar; // L or M
	fin >> size;	 // graph size

	// 예외 처리
	if (!fin || (typeChar != 'L' && typeChar != 'M') || size <= 0)
	{
		printErrorCode(100);
		return false;
	}

	// 그래프 객체 생성
	if (typeChar == 'L')
	{
		graph = new ListGraph(true, size);
	}
	else
	{
		graph = new MatrixGraph(false, size);
	}

	// L 타입: 인접 리스트 형태
	if (typeChar == 'L')
	{
		string line;
		getline(fin, line); // 첫 줄 개행 제거

		int next_start = -1;

		for (int cur = 0; cur < size; cur++)
		{
			string startLine;

			// 출발 정점 읽기
			if (next_start == -1)
			{
				if (!getline(fin, startLine))
				{
					printErrorCode(100);
					return false;
				}
				next_start = stoi(startLine);
			}

			int start = next_start;
			next_start = -1;

			// 간선 정보 읽기
			while (true)
			{
				streampos pos = fin.tellg();
				if (!getline(fin, line))
					break;

				if (line.size() == 0)
					continue;

				stringstream ss(line);
				vector<int> nums;
				int x;
				while (ss >> x)
					nums.push_back(x);

				if (nums.size() == 1)
				{
					next_start = nums[0];
					// 줄 되돌리지 않고, 그대로 다음 cur에서 사용
					break;
				}
				else if (nums.size() == 2)
				{
					int to = nums[0];
					int weight = nums[1];
					graph->insertEdge(start, to, weight);
				}
				else
				{
					printErrorCode(100);
					return false;
				}
			}
		}
	}
	else
	{
		// M 타입: 인접 행렬 형태
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				int weight;
				if (!(fin >> weight))
				{
					printErrorCode(100);
					return false;
				}

				if (weight != 0)
				{
					graph->insertEdge(i, j, weight);
				}
			}
		}
	}

	load = 1;

	fout << "========LOAD========" << endl;
	fout << "Success" << endl;
	fout << "====================" << endl;
	fout << endl;
	return true;
}

bool Manager::PRINT()
{
	// 저장된 그래프가 없는 경우 에러 출력
	if (!load || graph == nullptr)
	{
		printErrorCode(200);
		return false;
	}

	fout << "========PRINT========" << endl;

	// 그래프 출력
	graph->printGraph(&fout);

	fout << "====================" << endl;
	fout << endl;

	return true;
}

bool Manager::mBFS(char option, int vertex)
{
	if (!load || graph == nullptr)
	{
		printErrorCode(300);
		return false;
	}

	return BFS(graph, option, vertex, &fout);
}

bool Manager::mDFS(char option, int vertex)
{
	if (!load || graph == nullptr)
	{
		printErrorCode(400);
		return false;
	}

	return DFS(graph, option, vertex, &fout);
}

bool Manager::mDIJKSTRA(char option, int vertex)
{
}

bool Manager::mKRUSKAL()
{
	if (!load || graph == nullptr)
	{
		printErrorCode(500);
		return false;
	}
	else if (!Kruskal(graph, &fout))
	{
		printErrorCode(500);
		return false;
	}
}

bool Manager::mBELLMANFORD(char option, int s_vertex, int e_vertex)
{
}

bool Manager::mFLOYD(char option)
{
}

bool Manager::mCentrality()
{
}

void Manager::printErrorCode(int n)
{
	fout << "========ERROR=======" << endl;
	fout << n << endl;
	fout << "====================" << endl
		 << endl;
}
