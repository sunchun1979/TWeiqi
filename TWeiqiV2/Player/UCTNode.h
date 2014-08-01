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

	TVKey m_key;
	list<UCTNode*> m_parents;
	map<int, UCTNode*> m_children;
	pair<int, UCTNode*> m_currentBestChild;

	list<int> m_legalMoves;

public:
	uint64_t m_N;
	double m_Q;

	UCTNode(TBoard b, int c) : m_board(b), m_color(c), m_N(0), m_Q(0)
	{
	}

	~UCTNode()
	{
	}

	void Reset()
	{
		m_board.AssignRawVector(m_key);
		int n = m_board.GetNumLegalPositions(m_color);
		for (int i = 0; i < n; i++)
		{
			m_legalMoves.push_back(m_board.GetLegalMoveByIndex(m_color, i));
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
		return false;
	}

	UCTNode* Expand(int& move)
	{
		if (!FullyExpanded())
		{
			move = m_legalMoves.front();
			UCTNode* ret = new UCTNode(m_board, 1-m_color);
			ret->m_board.Move(move, m_color);
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
		double bestUCT = std::numeric_limits<double>::min();
		for (auto child : m_children)
		{
			double cUCT = GetUCT(m_N, 0.717);
			if (cUCT > bestUCT)
			{
				bestUCT = cUCT;
				m_currentBestChild = child;
			}
		}
		return m_currentBestChild.second;
	}

	int BestMove()
	{
		return m_currentBestChild.first;
	}

	void AddParent(UCTNode* parent)
	{
		m_parents.push_back(parent);
	}

	void AddChild(int move, UCTNode* child)
	{
		m_legalMoves.remove(move);
		m_children[move] = child;
		cout << child->m_board.ToString() << endl;
	}

	double GetUCT(uint64_t& pN, double c)
	{
		return m_Q/m_N + c * sqrt(2*log(pN)/m_N);
	}

	TBoard& GetBoard()
	{
		return m_board;
	}

	int GetColor()
	{
		return m_color;
	}

private:

};
