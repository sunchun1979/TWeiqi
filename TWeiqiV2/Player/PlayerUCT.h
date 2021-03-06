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
	uint64_t m_beginRootN;

	int m_defaultPolicyN;

public:
	PlayerUCT(TBoard board, int color) : PlayerBase<TBoard>(board, color)
	{
		m_defaultPolicyN = 10;
		m_root = new TNode(m_currentPosition, color);
		m_root->Reset(m_currentPosition);
		m_boardDict.insert(std::pair<TVKey, TNode*>(m_root->GetKey(), m_root));
	}

	~PlayerUCT()
	{
		for each (auto node in m_boardDict)
		{
			delete node.second;
		}
	}

	virtual bool Update(int move, int color)
	{
		bool ret = m_currentPosition.Move(move, color);
		TVKey key;
		m_currentPosition.AssignRawVector(key);
		if (m_boardDict.find(key) != m_boardDict.end())
		{
			m_root = m_boardDict[key];
		}else
		{
			m_root = new TNode(m_currentPosition, color);
			m_root->Reset(m_currentPosition);
			m_boardDict.insert(std::pair<TVKey, TNode*>(m_root->GetKey(), m_root));
		}
		return ret;
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
				//cout << "candidateMove = " << candidateMove << endl;
				//cout << "Expanded with legal moves " << candidate->m_legalMoves.size() << endl;
				//cout << candidate->GetBoard().ToString() << endl;
				candidateKey = candidate->GetKey();
				if (m_boardDict.find(candidateKey) == m_boardDict.end())
				{
					m_boardDict[candidateKey] = candidate;
					//cout << "New move " << candidateMove << endl;
					front->AddChild(candidateMove, candidate);
					return candidate;
				}else
				{
					delete candidate;
					//cout << "old move " << candidateMove << endl;
					front->AddChild(candidateMove, m_boardDict[candidateKey]);
					return m_boardDict[candidateKey];
				}
			}else
			{
				TNode* save = front;
				front = front->BestChild();
				if (front == save)
				{
					// ko situation in tree building
					cout << front->GetBoard().ToString() << endl;
					getchar();
				}
				if (front == nullptr)
				{
					save->MarkStop();
					front = save;
				}
			}
		}
		return front;
	}

	virtual double DefaultPolicy(TNode* node)
	{
		int delta = 0;
		for (int i = 0; i < m_defaultPolicyN; i++)
		{
			delta += PlayOut(node);
		}
		return (double)delta/m_defaultPolicyN;
	}

	static int PlayOut(TNode* node)
	{
		int color = node->GetColor();
		PlayerRandom<TBoard> randPlayer(node->GetBoard(), color);
		TBoard m_KOCheck[2];
		int move = randPlayer.Play(color, m_KOCheck);
		int count = 0;
		while ( (move >= 0) && (count < 1000) )
		{
			m_KOCheck[color] = randPlayer.GetBoard();
			color = 1 - color;
			move = randPlayer.Play(color, m_KOCheck);
			count++;
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
		if ( double(now - m_beginTime) / CLOCKS_PER_SEC > 30)
		{
			cout << " expanded " << m_root->m_N - m_beginRootN << " nodes" << endl;
			return false;
		}
		//cout << m_root->m_N << " " << m_root->m_Q << endl;
		//if (m_root->m_N - m_beginRootN > 1000)
		//{
		//	return false;
		//}
		return true;
	}

	virtual void InitializeResource()
	{
		m_beginRootN = m_root->m_N;
	}

	virtual int Play(int color, const TBoard* KOCheck, int KOLength = 2)
	{
		m_beginTime = clock();
		cout << m_root->GetBoard().ToString() << endl;
		InitializeResource();
		//cout << " root board 1 " << m_root->GetBoard().ToString() << endl;
		//m_root->PrintChildren();
		while(HasResource())
		{
			TNode* candidateNode = TreePolicy(m_root);
			double delta = DefaultPolicy(candidateNode);
			Backup(candidateNode, delta);

			//cout << candidateNode->GetBoard().ToString() << endl;
			//getchar();
		}
		//cout << " root board " << m_root->GetBoard().ToString() << endl;
		//m_root->PrintChildren();
		cout << " best move = " << m_root->BestMove() << endl;
		return m_root->BestMove();
	}
};
