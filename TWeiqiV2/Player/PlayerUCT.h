#pragma once

#include <unordered_map>
#include <map>
#include <set>
#include <queue>
#include <ctime>

#include "PlayerBase.h"
#include "PlayerRandom.h"
#include "UCTNode.h"

template <typename TBoard>
class PlayerUCT : public PlayerBase<TBoard>
{
	typedef UCTNode<TBoard> TNode;
private:

	map<TVKey, UCTNode<TBoard>*> m_boardDict;

	UCTNode<TBoard>* m_root;

	clock_t m_beginTime;

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

	virtual double DefaultPolicy(TNode* node)
	{
		return PlayOut(node);
	}

	static int PlayOut(TNode* node)
	{
		int color = node->GetColor();
		PlayerRandom<TBoard> randPlayer(node->GetBoard(), color);
		TBoard m_KOCheck[2];
		int move = randPlayer.Play(color, m_KOCheck);
		while (move >= 0)
		{
			m_KOCheck[color] = randPlayer.GetBoard();
			color = 1 - color;
			move = randPlayer.Play(color, m_KOCheck);
		}
		if (randPlayer.GetBoard().FinalCheckBlack())
		{
			return 1;
		}else
		{
			return -1;
		}
	}

	virtual void Backup(TNode* leafNode, double delta)
	{
		queue<TNode*> nodeToUpdate;
		set<TNode*> nodeSet;
		nodeToUpdate.push(leafNode);
		nodeSet.insert(leafNode);
		while (nodeToUpdate.size() > 0)
		{
			queue<TNode*> currentLevel;
			while (nodeToUpdate.size() > 0)
			{
				TNode* current = nodeToUpdate.front();
				nodeToUpdate.pop();
				current->m_N ++;
				current->m_Q += delta;
				currentLevel.push(current);
			}
			delta = -delta;

			while(currentLevel.size() > 0)
			{
				TNode* current = currentLevel.front();
				list<TNode*> currentParents = current->GetParents();
				for each (auto singleParent in currentParents)
				{
					if (nodeSet.find(singleParent) == nodeSet.end())
					{
						nodeSet.insert(singleParent);
						nodeToUpdate.push(singleParent);
					}
				}
				currentLevel.pop();
				nodeSet.erase(current);
			}
		}
	}

	virtual bool HasResource()
	{
		clock_t now = clock();
		//if ( double(now - m_beginTime) / CLOCKS_PER_SEC > 10)
		//{
		//	return false;
		//}
		cout << m_root->m_N << endl;
		if (m_root->m_N > 1000)
		{
			return false;
		}
		return true;
	}

	virtual int Play(int color, const TBoard* KOCheck, int KOLength = 2)
	{
		m_beginTime = clock();
		while(HasResource())
		{
			TNode* candidateNode = TreePolicy(m_root);
			double delta = DefaultPolicy(candidateNode);
			Backup(candidateNode, delta);
		}
		return m_root->BestMove();
	}
};
