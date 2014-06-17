// ConsoleTester.cpp : Defines the entry point for the console application.
//
#include "BitBoard.h"

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	BitBoard<BitArray19> c(19);
	c.Move(9, 9, BLACK);
	cout << c.ToString() << endl;
	return 0;
}

