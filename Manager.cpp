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
		string rest;	   // For checking extra arguments
		bool valid = true; // For checking argument validity

		if (cmd == "LOAD")
		{
			string filename;
			if (!(fin >> filename))
			{
				printErrorCode(100);
				fin.clear();
				getline(fin, rest);
				continue;
			}

			getline(fin, rest);
			for (char c : rest)
			{
				if (!isspace(c))
				{
					printErrorCode(100);
					valid = false;
					break;
				}
			}
			if (!valid)
				continue;

			LOAD(filename.c_str());
		}
		else if (cmd == "PRINT")
		{
			getline(fin, rest);
			for (char c : rest)
			{
				if (!isspace(c))
				{
					printErrorCode(200);
					valid = false;
					break;
				}
			}
			if (!valid)
				continue;

			PRINT();
		}
		else if (cmd == "BFS")
		{
			char option;
			int vertex;

			if (!(fin >> option >> vertex))
			{
				printErrorCode(300);
				fin.clear();
				getline(fin, rest);
				continue;
			}

			getline(fin, rest);
			for (char c : rest)
			{
				if (!isspace(c))
				{
					printErrorCode(300);
					valid = false;
					break;
				}
			}
			if (!valid)
				continue;

			mBFS(option, vertex);
		}
		else if (cmd == "DFS")
		{
			char option;
			int vertex;

			if (!(fin >> option >> vertex))
			{
				printErrorCode(400);
				fin.clear();
				getline(fin, rest);
				continue;
			}

			getline(fin, rest);
			for (char c : rest)
			{
				if (!isspace(c))
				{
					printErrorCode(400);
					valid = false;
					break;
				}
			}
			if (!valid)
				continue;

			mDFS(option, vertex);
		}
		else if (cmd == "DIJKSTRA")
		{
			char option;
			int vertex;

			if (!(fin >> option >> vertex))
			{
				printErrorCode(600);
				fin.clear();
				getline(fin, rest);
				continue;
			}

			getline(fin, rest);
			for (char c : rest)
			{
				if (!isspace(c))
				{
					printErrorCode(600);
					valid = false;
					break;
				}
			}
			if (!valid)
				continue;

			mDIJKSTRA(option, vertex);
		}
		else if (cmd == "KRUSKAL")
		{
			getline(fin, rest);
			for (char c : rest)
			{
				if (!isspace(c))
				{
					printErrorCode(500);
					valid = false;
					break;
				}
			}
			if (!valid)
				continue;

			mKRUSKAL();
		}
		else if (cmd == "BELLMANFORD")
		{
			char option;
			int s, e;

			if (!(fin >> option >> s >> e))
			{
				printErrorCode(700);
				fin.clear();
				getline(fin, rest);
				continue;
			}

			getline(fin, rest);
			for (char c : rest)
			{
				if (!isspace(c))
				{
					printErrorCode(700);
					valid = false;
					break;
				}
			}
			if (!valid)
				continue;

			mBELLMANFORD(option, s, e);
		}
		else if (cmd == "FLOYD")
		{
			char option;

			if (!(fin >> option))
			{
				printErrorCode(800);
				fin.clear();
				getline(fin, rest);
				continue;
			}

			getline(fin, rest);
			for (char c : rest)
			{
				if (!isspace(c))
				{
					printErrorCode(800);
					valid = false;
					break;
				}
			}
			if (!valid)
				continue;

			mFLOYD(option);
		}
		else if (cmd == "CENTRALITY")
		{
			getline(fin, rest);
			for (char c : rest)
			{
				if (!isspace(c))
				{
					printErrorCode(900);
					valid = false;
					break;
				}
			}
			if (!valid)
				continue;

			mCentrality();
		}
		else if (cmd == "EXIT")
		{
			getline(fin, rest);
			EXIT();
			break;
		}
		else
		{
			getline(fin, rest);
			continue;
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
		printErrorCode(100);
		return false;
	}

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

	if (!fin || (typeChar != 'L' && typeChar != 'M') || size <= 0)
	{
		printErrorCode(100);
		return false;
	}

	// Create graph
	if (typeChar == 'L')
	{
		graph = new ListGraph(true, size);
	}
	else
	{
		graph = new MatrixGraph(false, size);
	}

	// L type
	if (typeChar == 'L')
	{
		string line;
		getline(fin, line);

		int next_start = -1;

		for (int cur = 0; cur < size; cur++)
		{
			string startLine;

			// Read start
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

			// Read edge
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
		// M type
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
	if (!load || graph == nullptr)
	{
		printErrorCode(200);
		return false;
	}

	fout << "========PRINT========" << endl;

	// Print graph
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

	// Print BFS
	return BFS(graph, option, vertex, &fout);
}

bool Manager::mDFS(char option, int vertex)
{
	if (!load || graph == nullptr)
	{
		printErrorCode(400);
		return false;
	}

	// Print DFS
	return DFS(graph, option, vertex, &fout);
}

bool Manager::mDIJKSTRA(char option, int vertex)
{
	if (!load || graph == nullptr)
	{
		printErrorCode(600);
		return false;
	}

	// if Dijkstra fails, print error
	if (!Dijkstra(graph, option, vertex, &fout))
	{
		printErrorCode(600);
		return false;
	}

	return true;
}

bool Manager::mKRUSKAL()
{
	if (!load || graph == nullptr)
	{
		printErrorCode(500);
		return false;
	}
	// if Kruskal fails, print error
	else if (!Kruskal(graph, &fout))
	{
		printErrorCode(500);
		return false;
	}
	return true;
}

bool Manager::mBELLMANFORD(char option, int s_vertex, int e_vertex)
{
	if (!load || graph == nullptr)
	{
		printErrorCode(700);
		return false;
	}

	// If bellmanford fails, print error
	if (!Bellmanford(graph, option, s_vertex, e_vertex, &fout))
	{
		printErrorCode(700);
		return false;
	}

	return true;
}

bool Manager::mFLOYD(char option)
{
	if (!load || graph == nullptr)
	{
		printErrorCode(800);
		return false;
	}

	// If Floyd fails, print error
	if (!FLOYD(graph, option, &fout))
	{
		printErrorCode(800);
		return false;
	}

	return true;
}

bool Manager::mCentrality()
{
	if (!load || graph == nullptr)
	{
		printErrorCode(900);
		return false;
	}

	// if Centrality fails, print error
	if (!Centrality(graph, &fout))
	{
		printErrorCode(900);
		return false;
	}
	return true;
}

bool Manager::EXIT()
{
	fout << "========EXIT========" << endl;
	fout << "Success" << endl;
	fout << "====================" << endl;
	fout << endl;

	return true;
}

void Manager::printErrorCode(int n)
{
	fout << "========ERROR=======" << endl;
	fout << n << endl;
	fout << "====================" << endl;
	fout << endl;
}