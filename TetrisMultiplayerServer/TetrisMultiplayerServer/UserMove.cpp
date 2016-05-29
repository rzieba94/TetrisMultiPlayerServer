#include "stdafx.h"
#include "UserMove.h"


UserMove::UserMove(shared_ptr<RemoteUser> user, MoveType moveType): user(user), moveType(moveType)
{
}


UserMove::~UserMove()
{
}

shared_ptr<RemoteUser> UserMove::getUser()
{
	return user;
}

MoveType UserMove::getMoveType()
{
	return moveType;
}