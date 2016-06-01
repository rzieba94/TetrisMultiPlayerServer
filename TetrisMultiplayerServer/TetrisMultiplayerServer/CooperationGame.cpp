#include "stdafx.h"
#include "CooperationGame.h"


CooperationGame::CooperationGame(shared_ptr<RemoteUser> ownerUser, int gameId, int playersNumber) : ParentGameEngine(ownerUser, gameId, playersNumber, GameType::cooperation, playersNumber * 10)
{
}


CooperationGame::~CooperationGame()
{
}

void CooperationGame::run()
{
	cout << "Rozpoczeto nowa gre w trybie wspolnym.";
	if (waitForPlayers())
	{
		sendStartGameMsg();
		for (shared_ptr<RemoteUser> player : usersList)
		{
			placeNewTetromino(player);
		}
		while (true)
		{
			checkPlayersMove();
			notActiveTetrominos->clearLine(getLineToClear());
			for (shared_ptr<RemoteUser> player : usersList)
			{
				if (checkForInactiveBlock(player))
				{
					if (!placeNewTetromino(player))
					{
						cout << "Uzytkownik " << player->getNick() << " zakonczyl gre wspolna";
						sendEndGameMsg(player);
						usersList.remove(player);
						for (shared_ptr<RemoteUser> player : usersList)
						{
							sendlLoseMsg(player);
						}
					}
				}
			}

			if (checkIfGameEnded())
			{
				return;
			}

			checkFrameTime();
			this_thread::sleep_for(chrono::milliseconds(10));
		}
	}
	else
	{
		sendTimeoutMsg();
	}	
}

bool CooperationGame::waitForPlayers()
{
	clock_t waitingTime = clock();
	int waitingCounter = 901;

	while (waitingCounter > 0 && usersList.size() != playersNumber)
	{
		if (clock() - waitingTime >= 1000)
		{
			waitingTime = clock();
			waitingCounter--;
			sendWaitingMsg(waitingCounter);
		}
	}

	return waitingCounter > 0;
}

void CooperationGame::sendWaitingMsg(int time)
{
	WaitingTime msg;
	msg.cmd = Cmds::waiting;
	msg.waitingtime = time;
	sf::Packet packet;
	packet << msg.cmd << msg.waitingtime;
	for (shared_ptr<RemoteUser> player : usersList)
	{
		player->send(packet);
	}
}

void CooperationGame::sendTimeoutMsg()
{
	SimpleCommand msg;
	msg.cmd = Cmds::timeout;
	sf::Packet packet;
	packet << msg.cmd;

	for (shared_ptr<RemoteUser> player : usersList)
	{
		player->send(packet);
	}
}

bool CooperationGame::checkIfGameEnded()
{
	if (usersList.size() == 1)
	{
		cout << "Gra wspolna zostala zakonczona.";
		sendWinMessage(usersList.front());
		return true;
	}
	else
	{
		return false;
	}
}

void CooperationGame::sendWinMessage(shared_ptr<RemoteUser> player)
{
	SimpleCommand msg;
	msg.cmd = Cmds::endGame;
	msg.winner = true;
	sf::Packet packet;
	packet << msg.cmd << msg.winner;
	player->send(packet);
	player->setScore(0);
}

void CooperationGame::sendlLoseMsg(shared_ptr<RemoteUser> player)
{
	sendUserLost lost;
	lost.cmd = Cmds::userLost;
	lost.nick = player->getNick();
	lost.score = player->getScore();
	sf::Packet losePacket;
	losePacket.clear();
	losePacket << lost.cmd << lost.nick << lost.score;
	player->send(losePacket);
}