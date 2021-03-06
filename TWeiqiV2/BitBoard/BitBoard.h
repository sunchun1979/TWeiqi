#pragma once
#include <stdio.h>
#include <iostream>
#include <list>
#include <deque>
#include <array>
#include <vector>

#include "BitArray.h"
#include "BitArray64.h"
#include "Constants.h"

using namespace std;

typedef vector<uint64_t> TVKey;

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

	TBitGroup m_newMove;
	list<Titer> m_oldGroups;
	list<Titer> m_capturedStones;

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
			m_legal[c] = new TBitArray<N>(*other.m_legal[c]);
			for(TCiter it = other.m_groups[c].begin(); it != other.m_groups[c].end(); ++it)
			{
				m_groups[c].push_back(new TBitGroup(*(*it)));
			}
		}
		m_emptyStones = new TBitArray<N>(*other.m_emptyStones);
	}

	virtual ~BitBoard()
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
			//m_legal[c] = new TBitArray<N>(*other.m_legal[c]);
		}
		//*m_emptyStones = *other.m_emptyStones;
		return *this;
	}

	BitBoard& Clone(const BitBoard& other)
	{
		m_Size = other.m_Size;
		for (int c = 0; c < 2; c++)
		{
			*m_stones[c] = *other.m_stones[c];
			*m_legal[c] = *other.m_legal[c];
			for(Titer it = m_groups[c].begin(); it != m_groups[c].end(); ++it)
			{
				delete *it;
			}
			m_groups[c].clear();
			for(TCiter it = other.m_groups[c].begin(); it != other.m_groups[c].end(); ++it)
			{
				m_groups[c].push_back(new TBitGroup(*(*it)));
			}
		}
		*m_emptyStones = *other.m_emptyStones;
		return *this;
	}

	void Clear()
	{
		for (int c = 0; c < 2; c++)
		{
			m_stones[c]->SetAll(false);
			m_legal[c]->SetAll(true);
			for(Titer it = m_groups[c].begin(); it != m_groups[c].end(); ++it)
			{
				delete *it;
			}
			m_groups[c].clear();
		}
		m_emptyStones->SetAll(true);
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

	bool Move(int i, int j, int color, const BitBoard* cmp = nullptr)
	{
		return Move(GetMove(i,j), color, cmp);
	}

	bool Move(int move, int color, const BitBoard* cmp = nullptr)
	{
		TBitGroup* newMerged = new TBitGroup();
		bool hasMerged;
		bool hasCaptured;

		m_oldGroups.clear();
		m_capturedStones.clear();

		if(!CheckLegal(move, color, cmp, 
			newMerged, hasMerged, hasCaptured))
		{
			delete newMerged;
			return false;
		}

		newMerged->liberty &= *m_emptyStones;

		if (hasMerged)
		{
			for (list<Titer>::iterator it = m_oldGroups.begin(); it != m_oldGroups.end(); ++it)
			{
				delete *(*it);
				m_groups[color].erase(*it);
			}
		}

		m_groups[color].push_back(newMerged);
		*m_emptyStones ^= m_newMove.stones;

		if (hasCaptured)
		{
			TBitArray<N> capturedStonesAll;
			TBitArray<N> capturedNeighbors;
			for (auto it = m_capturedStones.begin(); it != m_capturedStones.end(); ++it)
			{
				capturedStonesAll |= (*(*it))->stones;
				AddLiberty(capturedNeighbors, (*(*it))->stones);
				*m_stones[1-color] ^= ((*(*it))->stones);
				delete *(*it);
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
			if (m_newMove.stones.Intersects((*it)->liberty))
			{
				(*it)->liberty.XorTrue(m_newMove.stones);
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
		return ! (m_legal[0]->Intersects(*m_legal[1]));
	}

	bool EndGameCheck2()
	{
		cout << "0: " << m_legal[0]->ToPositionString() << endl;
		cout << "1: " << m_legal[1]->ToPositionString() << endl;
		return ! (m_legal[0]->Intersects(*m_legal[1]));
	}

	const TBitArray<N> GetRawStones(int color)
	{
		return *m_stones[color];
	}

	const void AssignRawVector(TVKey& vec) const
	{
		int t = m_stones[0]->GetArrayLength();
		vec.resize(t*2);
		for (int i = 0; i < t; i++)
		{
			vec[i] = * (m_stones[0]->GetArray() + i);
		}
		for (int i = 0; i < m_stones[1]->GetArrayLength(); i++)
		{
			vec[i + t] = * (m_stones[1]->GetArray() + i);
		}
	}

	bool FinalCheckBlack()
	{
		int count[2] = {0, 0};
		for (int color = 0; color < 2; color++)
		{
			for (auto it = m_groups[color].begin(); it != m_groups[color].end(); ++it)
			{
				if ((*it)->liberty.GetNumOfOnes() > 1)
				{
					count[color] += (*it)->stones.GetNumOfOnes();
				}else
				{
					count[1-color] += (*it)->stones.GetNumOfOnes();
				}
			}
		}
		return count[0] > count[1];
	}

	static int EstimateCheck(const BitBoard& board, TBitArray<N>* blackEst, TBitArray<N>* whiteEst)
	{
		TBitArray<N> estimates[2];
		TBitArray<N> libTemp, estTemp;
		for (int c = 0; c < 2; c++)
		{
			estimates[c] = *board.m_stones[c];
			for (auto it = board.m_groups[c].begin(); it != board.m_groups[c].end(); ++it)
			{
				if ((*it)->liberty.GetNumOfOnes() == 1)
				{
					estimates[1-c] |= (*it)->stones;
					estimates[c].XorTrue((*it)->stones);
				}
			}
		}
		int32_t initInf = 0xFFFF;
		const int PATHN = 16;
		array<int32_t, N*N> influence[2];
		for (int c = 0; c < 2; c++)
		{
			influence[c].fill(0);
			for (int i = 0; i < N*N; i++)
			{
				if (estimates[c].Get(i))
				{
					influence[c][i] = initInf;
				}
			}

			int32_t currentInf = initInf;
			for (int p = 0; p < PATHN; p++)
			{
				estTemp.SetAll(false);
				libTemp.SetAll(false);
				for (int i = 0; i < N*N; i++)
				{
					if (estimates[c].Get(i))
					{
						GetLiberty(&libTemp, i);
						estTemp |= libTemp;
					}
				}
				estTemp.XorTrue(*board.m_stones[1-c]);

				for (int i = 0; i < N*N; i++)
				{
					if (estTemp.Get(i))
					{
						influence[c][i] += currentInf;
					}
				}
				currentInf = currentInf >> 1;
				estimates[c] |= estTemp;
			}
		}
		for (int i = 0; i < N*N; i++)
		{
			//if (influence[1][i] > 0)
			//{
			//	estimates[1].Set1(i);
			//}else
			//{
			//	estimates[1].Set0(i);
			//}
			//estimates[0].Set0(i);
			if (influence[0][i] > influence[1][i])
			{
				estimates[0].Set1(i);
				estimates[1].Set0(i);
			}else if (influence[0][i] < influence[1][i])
			{
				estimates[0].Set0(i);
				estimates[1].Set1(i);
			}else
			{
				estimates[0].Set0(i);
				estimates[1].Set0(i);
			}
		}
		if (blackEst != nullptr)
		{
			*blackEst = estimates[0];
		}
		if (whiteEst != nullptr)
		{
			*whiteEst = estimates[1];
		}
		return estimates[0].GetNumOfOnes() - estimates[1].GetNumOfOnes();
	}

	static int GetMove(int i, int j)
	{
		return i*N + j;
	}

	static void GetLiberty(TBitArray<N>* liberty, int move)
	{
		if(N==19)
			*liberty |= LibertyConst64_19[move];
		else
			GetLibertyGeneric(liberty, move);
	}

	static void GetLibertyGeneric(TBitArray<N>* liberty, int move)
	{
		if (move >= N)
		{
			liberty->Set1(move - N);
		}
		if (move + N < N*N)
		{
			liberty->Set1(move + N);
		}
		if (move % N > 0) // to optimize with bit op
		{
			liberty->Set1(move - 1);
		}
		if (move % N < N - 1)
		{
			liberty->Set1(move + 1);
		}
	}

	/*
	The following function is too expensive, resulting more move numbers to the end.
	bool EndGameCheckComplex()
	{
		TBitGroup newMove;
		TBitGroup* newMerged = new TBitGroup();
		list<Titer> oldGroups;
		list<Titer> capturedStones, selfCapture;
		bool hasMerged;
		bool hasCaptured;

		for (int c = 0; c < 2; c++)
		{
			int length = GetNumLegalPositions(c);
			for (int i = 0; i < length; i++)
			{
				int move = GetLegalMoveByIndex(c, i);
				newMove.stones.SetAll(false);
				newMove.liberty.SetAll(false);
				oldGroups.clear();
				capturedStones.clear();
				selfCapture.clear();
				if (!CheckLegal(move, c, nullptr, newMove, newMerged, oldGroups, capturedStones, selfCapture, hasMerged, hasCaptured, false))
				{
					m_legal[c]->Set0(move);
				}
			}
		}
		return ! (m_legal[0]->Intersects(*m_legal[1]));
	}
	*/

private:

	bool CheckLegal(int& move, int& color, const BitBoard* cmp,
		TBitGroup*& newMerged,
		bool& hasMerged, bool& hasCaptured
		)
	{
		if (!m_emptyStones->Get(move))
		{
			return false;
		}
		m_newMove.stones.SetAll(false);
		m_newMove.stones.Set1(move);
		m_newMove.liberty.SetAll(false);
		GetLiberty(&m_newMove.liberty, move);
		hasMerged = Merge(*newMerged, m_oldGroups, m_groups[color]);
		hasCaptured = CheckCapture(m_capturedStones, m_groups[1-color], newMerged->stones);
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

		*m_stones[color] |= m_newMove.stones;
		if (hasCaptured && (cmp!=nullptr)) // simple ko check
		{
			if (*m_stones[color] == *cmp->m_stones[color])
			{
				*m_stones[color] ^= m_newMove.stones;
				return false;
			}
		}
		return true;
	}

	bool Merge(TBitGroup& newMerged, list<Titer>& oldGroups, list<TBitGroup*>& groups)
	{
		newMerged = m_newMove;
		for(auto it = groups.begin(); it != groups.end(); ++it)
		{
			if (m_newMove.stones.Intersects((*it)->liberty))
			{
				oldGroups.push_back(it);
				newMerged.stones |= (*it)->stones;

				newMerged.liberty |= (*it)->liberty;
				newMerged.liberty.XorTrue((*it)->stones);
				newMerged.liberty.XorTrue(m_newMove.stones);
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

	void AddLiberty(TBitArray<N>& liberty, TBitArray<N>& stones)
	{
		int c = stones.GetNumOfOnes();
		if (c < N*3)
		{
			for (int i = 0; i < c; i++)
			{
				GetLiberty(&liberty, stones.GetNthOne(i));
			}
		}
		else
		{
			for (int i = 0; i < N * N; i++)
			{
				if (stones.Get(i))
				{
					GetLiberty(&liberty, i);
				}
			}
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