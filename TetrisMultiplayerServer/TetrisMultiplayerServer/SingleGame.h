#pragma once
#include "ParentGameEngine.h"

using namespace std;

class SingleGame : public ParentGameEngine
{
public:
	SingleGame(shared_ptr<RemoteUser> ownerUser);
	~SingleGame();
private:
	void run();
	bool placeNewTetromino();
	bool checkForInactiveBlock();
};

