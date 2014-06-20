// ConsoleTester.cpp : Defines the entry point for the console application.
//
#include "BitBoard.h"

#include "stdafx.h"

#include <cstdlib>
#include <ctime>

int trial = 0;

void OneGame(bool print)
{
	BitBoard<BitArray64, 19> board(19);
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
	BitBoard<BitArray64, 9> board(9);
	board.Move(4,4,BLACK);
	board.Move(5,4,WHITE);
	board.Move(3,4,WHITE);
	board.Move(4,5,WHITE);
	board.Move(4,3,WHITE);
	cout << board.ToString() << endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
	ManualGame9();
	//BitArray64<19> b;
	//b.Set1(100);
	//b.Set1(200);
	//b.Set1(123);
	//cout << b.ToPositionString() << endl;
	//cout << "n = " << b.GetNumOfOnes() << endl;
	//cout << "move = " << b.GetNthOne(0) << endl;
	//cout << "move = " << b.GetNthOne(1) << endl;
	//cout << "move = " << b.GetNthOne(2) << endl;

	////uint64_t test = 0;
	////test |= (1i64 << 17);
	////test |= (1i64 << 60);
	////test |= (1i64 << 37);
	////cout << bitset<64>(test) << endl;
	////cout << BitArray64Base::GetNthOneEach(test, 1) << endl;
	////cout << BitArray64Base::GetNthOneEach(test, 2) << endl;
	////cout << BitArray64Base::GetNthOneEach(test, 3) << endl;
	getchar();
	srand(10);
	std::clock_t start;
	start = std::clock();
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

