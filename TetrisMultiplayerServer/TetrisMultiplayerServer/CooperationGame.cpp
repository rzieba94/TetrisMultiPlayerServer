#include "stdafx.h"
#include "CooperationGame.h"


CooperationGame::CooperationGame(shared_ptr<RemoteUser> ownerUser, int gameId, int playersNumber) : ParentGameEngine(ownerUser, gameId, playersNumber, GameType::cooperation)
{
}


CooperationGame::~CooperationGame()
{
}

void CooperationGame::run()
{

}
