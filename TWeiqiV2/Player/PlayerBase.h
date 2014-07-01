#pragma once

#include "BitBoard.h"

template <typename TBoard>
class PlayerBase
{
public:
	PlayerBase() {}
	virtual ~PlayerBase() {}

	virtual int Play(int color, const TBoard* KOCheck, int KOLength = 2) = 0;

protected:

	TBoard m_currentPosition;

};

