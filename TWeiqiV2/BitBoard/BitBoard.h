#include <stdio.h>
#include <iostream>

#include "BitArray19.h"

using namespace std;

class BitBoard
{
public:
	BitBoard();
	~BitBoard();

private:

};

BitBoard::BitBoard()
{
	BitArray19 b;
	cout << b.ToString() << endl;
	b.SetAll(true);
	b.Set0(200);
	b.Set0(100);
	cout << b.ToString() << endl;
	BitArray19 c = b;
	c.Set1(200);
	cout << c.ToString() << endl;
	cout << (c & b).ToString() << endl;
}

BitBoard::~BitBoard()
{
}