#pragma once

#include <list>
#include <map>
#include <vector>
#include <limits>
#include <utility>

#include "BitBoard.h"

using namespace std;

template <typename TBoard>
class UCTNode
{
private:
	TBoard m_board;
	int m_color;
	int m_move;

	TVKey m_key;
	list<UCTNode*> m_parents;
	map<int, UCTNode*> m_children;
	pair<int, UCTNode*> m_currentBestChild;

public:

	list<int> m_legalMoves;

public:
	uint64_t m_N;
	double m_Q;

	UCTNode(TBoard b, int c) : m_move(-1), m_board(b), m_color(c), m_N(0), m_Q(0)
	{
	}

	~UCTNode()
	{
	}

	void Reset(TBoard& b)
	{
		m_board.Clone(b);
		Reset();
	}

	void Reset()
	{
		m_board.AssignRawVector(m_key);
		int n = m_board.GetNumLegalPositions(m_color);
		m_legalMoves.clear();
		//cout << m_board.ToString() << endl;
		//cout << "mcolor n " << m_color << " " << n << endl;
		for (int i = 0; i < n; i++)
		{
			int p = m_board.GetLegalMoveByIndex(m_color, i);
			//cout << "push back p = " << p << endl;
			m_legalMoves.push_back(p);
		}
	}

	const TVKey& GetKey() const
	{
		return m_key;
	}

	bool FullyExpanded()
	{
		return (m_legalMoves.size() == 0);
	}

	bool EndGameCheck()
	{
		return m_board.EndGameCheck();
	}

	UCTNode* Expand(int& move)
	{
		if (!FullyExpanded())
		{
			move = m_legalMoves.front();
			UCTNode* ret = new UCTNode(m_board, 1-m_color);
			ret->m_move = move;
			ret->m_board.Move(move, 1-m_color);
			ret->Reset();
			ret->m_parents.push_back(this);
			return ret;
		}else
		{
			return nullptr;
		}
	}

	UCTNode* BestChild()
	{
		double sign = m_color * 2 - 1;
		double bestUCT = sign * std::numeric_limits<double>::max();
		for (auto child : m_children)
		{
			double cUCT = child.second->GetUCT(m_N, 0.717);
			if (sign * cUCT < sign * bestUCT)
			{
				bestUCT = cUCT;
				m_currentBestChild = child;
			}
		}
		//if (m_currentBestChild.second == nullptr)
		//{
		//	cout << "Hehe" << endl;
		//	double bestUCT1 = sign * std::numeric_limits<double>::max();
		//	for (auto child1 : m_children)
		//	{
		//		double cUCT1 = child1.second->GetUCT(m_N, 0.717);
		//		cout << cUCT1 << endl;
		//		if (sign * cUCT1 > sign * bestUCT1)
		//		{
		//			cout << "shoudl hit once" << endl;
		//			bestUCT = cUCT1;
		//			m_currentBestChild = child1;
		//		}
		//	}
		//	getchar();
		//}
		return m_currentBestChild.second;
	}

	int BestMove()
	{
		BestChild();
		return m_currentBestChild.first;
	}

	void AddParent(UCTNode* parent)
	{
		m_parents.push_back(parent);
	}

	void AddChild(int move, UCTNode* child)
	{
		m_legalMoves.remove(move);
		if (this != child)
		{
			m_children[move] = child;
		}
	}

	list<UCTNode*> GetParents()
	{
		return m_parents;
	}

	void PrintChildren()
	{
		for (auto child : m_children)
		{
			cout << "child " << child.first << endl;
		}
	}

	double GetUCT(uint64_t& pN, double c)
	{
		return m_Q/m_N + c * sqrt(2*log(pN)/m_N);
	}

	TBoard& GetBoard()
	{
		return m_board;
	}

	int GetMove()
	{
		return m_move;
	}

	int GetColor()
	{
		return m_color;
	}

private:

};
