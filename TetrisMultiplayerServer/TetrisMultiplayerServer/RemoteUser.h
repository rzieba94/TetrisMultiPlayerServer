#pragma once
#include <string>
#include <SFML\Network.hpp>
#include <memory>
#include "Tetromino.h"

using namespace std;

class RemoteUser
{
public:
	RemoteUser(string nick, shared_ptr<sf::TcpSocket> clientSocket);
	~RemoteUser();
	void send(sf::Packet packet);
	sf::Packet receive();
	shared_ptr<Tetromino> getActiveTetromino();
	void setActiveTetromino(shared_ptr<Tetromino> activeTetromino);
	sf::Vector2i getStartPosition();
	void setStartPosition(sf::Vector2i startPosition);
	int getScore();
	void setScore(int score);
	string getNick();
private:
	string nick;
	shared_ptr<sf::TcpSocket> clientSocket;
	int score;
	shared_ptr<Tetromino> activeTetromino;
	sf::Vector2i startPosition;
};

