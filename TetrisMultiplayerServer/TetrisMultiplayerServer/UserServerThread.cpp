#include "stdafx.h"
#include "UserServerThread.h"


UserServerThread::UserServerThread(shared_ptr<RemoteUser> remoteUser) : remoteUser(remoteUser)
{
}

UserServerThread::~UserServerThread()
{
}

void UserServerThread::launchUserThread()
{
	thread(&UserServerThread::run, this);
}

void UserServerThread::run()
{
	cout << "Rozpoczeto watek uzytkownika ";

	ConnectionStatusMsg msg;
	msg.cmd = "connStatus";
	msg.status = "accepted";
	sf::Packet packet;
	packet << msg.cmd << msg.status;
	remoteUser->send(packet);

	string cmd = "start";
	while (!cmd.empty() && cmd != "end")
	{
		if(cmd == "newGame") startNewGame(packet);
		else if (cmd == "move") forwardMove(packet);
		else if (cmd == "getWaitingGames") sendWaitingGames(packet);
		else if (cmd == "connectToGame") connectToGame(packet);

		packet.clear();
		packet = remoteUser->receive();
		packet >> cmd;
	}
}

void UserServerThread::startNewGame(sf::Packet packet)
{
	StartGame msg;
	packet << msg.cmd << msg.gameType;
	if(msg.gameType == "single")
	{
		game = shared_ptr<ParentGameEngine> (new SingleGame(remoteUser));
	}
	else if (msg.gameType == "cooperation")
	{
		//game = shared_ptr<ParentGameEngine>(new CooperationGame(remoteUser));
	}
}

void UserServerThread::forwardMove(sf::Packet packet)
{
	MoveMsg msg;
	packet >> msg.cmd >> msg.moveType;
	if (msg.moveType == "DOWN")
	{
		game->registerMove(shared_ptr<UserMove>(new UserMove(remoteUser, DOWN)));
	}
	else if (msg.moveType == "LEFT")
	{
		game->registerMove(shared_ptr<UserMove>(new UserMove(remoteUser, LEFT)));
	}
	else if (msg.moveType == "DROP")
	{
		game->registerMove(shared_ptr<UserMove>(new UserMove(remoteUser, DROP)));
	}
	else if (msg.moveType == "RIGHT")
	{
		game->registerMove(shared_ptr<UserMove>(new UserMove(remoteUser, RIGHT)));
	}
}

void UserServerThread::sendWaitingGames(sf::Packet packet)
{

}

void UserServerThread::connectToGame(sf::Packet packet)
{

}

shared_ptr<ParentGameEngine> UserServerThread::getGame()
{
	return game;
}