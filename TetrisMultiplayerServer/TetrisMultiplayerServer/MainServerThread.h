#pragma once
#include "SFML\Network.hpp"
#include <thread>
#include <iostream>
#include "UserServerThread.h"
#include "RemoteUser.h"
#include "RemoteCmds.h"
#include "Cmds.h"
#include <list>

using namespace std;

class MainServerThread
{
public:
	MainServerThread(int portNumber);
	~MainServerThread();
	void launchServerThread();
	thread * getServerThread();
	void rejectNewConnection(shared_ptr<sf::TcpSocket> clientSocket);
private:
	void run();
	void acceptNewConnection(string nick, shared_ptr<sf::TcpSocket> clientSocket);

	thread serverThread;
	const int portNumber;
	list<shared_ptr<UserServerThread>> userThreadsList;
	list<shared_ptr<ParentGameEngine>> gamesList;
};

