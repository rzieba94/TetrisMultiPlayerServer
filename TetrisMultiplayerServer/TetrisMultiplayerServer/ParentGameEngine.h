#pragma once
#include <thread>
#include "TetrominosGroup.h"
#include "TetrominoFactory.h"
#include <memory>
#include <thread>
#include <ctime>
#include "RemoteUser.h"
#include <concurrent_queue.h>

class ParentGameEngine
{
public:
	ParentGameEngine(shared_ptr<RemoteUser>);
	virtual ~ParentGameEngine();
	void startThread();
protected:
	virtual void run() = 0;
	void checkFrameTime();
	void moveDownAllActiveBlocks();
	bool placeNewTetromino(shared_ptr<RemoteUser> player);
	bool checkForInactiveBlock(shared_ptr<RemoteUser> player);
	int getLineToClear();

	TetrominoFactory tetrominoFactory;
	thread gameThread;
	concurrent_queue<Tetromino> moveQueue;
	clock_t startTime;
	TetrominosGroup notActiveTetrominos;
	list<shared_ptr<RemoteUser>> usersList;
	static const int FRAME_DURATION = 1000;
};

