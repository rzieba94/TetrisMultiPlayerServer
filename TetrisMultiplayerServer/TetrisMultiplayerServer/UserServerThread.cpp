#include "stdafx.h"
#include "UserServerThread.h"


UserServerThread::UserServerThread(shared_ptr<RemoteUser> remoteUser, list<shared_ptr<ParentGameEngine>> & gamesList) : remoteUser(remoteUser), gamesList(gamesList)
{
}

UserServerThread::~UserServerThread()
{
}

void UserServerThread::launchUserThread()
{
	thread(&UserServerThread::run, this).detach();
}

void UserServerThread::run()
{
	cout << "Rozpoczeto watek uzytkownika ";
	
	ConnectionStatusMsg msg;
	msg.cmd = Cmds::connStatus;
	msg.status = "accepted";
	sf::Packet packet;
	packet << msg.cmd << msg.status;
	remoteUser->send(packet);

	int cmd = Cmds::connStatus;
	
	while (cmd != Cmds::endServer)
	{
		if(cmd == Cmds::startGame) startNewGame(packet);
		else if (cmd == Cmds::move) forwardMove(packet);
		else if (cmd == Cmds::getGamesList) sendWaitingGames(packet);
		else if (cmd == Cmds::connectToGame) connectToGame(packet);

		packet.clear();
		packet = remoteUser->receive();
		packet >> cmd;
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	cout << "koniec gry" << endl;
}

void UserServerThread::startNewGame(sf::Packet packet)
{
	cout << endl << "Rozpoczecie nowej gry" << endl;
	StartGame msg;
	packet >> msg.gameType >> msg.playersNumber >> msg.userIds;
	cout << msg.gameType << endl;
	if(msg.gameType == GameType::single)
	{
		remoteUser->setStartPosition(sf::Vector2i(5, 2));
		game = shared_ptr<ParentGameEngine> (new SingleGame(remoteUser, getCurrentGameId()));
		game->startThread();
		gamesList.push_back(game);
	}
	else if (msg.gameType == GameType::cooperation)
	{
		game = shared_ptr<ParentGameEngine>(new CooperationGame(remoteUser, getCurrentGameId(), msg.playersNumber));
		game->startThread();
		gamesList.push_back(game);
	}
}

void UserServerThread::forwardMove(sf::Packet packet)
{
	MoveMsg msg;
	packet >> msg.moveType;
	if (msg.moveType == MoveType::DOWN)
	{
		game->registerMove(shared_ptr<UserMove>(new UserMove(remoteUser, DOWN)));
	}
	else if (msg.moveType == MoveType::LEFT)
	{
		game->registerMove(shared_ptr<UserMove>(new UserMove(remoteUser, LEFT)));
	}
	else if (msg.moveType == MoveType::DROP)
	{
		game->registerMove(shared_ptr<UserMove>(new UserMove(remoteUser, DROP)));
	}
	else if (msg.moveType == MoveType::RIGHT)
	{
		game->registerMove(shared_ptr<UserMove>(new UserMove(remoteUser, RIGHT)));
	}
	else if (msg.moveType == MoveType::ROTATE)
	{
		game->registerMove(shared_ptr<UserMove>(new UserMove(remoteUser, ROTATE)));
	}
}

void UserServerThread::sendWaitingGames(sf::Packet packet)
{

}

void UserServerThread::connectToGame(sf::Packet packet)
{

}

int UserServerThread::getCurrentGameId()
{
	static int gameId;
	gameId++;
	return gameId;
}