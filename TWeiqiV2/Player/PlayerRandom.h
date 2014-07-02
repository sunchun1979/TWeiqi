#pragma once

#include "PlayerBase.h"

template <typename TBoard>
class PlayerRandom : public PlayerBase<TBoard>
{
public:
	PlayerRandom(TBoard board, int color) : PlayerBase<TBoard>(board, color)
	{
	}

	~PlayerRandom() {}

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
