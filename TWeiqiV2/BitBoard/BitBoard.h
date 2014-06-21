#pragma once
#include <stdio.h>
#include <iostream>
#include <list>
#include <vector>

#include "BitArray.h"
#include "BitArray64.h"
#include "Constants.h"

using namespace std;

//template<typename TBitArray<N>>
template <template<int N> class TBitArray, int N>
class BitBoard
{
	struct TBitGroup
	{
		TBitArray<N> stones;
		TBitArray<N> liberty;
	};

	typedef typename list<TBitGroup*>::iterator Titer;
	typedef typename list<TBitGroup*>::const_iterator TCiter;

protected:

	int m_Size;
	TBitArray<N>* m_stones[2];
	TBitArray<N>* m_emptyStones;
	list<TBitGroup*> m_groups[2];

	TBitArray<N>* m_legal[2];

private:

	//BitBoard();

public:

	BitBoard()
	{
		m_Size = N;
		for (int c = 0; c < 2; c++)
		{
			m_stones[c] = new TBitArray<N>();
			m_stones[c]->SetAll(false);
			m_legal[c] = new TBitArray<N>();
			m_legal[c]->SetAll(true);
		}
		m_emptyStones = new TBitArray<N>();
		m_emptyStones->SetAll(true);
	}

	BitBoard(const BitBoard& other)
	{
		m_Size = other.m_Size;
		for (int c = 0; c < 2; c++)
		{
			m_stones[c] = new TBitArray<N>(*other.m_stones[c]);
			for(TCiter it = other.m_groups[c].begin(); it != other.m_groups[c].end(); ++it)
			{
				m_groups[c].push_back(new TBitGroup(*(*it)));
			}
		}
		m_emptyStones = new TBitArray<N>(*other.m_emptyStones);
	}

	~BitBoard()
	{
		for (int c = 0; c < 2; c++)
		{
			delete m_stones[c];
			delete m_legal[c];
			for(Titer it = m_groups[c].begin(); it != m_groups[c].end(); ++it)
			{
				delete *it;
			}
			m_groups[c].clear();
		}
		delete m_emptyStones;
	}

	BitBoard& operator = (const BitBoard& other)
	{
		m_Size = other.m_Size;
		for (int c = 0; c < 2; c++)
		{
			*m_stones[c] = *other.m_stones[c];
		}
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

	bool Move(int i, int j, int color, BitBoard* cmp = nullptr)
	{
		return Move(GetMove(i,j), color, cmp);
	}

	bool Move(int move, int color, BitBoard* cmp = nullptr)
	{
		if (!m_emptyStones->Get(move))
		{
			return false;
		}
		TBitGroup newMove;
		newMove.stones.Set1(move);
		newMove.liberty.SetAll(false);
		GetLiberty(&newMove.liberty, move);

		TBitGroup* newMerged = new TBitGroup();
		list<Titer> oldGroups;
		bool hasMerged = Merge(*newMerged, oldGroups, m_groups[color], newMove);
		list<Titer> capturedStones, selfCapture;
		bool hasCaptured = CheckCapture(capturedStones, m_groups[1-color], newMerged->stones);
		if (!hasCaptured)
		{
			//cout << "newMerged : " << newMerged->liberty.ToPositionString() << endl;
			//cout << "stones : " << m_stones[1-color]->ToPositionString() << endl;
			if (!newMerged->liberty.HasTrue())
			{
				return false;
			}
			if (CheckCaptureEach(*newMerged, *m_stones[1-color]))
			{
				return false;
			}
		}

		*m_stones[color] |= newMove.stones;
		if (hasCaptured && (cmp!=nullptr)) // simple ko check
		{
			if (*m_stones[color] == *cmp->m_stones[color])
			{
				*m_stones[color] ^= newMove.stones;
				return false;
			}
		}

		newMerged->liberty &= *m_emptyStones;

		if (hasMerged)
		{
			for (list<Titer>::iterator it = oldGroups.begin(); it != oldGroups.end(); ++it)
			{
				delete *(*it);
				m_groups[color].erase(*it);
			}
		}

		m_groups[color].push_back(newMerged);
		*m_emptyStones ^= newMove.stones;

		if (hasCaptured)
		{
			TBitArray<N> capturedStonesAll;
			TBitArray<N> capturedNeighbors;
			for (auto it = capturedStones.begin(); it != capturedStones.end(); ++it)
			{
				capturedStonesAll |= (*(*it))->stones;
				AddLiberty(capturedNeighbors, (*(*it))->stones);
				*m_stones[1-color] ^= ((*(*it))->stones);
				m_groups[1-color].erase(*it);
			}
			*m_emptyStones |= capturedStonesAll;
			for (auto it = m_groups[color].begin(); it != m_groups[color].end(); ++it)
			{
				if (capturedNeighbors.Intersects((*it)->stones))
				{
					(*it)->liberty.SetAll(false);
					AddLiberty((*it)->liberty, (*it)->stones);
					(*it)->liberty &= (*m_emptyStones);
				}
			}
		}

		for (auto it = m_groups[1-color].begin(); it != m_groups[1-color].end(); ++it)
		{
			if (newMove.stones.Intersects((*it)->liberty))
			{
				(*it)->liberty.XorTrue(newMove.stones);
			}
		}

		for (int c = 0; c < 2; c++)
		{
			*m_legal[c] = *m_emptyStones;
		}

		return true;
	}

	int GetNumLegalPositions(int color)
	{
		return m_legal[color]->GetNumOfOnes();
	}

	int GetLegalMoveByIndex(int color, int n)
	{
		return m_legal[color]->GetNthOne(n);
	}

	void MarkMoveIllegal(int color, int move)
	{
		m_legal[color]->Set0(move);
	}

	int GetNumEmptyPositions()
	{
		return m_emptyStones->GetNumOfOnes();
	}

	int GetMoveByIndex(int n)
	{
		return m_emptyStones->GetNthOne(n);
	}

	bool EndGameCheck()
	{
		return m_legal[0]->Intersects(*m_legal[1]);
	}

private:

	bool Merge(TBitGroup& newMerged, list<Titer>& oldGroups, list<TBitGroup*>& groups, const TBitGroup& newMove)
	{
		newMerged = newMove;
		for(auto it = groups.begin(); it != groups.end(); ++it)
		{
			if (newMove.stones.Intersects((*it)->liberty))
			{
				oldGroups.push_back(it);
				newMerged.stones |= (*it)->stones;

				newMerged.liberty |= (*it)->liberty;
				newMerged.liberty.XorTrue((*it)->stones);
				newMerged.liberty.XorTrue(newMove.stones);
			}
		}
		return (oldGroups.size() > 0);
	}

	bool CheckCapture(list<Titer>& capturedGroups, list<TBitGroup*>& liberties, TBitArray<N>& stones)
	{
		bool ret = false;
		for (auto it = liberties.begin(); it != liberties.end(); ++it)
		{
			if (CheckCaptureEach(*(*it), stones))
			{
				ret = true;
				capturedGroups.push_back(it);
			}
		}
		return ret;
	}

	bool CheckCaptureEach(TBitGroup& liberty, TBitArray<N>& stones)
	{
		if (stones.Intersects(liberty.liberty))
		{
			if (liberty.liberty.XorTrueCheck(stones))
				return false;
			else
				return true;
		}
		return false;
	}

	int GetMove(int i, int j)
	{
		return i*m_Size + j;
	}

	void AddLiberty(TBitArray<N>& liberty, TBitArray<N>& stones)
	{
		for (int i = 0; i < m_Size * m_Size; i++)
		{
			if (stones.Get(i))
			{
				GetLiberty(&liberty, i);
			}
		}
	}

	void GetLiberty(TBitArray<N>* liberty, int move)
	{
		GetLibertyGeneric(liberty, move);
	}

	void GetLibertyGeneric(TBitArray<N>* liberty, int move)
	{
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

	void DebugCode(string prefix)
	{
		cout << "==== " << prefix << " ====" << endl;
		for (int c = 0; c < 2; c++)
		{
			cout << "color = " << c << endl;
			cout << m_stones[c]->ToPositionString() << endl;
			for (auto it = m_groups[c].begin(); it != m_groups[c].end(); ++it)
			{
				cout << "stone " << (*it)->stones.ToPositionString() << endl;
				cout << "liberty " << (*it)->liberty.ToPositionString() << endl;
			}
			cout << endl;
		}
	}
};