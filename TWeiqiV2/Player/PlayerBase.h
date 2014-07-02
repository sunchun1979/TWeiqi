#pragma once

#include "BitBoard.h"

template <typename TBoard>
class PlayerBase
{
public:
	PlayerBase(TBoard position, int color) : m_currentPosition(position), m_color(color)
	{
	}

	virtual ~PlayerBase() {}

	virtual int Play(int color, const TBoard* KOCheck, int KOLength = 2) = 0;

protected:

	TBoard m_currentPosition;
	int m_color;

};

