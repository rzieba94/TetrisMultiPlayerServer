#pragma once
#include "ParentGameEngine.h"

class CooperationGame : public ParentGameEngine
{
public:
	CooperationGame(shared_ptr<RemoteUser> ownerUser, int gameId, int playersNumber);
	~CooperationGame();
private:
	void run();
	bool waitForPlayers();
	void sendWaitingMsg(int time);
	void sendTimeoutMsg();
	bool checkIfGameEnded();
};

