#include <stdio.h>
#include <iostream>
#include <list>

#include "BitArray.h"
#include "Constants.h"

using namespace std;

template<class TBitArray>
class BitBoard
{
protected:

	int m_Size;
	TBitArray* m_stones[2];
	TBitArray* m_emptyStones;
	list<TBitArray*> m_groups[2];
	list<TBitArray*> m_liberties[2];

private:
	BitBoard();

public:

	BitBoard(int size)
	{
		m_Size = size;
		for (int c = 0; c < 2; c++)
		{
			m_stones[c] = new TBitArray();
			m_stones[c]->SetAll(false);
		}
		m_emptyStones = new TBitArray();
		m_emptyStones->SetAll(true);
	}

	BitBoard(const BitBoard& other)
	{
		*this = other;
	}

	~BitBoard()
	{
		for (int c = 0; c < 2; c++)
		{
			delete m_stones[c];
			for(list<TBitArray*>::iterator it = m_groups[c].begin(); it != m_groups[c].end(); ++it)
			{
				delete *it;
			}
			m_groups[c].clear();
			for(list<TBitArray*>::iterator it = m_liberties[c].begin(); it != m_liberties[c].end(); ++it)
			{
				delete *it;
			}
			m_liberties[c].clear();
		}
		delete m_emptyStones;
	}

	BitBoard& operator = (const BitBoard& other)
	{
		m_Size = other.m_Size;
		for (int c = 0; c < 2; c++)
		{
			m_stones[c] = new TBitArray(*other.m_stones[c]);
			for(list<TBitArray*>::const_iterator it = other.m_groups[c].begin(); it != other.m_groups[c].end(); ++it)
			{
				m_groups[c].push_back(new TBitArray(*(*it)));
			}
			for(list<TBitArray*>::const_iterator it = other.m_liberties[c].begin(); it != other.m_liberties[c].end(); ++it)
			{
				m_liberties[c].push_back(new TBitArray(*(*it)));
			}
		}
		m_emptyStones = new TBitArray(*other.m_emptyStones);
		return *this;
	}

	bool operator == (const BitBoard& other)
	{
		return ((m_Size == other->m_Size) && (*m_stones[0] == *other->m_stones[0]) && (*m_stones[1] == *other->m_stones[1]));
	}

	string ToString()
	{
		string ret;
		for (int i = 0; i < m_Size; i++)
		{
			ret += " ";
			for (int j = 0; j < m_Size; j++)
			{
				if (m_stones[BLACK]->Get(GetMove(i,j)))
				{
					ret += "X ";
				}
				else if (m_stones[WHITE]->Get(GetMove(i,j)))
				{
					ret += "O ";
				}
				else
				{
					ret += ". ";
				}
			}
			ret += "\n";
		}
		return ret;
	}

	bool Move(int i, int j, int color)
	{
		return Move(GetMove(i,j), color);
	}

	bool Move(int move, int color)
	{
		if (!m_emptyStones->Get(move))
		{
			return false;
		}
		TBitArray liberty;
		GetLiberty(&liberty, move);
		TBitArray current;
		current.Set1(move);

		return true;
	}

private:
	
	void Merge(TBitArray& group)
	{
	}

	int GetMove(int i, int j)
	{
		return i*m_Size + j;
	}

	void GetLiberty(TBitArray* liberty, int move)
	{
		GetLibertyGeneric(liberty, move);
	}

	void GetLibertyGeneric(TBitArray* liberty, int move)
	{
		liberty->SetAll(false);
		if (move >= m_Size)
		{
			liberty->Set1(move - m_Size);
		}
		if (move + m_Size < m_Size*m_Size)
		{
			liberty->Set1(move + m_Size);
		}
		if (move % m_Size > 0) // to optimize with bit op
		{
			liberty->Set1(move - 1);
		}
		if (move % m_Size < m_Size - 1)
		{
			liberty->Set1(move + 1);
		}
	}
};