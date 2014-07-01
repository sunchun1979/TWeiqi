#pragma once

#include <unordered_map>
#include <map>

#include "PlayerBase.h"
#include "UCTNode.h"

template <typename TBoard>
class PlayerUCT : public PlayerBase<TBoard>
{
private:

	map<TVKey, UCTNode<TBoard>*> boardDict;

	UCTNode<TBoard>* root;

public:
	PlayerUCT(TBoard board)
	{
		m_currentPosition = board;
		root = new UCTNode<TBoard>(board);
		boardDict.insert(std::pair<TVKey, UCTNode<TBoard>*>(root->GetKey(), root));
	}

	~PlayerUCT()
	{
		for each (auto node in boardDict)
		{
			delete node.second;
		}
	}

	virtual int Play(int color, const TBoard* KOCheck, int KOLength = 2)
	{
		// Random play
		int candidates = m_currentPosition.GetNumLegalPositions(color);
		int move = m_currentPosition.GetLegalMoveByIndex(color, rand() % candidates);
		bool gameEnd = false;
		while (!m_currentPosition.Move(move, color, &KOCheck[color]))
		{
			if (candidates == 1)
			{
				gameEnd = true;
				break;
			}
			m_currentPosition.MarkMoveIllegal(color, move);
			if (gameEnd = m_currentPosition.EndGameCheck())
			{
				break;
			}else
			{
				candidates = m_currentPosition.GetNumLegalPositions(color);
				move = m_currentPosition.GetLegalMoveByIndex(color, rand() % candidates);
			}
		}
		if (!gameEnd)
		{
			return move;
		}else
		{
			return -1;
		}
	}
};
