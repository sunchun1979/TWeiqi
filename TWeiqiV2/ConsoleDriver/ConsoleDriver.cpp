// ConsoleDriver.cpp : Defines the entry point for the console application.
//
#include <algorithm>
#include <string>
#include <iostream>
#include <vector>

#include "TGame.h"
#include "BitBoard.h"

#include "stdafx.h"

using namespace std;

static string ToLower(const string& tmp)
{
	string ret(tmp);
	transform(tmp.begin(), tmp.end(), ret.begin(), ::tolower);
	return ret;
}

int _tmain(int argc, _TCHAR* argv[])
{
	TGame<19> game;
	string commandEx = "";
	while (commandEx != "exit")
	{
		getline(cin, commandEx);
		commandEx = ToLower(commandEx);
		int firstSpace = (int)commandEx.find_first_of(" ");
		string prefix = commandEx.substr(0, firstSpace);
		istringstream rest(commandEx.substr(firstSpace + 1, commandEx.length()));
		istream_iterator<uint64_t> beg(rest), end;
		vector<uint64_t> args(beg, end);
		if (prefix == "move")
		{
			game.PlayFromCurrent((int)args[0], (int)args[1]);
		}else if (prefix == "getcurrent")
		{
			auto current = game.GetCurrentStones((int)args[0]);
			cout << current.ToString() << endl;
		}
		cout << "done" << endl;
	}
	return 0;
}

