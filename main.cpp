// MinimizeDFA.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include <iostream>
#include "DFA.h"
#include <fstream>
#include <string>


using namespace std;
using namespace dfa;


Graph * createGraphFromFile(const char * filePath)
{
	ifstream myfile;
	myfile.open(filePath);
	if (myfile.is_open())
	{
		int numberOfAlphabets;
		char * alphabets;
		myfile >> numberOfAlphabets;
		alphabets = new char[numberOfAlphabets];
		for (int i = 0; i < numberOfAlphabets; i++)
		{
			myfile >> alphabets[i];
		}
		int numberOfNodes;
		myfile >> numberOfNodes;
		Graph *ptr = new Graph(numberOfNodes, numberOfAlphabets, alphabets);
		NodeInfo *nodes = new NodeInfo[numberOfNodes];
		for (int i = 0; i < numberOfNodes; i++)
		{
			NodeInfo inf(i);
			nodes[i] = inf;
		}
		int numberOfEndStates;
		myfile >> numberOfEndStates;
		for (int i = 0; i < numberOfEndStates; i++)
		{
			int number;
			myfile >> number;
			nodes[number].setAsEndState();
		}
		int numberOfTransitions;
		myfile >> numberOfTransitions;
		for (int i = 0; i < numberOfTransitions; i++)
		{
			int source;
			int dest;
			char alphabet;
			myfile >> source;
			myfile >> alphabet;
			myfile >> dest;

			NodeTransition transition(dest, alphabet);
			nodes[source].addNewTransition(transition);
		}

		for (int i = 0; i < numberOfNodes; i++)
		{
			ptr->addNode(nodes[i]);
		}

		delete[] nodes;
		myfile.close();
		return ptr;
	}
	else
	{
		cout << "some error occurs when reading the file in path : " << filePath << endl;
		return nullptr;
	}
}


int main()
{
	string filePath;
	cout << "enter file path (Ex:  D:/path/to/file.txt) : ";
	getline(cin, filePath);
	Graph * graph = createGraphFromFile(filePath.data());
	if (graph == nullptr)
	{
		cout << "graph not created ! " << endl;
		system("pause");
		return 0;
	}
	cout << "Graph before minimize : "<<endl;
	graph->print();
	graph->minimize();
	cout << "*************************************************************" << endl;
	cout << "Graph after minimize : " << endl;
	graph->print();
	system("pause");
	delete graph;
    return 0;
}

