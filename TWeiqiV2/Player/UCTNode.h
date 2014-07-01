#pragma once

#include <list>
#include <vector>

#include "BitBoard.h"

template <typename TBoard>
class UCTNode
{
private:
	TBoard board;
	uint64_t N;
	double Q;

	TVKey key;
	list<UCTNode*> parents;
	list<UCTNode*> children;

public:
	UCTNode(TBoard b) : board(b), N(0), Q(0)
	{
		board.AssignRawVector(key);
	}

	~UCTNode()
	{
	}

	const TVKey& GetKey() const
	{
		return key;
	}

private:

};
