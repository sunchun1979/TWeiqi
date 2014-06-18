// ConsoleTester.cpp : Defines the entry point for the console application.
//
#include "BitBoard.h"

#include "stdafx.h"

#include <cstdlib>
#include <ctime>

BitBoard<BitArray19> OneGame()
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
	return board;
}


int _tmain(int argc, _TCHAR* argv[])
{
	srand(1);
	BitBoard<BitArray19> board(19);
	std::clock_t start;
	start = std::clock();
	for (int i = 0; i < 2000; i++)
	{
		cout << i << endl;
		board = OneGame();
	}
	cout << board.ToString() << endl;
	std::clock_t end = std::clock();
	std::cout << "Time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
	return 0;
}

