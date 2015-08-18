// Stage_11_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Gameloop.h>
#include <iostream>

using namespace stage_11;

int _tmain(int argc, _TCHAR* argv[])
{
	Gameloop loop(4);
	loop.start();
	char c;
	std::cin >> c;
	return 0;
}

