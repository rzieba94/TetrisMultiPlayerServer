#pragma once
#include "RemoteUser.h"
#include "SingleGame.h"
#include "CooperationGame.h"

class UserServerThread
{
public:
	UserServerThread(shared_ptr<RemoteUser> remoteUser, list<shared_ptr<ParentGameEngine>> & gamesList);
	~UserServerThread();
	void launchUserThread();
private: 
	void run();
	void startNewGame(sf::Packet packet);
	void forwardMove(sf::Packet packet);
	void sendWaitingGames(sf::Packet packet);
	void connectToGame(sf::Packet packet);
	int getCurrentGameId();

	shared_ptr<RemoteUser> remoteUser;
	shared_ptr<ParentGameEngine> game;
	list<shared_ptr<ParentGameEngine>> gamesList;
};

