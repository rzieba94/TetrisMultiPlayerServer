#pragma once
#include <thread>
#include "TetrominosGroup.h"
#include "TetrominoFactory.h"
#include <memory>
#include <thread>
#include <ctime>
#include "RemoteUser.h"
#include <concurrent_queue.h>
#include "UserMove.h"
#include "RemoteCmds.h"
#include "GameType.h"
#include "Cmds.h"

class ParentGameEngine
{
public:
	ParentGameEngine(shared_ptr<RemoteUser> ownerUser, int gameId, int playersNumber, GameType gameType, int columnsNumber);
	virtual ~ParentGameEngine();
	void startThread();
	void registerMove(shared_ptr<UserMove> userMove);
	void addPlayer(shared_ptr<RemoteUser> player);
protected:
	void checkPlayersMove();
	virtual void run() = 0;
	void checkFrameTime();
	void moveDownAllActiveBlocks();
	virtual bool placeNewTetromino(shared_ptr<RemoteUser> player);
	virtual bool checkForInactiveBlock(shared_ptr<RemoteUser> player);
	int getLineToClear();
	void sendStartGameMsg();
	void sendEndGameMsg(shared_ptr<RemoteUser> player);

	int columnsNumber;
	GameType gameType;
	int playersNumber;
	int gameId;
	TetrominoFactory tetrominoFactory;
	thread gameThread;
	concurrency::concurrent_queue<shared_ptr<UserMove>> moveQueue;
	clock_t startTime;
	TetrominosGroup notActiveTetrominos;
	list<shared_ptr<RemoteUser>> usersList;
	static const int FRAME_DURATION = 1000;
};

