#include <string>
#include <list>
#include <iostream>

#include "BitBoard.h"

#pragma once

using namespace std;

template <int N>
class TGame
{
	typedef BitBoard<BitArray64, N> TBoard;
private:
	TBoard m_currentPosition;
	TBoard m_KOCheck[2];
	list<TBoard> m_history;

public:
	TGame()
	{
	}

	~TGame()
	{
	}

	void LoadSGF(string filename)
	{
	}

	int GetTotalMoves()
	{
		return m_history.size;
	}

	void MoveTo(int moveIndex)
	{
		int total = m_history.size;
		for (int i = 0; i < total - moveIndex - 1; i++)
		{
			m_history.pop_back();
		}
		m_currentPosition = m_history.back();
	}

	bool PlayFromCurrent(int move, int color)
	{
		if (m_currentPosition.Move(move, color, &m_KOCheck[color]))
		{
			m_KOCheck[color] = m_currentPosition;
			m_history.push_back(m_currentPosition);
			return true;
		}
		return false;
	}

	BitArray64<N> GetCurrentStones(int color)
	{
		return m_currentPosition.GetRawStones(color);
	}

	BitArray64<N> GetCurrentEst(int color)
	{
		throw new exception("not implmented");
	}

private:

};
