// ConsoleTester.cpp : Defines the entry point for the console application.
//
#include "BitBoard.h"
#include "Game.h"

#include "stdafx.h"

#include <cstdlib>
#include <ctime>

//#include <vld.h>

#pragma region PerformanceTest

long trial = 0;


template <int N>
void OneGameFull(bool print)
{
	BitBoard<BitArray64, N> board;
	BitBoard<BitArray64, N> boardHistory[2];
	int c = BLACK;
	bool gameEnd = false;
	while (!gameEnd)
	{
		int candidates = board.GetNumLegalPositions(c);
		int move = board.GetLegalMoveByIndex(c, rand() % candidates);
		while (!board.Move(move, c, &boardHistory[c]))
		{
			if (candidates == 1)
			{
				gameEnd = true;
				break;
			}
			board.MarkMoveIllegal(c, move);
			if (gameEnd = board.EndGameCheck())
			{
				break;
			}else
			{
				candidates = board.GetNumLegalPositions(c);
				move = board.GetLegalMoveByIndex(c, rand() % candidates);
				trial++;
			}
		}
		trial++;
		boardHistory[c] = board;
		c = 1 - c;
		//if (print)
		//{
		//	cout << board.ToString() << endl;
		//	getchar();
		//}
	}
	if (print)
	{
		cout << "final position" << endl;
		cout << board.ToString() << endl;
	}
}

// this is very slow because of extra copies in Game class
template <int N>
void GameDriver(bool print, int finalCount)
{
	Game<N> g;
	int c = BLACK;
	int moveCount = 0;
	bool finish = false;
	do
	{
		finish = !g.ComputerPlay(c);
		moveCount++;
		c = 1 - c;
	} while ( (!finish) && (moveCount < finalCount) );
	if (print)
	{
		cout << g.GetCurrentBoard().ToString() << endl;
		auto p = g.GetCurrentBoard();
		cout << "est " << endl;
		cout << p.EstimateCheck(p, nullptr, nullptr) << endl;
	}
}

void PerformanceTest()
{
	srand(10);
	std::clock_t start;
	start = std::clock();
	for (int i = 0; i < 1000; i++)
	{
		OneGameFull<19>(false);
	}
	OneGameFull<19>(true);
	//cout << "trial = " << trial << endl;
	std::clock_t end = std::clock();
	std::cout << "Time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
}

#pragma endregion PerformanceTest

void ManualGame9()
{
	BitBoard<BitArray64, 9> board;
	BitBoard<BitArray64, 9> boardHistory[2]; // simple ko check
	static const int NMoves = 6;
	// check self capture
	//int move[2][NMoves][2] = {
	//	{ {0, 4}, {1, 4}, {2, 3}, {0, 2}, {1, 2}, {2, 2} },
	//	{ {8, 7}, {8, 6}, {8, 5}, {8, 4}, {0, 3}, {1, 3} } 
	//};
	// check capture
	int move[2][NMoves][2] = {
		{ {0, 4}, {1, 4}, {2, 3}, {0, 2}, {1, 2}, {2, 2} },
		{ {0, 3}, {1, 3}, {8, 5}, {8, 4}, {8, 3}, {8, 2} } 
	};
	int c = BLACK;
	for (int i = 0; i < NMoves-1; i++)
	{
		board.Move(move[c][i][1], move[c][i][0], c);
		c = 1 - c;
		cout << board.ToString() << endl;
		//getchar();
		board.Move(move[c][i][1], move[c][i][0], c);
		c = 1 - c;
		cout << board.ToString() << endl;
		//getchar();
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	srand(0);
	Game<4> game;
	int gameSequence[] = {5, 10, 7};
	for (int i = 0; i < 3; i++)
	{
		cout << "play " << gameSequence[i] << endl;
		game.PlayFromCurrent(gameSequence[i], 0);
		cout << game.GetCurrentBoard().ToString() << endl;
		game.ComputerPlay(1);
		cout << game.GetCurrentBoard().ToString() << endl;
	}
	return 0;
}

