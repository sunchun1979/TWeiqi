#pragma once

#include "BitBoard.h"

template <typename TBoard>
class PlayerBase
{
public:
	PlayerBase(TBoard position, int color) :  m_color(color)
	{
		m_currentPosition.Clone(position);
	}

	virtual ~PlayerBase() {}

	virtual int Play(int color, const TBoard* KOCheck, int KOLength = 2) = 0;

	virtual bool Update(int move, int color)
	{
		return m_currentPosition.Move(move, color);
	}

	virtual TBoard& GetBoard()
	{
		return m_currentPosition;
	}

protected:

	TBoard m_currentPosition;
	int m_color;

};

