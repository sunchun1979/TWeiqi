// ConsoleDriver.cpp : Defines the entry point for the console application.
//
#include <algorithm>
#include <string>
#include <iostream>
#include <vector>

#include "Game.h"
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
	Game<19> game;
	string commandEx = "";
	while (commandEx != "exit")
	{
		getline(cin, commandEx);
		commandEx = ToLower(commandEx);
		int firstSpace = (int)commandEx.find_first_of(" ");
		string prefix = commandEx.substr(0, firstSpace);
		istringstream rest(commandEx.substr(firstSpace + 1, commandEx.length()));
		istream_iterator<string> beg(rest), end;
		vector<string> args(beg, end);
		if (prefix == "move")
		{
			game.PlayFromCurrent(stoi(args[0]), stoi(args[1]));
			cout << "done" << endl;
		}else if (prefix == "getcurrent")
		{
			auto current = game.GetCurrentStones(stoi(args[0]));
			cout << current.ToString() << endl;
		}else if (prefix == "loadsgf")
		{
			game.LoadSGF(args[0]);
			cout << "done" << endl;
		}
	}
	return 0;
}

