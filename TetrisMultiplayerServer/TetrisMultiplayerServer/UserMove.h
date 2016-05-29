#pragma once
#include "stdafx.h"
#include "RemoteUser.h"

class UserMove
{
public:
	UserMove(shared_ptr<RemoteUser> user, MoveType moveType);
	~UserMove();
	shared_ptr<RemoteUser> getUser();
	MoveType getMoveType();
private:
	shared_ptr<RemoteUser> user;
	MoveType moveType;
};

