#include "stdafx.h"
#include "RemoteUser.h"


RemoteUser::RemoteUser(string nick, shared_ptr<sf::TcpSocket> clientSocket): nick(nick), clientSocket(clientSocket), score(0)
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

shared_ptr<Tetromino> RemoteUser::getActiveTetromino()
{
	return activeTetromino;
}

void RemoteUser::setActiveTetromino(shared_ptr<Tetromino> activeTetromino)
{
	this->activeTetromino = activeTetromino;
}

sf::Vector2i RemoteUser::getStartPosition()
{
	return startPosition;
}

void RemoteUser::setStartPosition(sf::Vector2i startPosition)
{
	this->startPosition = startPosition;
}

int RemoteUser::getScore()
{
	return score;
}

void RemoteUser::setScore(int score)
{
	this->score = score;
}