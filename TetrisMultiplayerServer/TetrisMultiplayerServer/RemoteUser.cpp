#include "stdafx.h"
#include "RemoteUser.h"


RemoteUser::RemoteUser(string nick, shared_ptr<sf::TcpSocket> clientSocket): nick(nick), clientSocket(clientSocket)
{
}


RemoteUser::~RemoteUser()
{
}

void RemoteUser::send(sf::Packet packet)
{
	clientSocket->send(packet);
}

sf::Packet RemoteUser::receive()
{
	sf::Packet packet;
	clientSocket->receive(packet);
	return packet;
}