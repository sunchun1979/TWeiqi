#pragma once

#include <unordered_map>
#include <map>

#include "PlayerBase.h"
#include "UCTNode.h"

template <typename TBoard>
class PlayerUCT : public PlayerBase<TBoard>
{
	typedef UCTNode<TBoard> TNode;
private:

	map<TVKey, UCTNode<TBoard>*> m_boardDict;

	UCTNode<TBoard>* m_root;

public:
	PlayerUCT(TBoard board, int color) : PlayerBase<TBoard>(board, color)
	{
		m_root = new TNode(m_currentPosition, color);
		m_root->Reset();
		m_boardDict.insert(std::pair<TVKey, TNode*>(m_root->GetKey(), m_root));
	}

	~PlayerUCT()
	{
		for each (auto node in m_boardDict)
		{
			delete node.second;
		}
	}

	virtual TNode* TreePolicy(TNode* node)
	{
		TNode* front = node;
		while (!front->EndGameCheck())
		{
			if (!front->FullyExpanded())
			{
				TNode* candidate;
				int candidateMove;
				TVKey candidateKey;

				candidate = front->Expand(candidateMove);
				candidateKey = candidate->GetKey();
				if (m_boardDict.find(candidateKey) == m_boardDict.end())
				{
					m_boardDict[candidateKey] = candidate;
					front->AddChild(candidateMove, candidate);
					return candidate;
				}else
				{
					delete candidate;
					front->AddChild(candidateMove, m_boardDict[candidateKey]);
					return m_boardDict[candidateKey];
				}
			}else
			{
				front = front->BestChild();
			}
		}
		return front;
	}

	virtual int Play(int color, const TBoard* KOCheck, int KOLength = 2)
	{
		TreePolicy(m_root);
		return m_root->BestMove();
		//// Random play
		//int candidates = m_currentPosition.GetNumLegalPositions(color);
		//int move = m_currentPosition.GetLegalMoveByIndex(color, rand() % candidates);
		//bool gameEnd = false;
		//while (!m_currentPosition.Move(move, color, &KOCheck[color]))
		//{
		//	if (candidates == 1)
		//	{
		//		gameEnd = true;
		//		break;
		//	}
		//	m_currentPosition.MarkMoveIllegal(color, move);
		//	if (gameEnd = m_currentPosition.EndGameCheck())
		//	{
		//		break;
		//	}else
		//	{
		//		candidates = m_currentPosition.GetNumLegalPositions(color);
		//		move = m_currentPosition.GetLegalMoveByIndex(color, rand() % candidates);
		//	}
		//}
		//if (!gameEnd)
		//{
		//	return move;
		//}else
		//{
		//	return -1;
		//}
	}
};
