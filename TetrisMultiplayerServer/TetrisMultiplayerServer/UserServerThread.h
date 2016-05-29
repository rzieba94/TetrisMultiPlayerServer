#pragma once
#include "RemoteUser.h"
#include "SingleGame.h"
#include "CooperationGame.h"

class UserServerThread
{
public:
	UserServerThread(shared_ptr<RemoteUser> remoteUser);
	~UserServerThread();
	void launchUserThread();
private: 
	void run();
	void startNewGame(sf::Packet packet);
	void forwardMove(sf::Packet packet);
	void sendWaitingGames(sf::Packet packet);
	void connectToGame(sf::Packet packet);
	shared_ptr<ParentGameEngine> getGame();

	shared_ptr<RemoteUser> remoteUser;
	shared_ptr<ParentGameEngine> game;
};

