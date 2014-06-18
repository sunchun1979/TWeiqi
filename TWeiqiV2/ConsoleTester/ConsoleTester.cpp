// ConsoleTester.cpp : Defines the entry point for the console application.
//
#include "BitBoard.h"

#include "stdafx.h"

#include <cstdlib>
#include <ctime>

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
			move = board.GetMoveByIndex(rand() % candidates);
		}
		c = 1 - c;
	}
	if (print)
	{
		cout << board.ToString() << endl;
	}
}


int _tmain(int argc, _TCHAR* argv[])
{
	getchar();
	srand(1);
	std::clock_t start;
	start = std::clock();
	for (int i = 0; i < 10000; i++)
	{
		cout << i << endl;
		OneGame(false);
	}
	OneGame(true);
	std::clock_t end = std::clock();
	std::cout << "Time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
	return 0;
}

