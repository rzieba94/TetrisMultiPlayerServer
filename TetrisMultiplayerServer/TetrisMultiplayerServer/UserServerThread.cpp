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

	string cmd;
	while (!cmd.empty && cmd != "end")
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

}

void UserServerThread::forwardMove(sf::Packet packet)
{

}

void UserServerThread::sendWaitingGames(sf::Packet packet)
{

}

void UserServerThread::connectToGame(sf::Packet packet)
{

}
