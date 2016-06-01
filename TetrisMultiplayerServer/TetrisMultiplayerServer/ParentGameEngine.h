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
	int gameId;
	list<shared_ptr<RemoteUser>> usersList;
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
	shared_ptr<TetrominosGroup> getAllOtherTetrominos(shared_ptr<RemoteUser> user);

	int columnsNumber;
	GameType gameType;
	int playersNumber;
	
	TetrominoFactory tetrominoFactory;
	thread gameThread;
	concurrency::concurrent_queue<shared_ptr<UserMove>> moveQueue;
	clock_t startTime;
	shared_ptr<TetrominosGroup> notActiveTetrominos;

	static const int FRAME_DURATION = 1000;
};

