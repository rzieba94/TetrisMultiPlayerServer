#include "stdafx.h"
#include "SingleGame.h"
#include <thread>

using namespace std;

SingleGame::SingleGame(shared_ptr<RemoteUser>ownerUser, int gameId) : ParentGameEngine(ownerUser, gameId, 1, GameType::single, 10)
{
}


SingleGame::~SingleGame()
{
}

void SingleGame::run()
{
	cout << "Uzytkownik " << usersList.front()->getNick() << " rozpoczal nowa gre pojedyncza";
	sendStartGameMsg();
	placeNewTetromino();
	while (true)
	{
		checkPlayersMove();
		notActiveTetrominos.clearLine(getLineToClear());
		if (checkForInactiveBlock())
		{
			if (!placeNewTetromino())
			{
				cout << "Uzytkownik " << usersList.front()->getNick() << " zakonczyl gre pojedyncza";
				sendEndGameMsg();
				return;
			}
		}

		checkFrameTime();
		this_thread::sleep_for(chrono::milliseconds(10));
	}
}

bool SingleGame::placeNewTetromino()
{
	return ParentGameEngine::placeNewTetromino(usersList.front());
}

bool SingleGame::checkForInactiveBlock()
{
	return ParentGameEngine::checkForInactiveBlock(usersList.front());
}

void SingleGame::sendEndGameMsg()
{
	ParentGameEngine::sendEndGameMsg(usersList.front());
}