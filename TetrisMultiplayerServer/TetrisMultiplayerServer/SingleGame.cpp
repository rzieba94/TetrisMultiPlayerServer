#include "stdafx.h"
#include "SingleGame.h"
#include <thread>

using namespace std;

SingleGame::SingleGame(shared_ptr<RemoteUser>ownerUser) : ParentGameEngine(ownerUser)
{
}


SingleGame::~SingleGame()
{
}

void SingleGame::run()
{
	placeNewTetromino();
	while (true)
	{
		checkPlayersMove();
		notActiveTetrominos.clearLine(getLineToClear());
		if (checkForInactiveBlock())
		{
			if (!placeNewTetromino())
			{
				return;
			}
		}
		

		checkFrameTime();
		this_thread::sleep_for(chrono::milliseconds(10));
	}
}

void SingleGame::checkPlayersMove()
{
	
}

bool SingleGame::placeNewTetromino()
{
	return placeNewTetromino(usersList.front());
}

bool SingleGame::checkForInactiveBlock()
{
	return checkForInactiveBlock(usersList.front());
}