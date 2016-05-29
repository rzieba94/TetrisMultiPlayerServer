#pragma once
#include "ParentGameEngine.h"

class CooperationGame : public ParentGameEngine
{
public:
	CooperationGame(shared_ptr<RemoteUser> ownerUser);
	~CooperationGame();
};

