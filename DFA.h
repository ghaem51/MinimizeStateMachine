#pragma once
#include <iostream>

using namespace std;

namespace dfa
{
	class NodeTransition
	{
	private:
		int state = -1;
		char alphabet = '#';
	public:

		NodeTransition(int state = -1, char alphabet = '#')
		{
			this->state = state;
			this->alphabet = alphabet;
		}
		int getState()
		{
			return state;
		}

		void setState(int state)
		{
			this->state = state;
		}

		void setAlphabet(char alphabet)
		{
			this->alphabet = alphabet;
		}

		char getAlphabet()
		{
			return alphabet;
		}

		void operator =(const NodeTransition &other)
		{
			this->state = other.state;
			this->alphabet = other.alphabet;
		}

		bool operator ==(const NodeTransition &other)
		{
			return (this->state == other.state && this->alphabet == other.alphabet);
		}
	};
	class NodeInfo
	{
	private:
		int state = -1;
		NodeTransition * nextStates = nullptr;
		int sizeOfNextNodes = 0;
		bool isEndState = false;
	public:
		NodeInfo(int state = -1)
		{
			this->state = state;
		}

		int getState()
		{
			return state;
		}

		bool addNewTransition(const NodeTransition &newState)
		{
			if (sizeOfNextNodes == 0)
			{
				nextStates = new NodeTransition[1];
				nextStates[0] = newState;
				sizeOfNextNodes++;
				return true;
			}
			else if (sizeOfNextNodes>0)
			{
				for (int i = 0; i < sizeOfNextNodes; i++)
				{
					if (nextStates[i] == newState)
						return false;
				}
				NodeTransition *temp = nextStates;
				nextStates = new NodeTransition[sizeOfNextNodes + 1];
				for (int i = 0; i < sizeOfNextNodes; i++)
				{
					nextStates[i] = temp[i];
				}
				nextStates[sizeOfNextNodes] = newState;
				delete temp;
				sizeOfNextNodes++;
				return true;
			}
			else
			{
				cout << "some fukcing problem occurs wtf !" << endl;
				return false;
			}
		}

		int findNextNodeByAlphabet(char alphabet)
		{
			for (int i = 0; i < sizeOfNextNodes; i++)
			{
				if (nextStates[i].getAlphabet() == alphabet)
					return nextStates[i].getState();
			}

			return -1;
		}

		void setAsEndState()
		{
			isEndState = true;
		}

		bool isAnEndState()
		{
			return isEndState;
		}

		~NodeInfo()
		{
			if (nextStates != nullptr)
			{
				delete[] nextStates;
			}
		}

		void operator = (const NodeInfo &other)
		{
			this->state = other.state;
			this->sizeOfNextNodes = other.sizeOfNextNodes;
			this->isEndState = other.isEndState;
			if (nextStates != nullptr)
			{
				delete[] nextStates;
			}
			nextStates = new NodeTransition[sizeOfNextNodes];
			for (int i = 0; i < other.sizeOfNextNodes; i++)
			{
				nextStates[i] = other.nextStates[i];
			}
		}

		void print()
		{
			cout << "============================Node" << state << "============================" << endl;
			for (int i = 0; i < sizeOfNextNodes; i++)
			{
				cout << "-----" << nextStates[i].getAlphabet() << "-----> " << nextStates[i].getState() << endl;
			}

			if (isEndState)
			{
				cout << "End STATE  " << endl;
			}
			cout << "=============================================================" << endl;
		}
	};


	class Graph
	{
	private:
		NodeInfo *inforMationOfNodes = nullptr;
		int sizeOfNodes = -1;
		int sizeOfAlphabets;
		char *alphabets;
		void checkPairs(int**pairs, int*check, int sizeOfPairs)
		{
		firstLoop:	
			for (int i = 0; i < sizeOfPairs; i++)
			{
				if (check[i] == -1 || check[i] == 2)
				{
					checkPair(pairs, check, sizeOfPairs, i);
				}
			}
			for (int i = 0; i < sizeOfPairs; i++)
			{
				if (check[i] == -1 || check[i] == 2)
				{
					goto firstLoop;
				}
			}
			
		}
		void checkPair(int**pairs, int*check, int sizeOfPairs, int index)
		{
			check[index] = 2;
			int * pair = pairs[index];
			bool is = false;
			int count = 0;
			for (int i = 0; i < sizeOfAlphabets; i++)
			{
				char alphabet = alphabets[i];
				int dest[2];
				dest[0] = inforMationOfNodes[pair[0]].findNextNodeByAlphabet(alphabet);
				dest[1] = inforMationOfNodes[pair[1]].findNextNodeByAlphabet(alphabet);
				if (dest[0] == dest[1])
				{
					count++;
					continue;
				}
				else if((dest[0]==pair[0]&&dest[1]==pair[1])||(dest[0] == pair[1] && dest[1] == pair[0]))
				{
					count++;
					continue;
				}
				else if ((inforMationOfNodes[dest[0]].isAnEndState() && !inforMationOfNodes[dest[1]].isAnEndState()) ||
					(!inforMationOfNodes[dest[0]].isAnEndState() && inforMationOfNodes[dest[1]].isAnEndState()))
				{
					check[index] = 0;
					return;
				}
				else
				{
					int selectedIndex = -1;
					for (int i = 0; i < sizeOfPairs; i++)
					{
						if ((pairs[i][0] == dest[0] && pairs[i][1] == dest[1])||
							(pairs[i][0] == dest[1] && pairs[i][1] == dest[0]))
						{
							selectedIndex = i;
							break;
						}
					}
					//its never must be -1 be care about it kido
					if (check[selectedIndex] == -1)
					{
						//it means its just not checked check it before please !
						checkPair(pairs, check, sizeOfPairs, selectedIndex);
					}
					else if (check[selectedIndex] == 2)
					{

						// handle this please kido
						continue;
					}
					//after this check of selected index must be valid if not valid your algorithm is shit kido !
					if (check[selectedIndex] == 1)
					{
						count++;
						continue;
					}
					else if (check[selectedIndex] == 0)
					{
						check[index] = 0;
						return;
					}

				}
			}
			if(count==sizeOfAlphabets)
				check[index] = 1;
			return;
		}
	public:

		Graph(int sizeOfNodes = 0, int sizeOfAlphabets = 0, char * alphabets = nullptr)
		{
			this->sizeOfNodes = sizeOfNodes;
			inforMationOfNodes = new NodeInfo[sizeOfNodes];
			this->sizeOfAlphabets = sizeOfAlphabets;
			if (alphabets != nullptr)
			{
				this->alphabets = new char[sizeOfAlphabets];
				for (int i = 0; i < sizeOfAlphabets; i++)
				{
					this->alphabets[i] = alphabets[i];
				}

				delete[] alphabets;
			}

		}

		~Graph()
		{
			if (inforMationOfNodes != nullptr)
			{
				delete[] inforMationOfNodes;
			}

			if (alphabets != nullptr)
			{
				delete[] alphabets;
			}
		}

		bool addNode(NodeInfo &info)
		{
			if (info.getState() >= sizeOfNodes)
				return false;
			inforMationOfNodes[info.getState()] = info;
			return true;
		}

		void print()
		{
			for (int i = 0; i < sizeOfNodes; i++)
			{
				inforMationOfNodes[i].print();
			}
		}

		void minimize()
		{
			while (true)
			{
				//this section just craete paris and tables to check if pairs are valid
				int ** pairs = nullptr;
				int * check;
				int sizeOfPairs = 0;
				for (int i = 0; i < sizeOfNodes; i++)
				{
					for (int j = i + 1; j < sizeOfNodes; j++)
					{
						if ((inforMationOfNodes[i].isAnEndState() && inforMationOfNodes[j].isAnEndState())
							|| (!inforMationOfNodes[i].isAnEndState() && !inforMationOfNodes[j].isAnEndState()))
						{
							if (sizeOfPairs == 0)
							{
								pairs = new int*[1];
								pairs[0] = new int[2];
								pairs[0][0] = inforMationOfNodes[i].getState();
								pairs[0][1] = inforMationOfNodes[j].getState();
								sizeOfPairs++;
							}
							else
							{
								int ** temp = pairs;
								pairs = new int*[sizeOfPairs + 1];
								for (int i = 0; i < sizeOfPairs; i++)
								{
									pairs[i] = new int[2];
									pairs[i][0] = temp[i][0];
									pairs[i][1] = temp[i][1];
								}

								//this section just for free memory , no memory leak kido !
								for (int i = 0; i < sizeOfPairs; i++)
								{
									delete[] temp[i];
								}
								delete[] temp;
								//this section just for free memory , no memory leak kido !
								pairs[sizeOfPairs] = new int[2];
								pairs[sizeOfPairs][0] = inforMationOfNodes[i].getState();
								pairs[sizeOfPairs][1] = inforMationOfNodes[j].getState();
								sizeOfPairs++;
							}
						}
					}
				}
				if (sizeOfPairs == 0)
				{
					return;
				}
				check = new int[sizeOfPairs];
				for (int i = 0; i < sizeOfPairs; i++)
				{
					check[i] = -1;
				}
				//end of section
				checkPairs(pairs, check, sizeOfPairs);
				bool isNewPairFound = false;
				for (int i = 0; i < sizeOfNodes; i++)
				{
					if (check[i] == 1)
					{
						isNewPairFound = true;
						break;
					}
				}
				if (isNewPairFound)
				{
					int *nodesPairGroup = new int[sizeOfNodes];
					for (int i = 0; i < sizeOfNodes; i++)
					{
						nodesPairGroup[i] = -1;
					}
					int group = 0;
					int newSizeOfNodes = 0;
					//in this section we just make valid pairs into just one node
					for (int i = 0; i < sizeOfPairs; i++)
					{
						if (check[i] == 1)
						{
							//it seems this 2 nodes can join togther
							if (nodesPairGroup[pairs[i][0]] != -1)
							{
								nodesPairGroup[pairs[i][1]] = nodesPairGroup[pairs[i][0]];
							}
							else if (nodesPairGroup[pairs[i][1]] != -1)
							{
								nodesPairGroup[pairs[i][0]] = nodesPairGroup[pairs[i][1]];
							}
							else
							{
								nodesPairGroup[pairs[i][0]] = group;
								nodesPairGroup[pairs[i][1]] = group;
								newSizeOfNodes++;
								group++;
							}
						}
						else
						{
							if (nodesPairGroup[pairs[i][0]] == -1)
							{
								nodesPairGroup[pairs[i][0]] = group;
								group++;
								newSizeOfNodes++;
							}
							if (nodesPairGroup[pairs[i][1]] == -1)
							{
								nodesPairGroup[pairs[i][1]] = group;
								group++;
								newSizeOfNodes++;
							}
						}
					}
					NodeInfo *newNodes = new NodeInfo[newSizeOfNodes];
					for (int i = 0; i < newSizeOfNodes; i ++)
					{
						NodeInfo info(i);
						int groupAgent = -1;
						for (int j = 0; j < sizeOfNodes; j++)
						{
							if (nodesPairGroup[j] == i)
							{
								groupAgent = j;
								break;
							}
						}
						for (int i = 0; i < sizeOfAlphabets; i++)
						{
							char alphabet = alphabets[i];
							int nextNode = nodesPairGroup[inforMationOfNodes[groupAgent].findNextNodeByAlphabet(alphabet)];
							NodeTransition transition(nextNode, alphabet);
							info.addNewTransition(transition);
						}
						if (inforMationOfNodes[groupAgent].isAnEndState())
						{
							info.setAsEndState();
						}
						newNodes[i] = info;
					}
					delete[] inforMationOfNodes;
					inforMationOfNodes = newNodes;
					sizeOfNodes = newSizeOfNodes;

					delete[] nodesPairGroup;
					//end of section

					for (int i = 0; i < sizeOfPairs; i++)
					{
						delete[] pairs[i];
					}
					delete[] pairs;
					delete[] check;
					//return;
				}
				else
				{
					for (int i = 0; i < sizeOfPairs; i++)
					{
						delete[] pairs[i];
					}
					delete[] pairs;
					delete[] check;
					return;
				}
			}
		}
	};
}