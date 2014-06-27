// ConsoleTester.cpp : Defines the entry point for the console application.
//
#include "BitBoard.h"
#include "TGame.h"

#include "stdafx.h"

#include <cstdlib>
#include <ctime>


#pragma region PerformanceTest

long trial = 0;

template <int N>
void OneGame(bool print)
{
	BitBoard<BitArray64, N> board;
	BitBoard<BitArray64, N> boardHistory[2]; // simple ko check
	int totalMove = N*N/2;
	int c = BLACK;
	for (int i = 0; i < totalMove; i++)
	{
		int candidates = board.GetNumEmptyPositions();
		int move = board.GetMoveByIndex(rand() % candidates);
		while(!board.Move(move, c, &boardHistory[c])){
			trial ++;
			move = board.GetMoveByIndex(rand() % candidates);
		}
		boardHistory[c] = board;
		c = 1 - c;
	if (print)
	{
		cout << board.ToString() << endl;
		getchar();
	}
	}
	if (print)
	{
		cout << board.ToString() << endl;
	}
}

//void OneGameFull(bool print)
//{
//	BitBoard<BitArray64, 9> board;
//	BitBoard<BitArray64, 9> boardHistory[2];
//	int c = BLACK;
//	bool gameEnd = false;
//	while (!gameEnd)
//	{
//		int candidates = board.GetNumLegalPositions(c);
//		int move = board.GetLegalMoveByIndex(c, rand() % candidates);
//		while (!board.Move(move, c, &boardHistory[c]))
//		{
//			if (candidates == 1)
//			{
//				gameEnd = true;
//				break;
//			}
//			board.MarkMoveIllegal(c, move);
//			if (gameEnd = board.EndGameCheck())
//			{
//				break;
//			}else
//			{
//				candidates = board.GetNumLegalPositions(c);
//				if (candidates > 1)
//				{
//					move = board.GetLegalMoveByIndex(c, rand() % candidates);
//					trial++;
//				}
//				else
//				{
//					gameEnd = true;
//					break;
//				}
//			}
//		}
//		trial++;
//		if (!gameEnd)
//		{
//			boardHistory[c] = board;
//			c = 1 - c;
//			cout << board.ToString() << endl;
//			//getchar();
//		}
//	}
//	if (print)
//	{
//		cout << "final position" << endl;
//		cout << board.ToString() << endl;
//	}
//}

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

void PerformanceTest()
{
	srand(10);
	std::clock_t start;
	start = std::clock();
	for (int i = 0; i < 10000; i++)
	{
		OneGameFull<19>(false);
	}
	OneGameFull<19>(true);
	cout << "trial = " << trial << endl;
	std::clock_t end = std::clock();
	std::cout << "Time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
}

#pragma endregion PerformanceTest

void ManualGame9()
{
	BitBoard<BitArray64, 9> board;
	BitBoard<BitArray64, 9> boardHistory[2]; // simple ko check
	static const int NMoves = 6;
	int move[2][NMoves][2] = {
		{ {0, 4}, {1, 4}, {2, 3}, {0, 2}, {1, 2}, {2, 2} },
		{ {8, 7}, {8, 6}, {8, 5}, {8, 4}, {0, 3}, {1, 3} } 
	};
	int c = BLACK;
	for (int i = 0; i < NMoves; i++)
	{
		board.Move(move[c][i][1], move[c][i][0], c);
		c = 1 - c;
		cout << board.ToString() << endl;
		getchar();
		board.Move(move[c][i][1], move[c][i][0], c);
		c = 1 - c;
		cout << board.ToString() << endl;
		getchar();
	}
	cout << board.ToString() << endl;
	getchar();
}

int _tmain(int argc, _TCHAR* argv[])
{
	TGame<19> t;
	t.LoadSGF("C:\\Users\\chunsun\\SkyDrive\\Projects\\Weiqi\\DataSet\\single\\66.sgf");
	auto v = t.GetCurrentBoard();
	cout << v.ToString() << endl;
	return 0;
}

