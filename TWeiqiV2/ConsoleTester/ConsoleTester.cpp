// ConsoleTester.cpp : Defines the entry point for the console application.
//
#include "BitBoard.h"

#include "stdafx.h"

#include <cstdlib>
#include <ctime>

int trial = 0;

void OneGame(bool print)
{
	BitBoard<BitArray19> board(19);
	int totalMove = 200;
	int c = BLACK;
	for (int i = 0; i < totalMove; i++)
	{
		int candidates = board.GetNumEmptyPositions();
		int move = board.GetMoveByIndex(rand() % candidates);
		while(!board.Move(move, c)){
			trial ++;
			move = board.GetMoveByIndex(rand() % candidates);
		}
		c = 1 - c;
	}
	if (print)
	{
		cout << board.ToString() << endl;
	}
}

void ManualGame9()
{
	BitBoard<BitArray9> board(9);
	board.Move(4,4,BLACK);
	board.Move(5,4,WHITE);
	board.Move(3,4,WHITE);
	board.Move(4,5,WHITE);
	board.Move(4,3,WHITE);
	cout << board.ToString() << endl << endl;
	cout << board.Move(4,4, BLACK) << endl;
	cout << board.ToString() << endl << endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
	getchar();
	srand(1);
	std::clock_t start;
	start = std::clock();
	///ManualGame9();
	for (int i = 0; i < 10000; i++)
	{
		//cout << i << endl;
		OneGame(false);
	}
	OneGame(true);
	cout << "trial = " << trial << endl;
	std::clock_t end = std::clock();
	std::cout << "Time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
	return 0;
}

