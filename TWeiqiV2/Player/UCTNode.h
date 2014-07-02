#pragma once

#include <list>
#include <map>
#include <vector>

#include "BitBoard.h"

using namespace std;

template <typename TBoard>
class UCTNode
{
private:
	TBoard m_board;
	int m_color;
	uint64_t m_N;
	double m_Q;

	TVKey m_key;
	list<UCTNode*> m_parents;
	map<int, UCTNode*> m_children;

	list<int> m_legalMoves;

public:
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
		return m_children.begin()->second;
	}

	int BestMove()
	{
		return m_children.begin()->first;
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

private:

};
