#pragma once
#include <string>
#include <SFML\Network.hpp>
#include <memory>

using namespace std;

class RemoteUser
{
public:
	RemoteUser(string nick, shared_ptr<sf::TcpSocket> clientSocket);
	~RemoteUser();
	void send(sf::Packet packet);
	sf::Packet receive();
private:
	string nick;
	shared_ptr<sf::TcpSocket> clientSocket;
};

