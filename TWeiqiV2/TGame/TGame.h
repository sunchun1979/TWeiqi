#include <string>
#include <list>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <regex>

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

		m_history.clear();
		m_currentPosition.Clear();

		ifstream ifs(filename, std::ifstream::in);
		string buffer((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
		ifs.close();

		buffer.erase(remove(buffer.begin(), buffer.end(), '\n'), buffer.end());
		buffer.erase(remove(buffer.begin(), buffer.end(), '\r'), buffer.end());
		buffer.erase(remove(buffer.begin(), buffer.end(), ' '), buffer.end());
		buffer.erase(remove(buffer.begin(), buffer.end(), ';'), buffer.end());
		if (buffer.find_first_of("(") != buffer.npos)
		{
			buffer.erase(buffer.begin(), buffer.begin() + buffer.find_first_of("("));
		}
		if (buffer.find_last_of(")") != buffer.npos)
		{
			buffer.erase(buffer.begin() + buffer.find_last_of(")"), buffer.end());
		}

		regex reg1("[A-Z]");
		smatch m;
		int p1;
		int p2;
		string phrase;
		while (!buffer.empty())
		{
			regex_search(buffer, m, reg1);
			p1 = (int)m.position();
			p2 = (int)buffer.find_first_of('[', p1 + 1);
			p2 = (int)buffer.find_first_of(']', p2 + 1);
			if(regex_search(buffer.substr(p2+1), m, reg1))
			{
				p2 = p2 + (int)m.position();
			}else
			{
				p2 = (int)buffer.find_last_of(']');
			}
			phrase = buffer.substr(p1, p2 - p1 + 1);
			handlePhrase(phrase);
			buffer = buffer.substr(p2 + 1);
		}
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

	bool ComputerPlay(int color)
	{
		// Random play
		int candidates = m_currentPosition.GetNumLegalPositions(color);
		int move = m_currentPosition.GetLegalMoveByIndex(color, rand() % candidates);
		bool gameEnd = false;
		while (!m_currentPosition.Move(move, color, &m_KOCheck[color]))
		{
			if (candidates == 1)
			{
				gameEnd = true;
				break;
			}
			m_currentPosition.MarkMoveIllegal(color, move);
			if (gameEnd = m_currentPosition.EndGameCheck())
			{
				break;
			}else
			{
				candidates = m_currentPosition.GetNumLegalPositions(color);
				move = m_currentPosition.GetLegalMoveByIndex(color, rand() % candidates);
			}
		}
		if (!gameEnd)
		{
			m_KOCheck[color] = m_currentPosition;
			m_history.push_back(m_currentPosition);
			return true;
		}else
		{
			return false;
		}
	}

	BitArray64<N> GetCurrentStones(int color)
	{
		return m_currentPosition.GetRawStones(color);
	}

	BitArray64<N> GetCurrentEst(int color)
	{
		throw new exception("not implmented");
	}

	TBoard GetCurrentBoard()
	{
		return m_currentPosition;
	}

private:

	void handlePhrase(string phrase)
	{
		string key = phrase.substr(0, phrase.find_first_of('['));
		if ( (key=="B") || (key=="AB") || (key=="W") || (key=="AW") )
		{
			int color;
			if (key.find("B") != key.npos)
			{
				color = 0;
			}else
			{
				color = 1;
			}
			string values = phrase.substr(phrase.find_first_of('['));
			values.erase(remove(values.begin(), values.end(), '['), values.end());
			values.erase(remove(values.begin(), values.end(), ']'), values.end());
			for (int i = 0; i < values.length()/2; i++)
			{
				int x = (int)(values[2*i]-'a');
				int y = (int)(values[2*i+1]-'a');
				int move = m_currentPosition.GetMove(x, y);
				m_currentPosition.Move(move, color);
				TBoard* newBoard = new TBoard(m_currentPosition);
				m_history.push_back(*newBoard);
			}
		}
	}
};
