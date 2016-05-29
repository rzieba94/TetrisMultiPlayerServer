#include "stdafx.h"
#include "ParentGameEngine.h"

using namespace std;

ParentGameEngine::ParentGameEngine(shared_ptr<RemoteUser> ownerUser) : notActiveTetrominos(sf::Vector2i(100, 200)), tetrominoFactory(), moveQueue()
{
	this->startTime = clock();
	this->usersList.push_back(ownerUser);
}


ParentGameEngine::~ParentGameEngine()
{
}


void ParentGameEngine::startThread()
{
	gameThread = thread(&ParentGameEngine::run, this);
}

void ParentGameEngine::checkFrameTime()
{
	if (clock() - startTime > FRAME_DURATION)
	{
		moveDownAllActiveBlocks();
		startTime = clock();
	}
}

void ParentGameEngine::moveDownAllActiveBlocks()
{
	for (shared_ptr<RemoteUser> player : usersList)
	{
		player->getActiveTetromino()->moveDown();
	}
}

bool ParentGameEngine::placeNewTetromino(shared_ptr<RemoteUser> player)
{
	shared_ptr<Tetromino> newTetromino = tetrominoFactory.getRandomTetromino(player->getStartPosition());
	if (!newTetromino->checkColision(notActiveTetrominos, DOWN, 200))
	{
		player->setActiveTetromino(newTetromino);
		return true;
	}
	else
	{
		return false;
	}
}

int ParentGameEngine::getLineToClear()
{
	list<shared_ptr<Brick>> bricksList = notActiveTetrominos.getBricksList();
	vector <int> bricksInRowsCounter(20, 0);

	for (shared_ptr<Brick> brick : notActiveTetrominos.getBricksList())
	{
		sf::Vector2i brickPosition = brick->getPosition();
		int brickRow = brickPosition.y;
		bricksInRowsCounter[brickRow]++;
	}

	for (int lineNo = 0; lineNo < bricksInRowsCounter.size(); lineNo++)
	{
		if (bricksInRowsCounter[lineNo] == 10)
		{
			return lineNo;
		}
	}
	return -1;
}

bool ParentGameEngine::checkForInactiveBlock(shared_ptr<RemoteUser> player)
{
	shared_ptr<Tetromino> activeTetromino = player->getActiveTetromino();
	if (activeTetromino->checkColision(notActiveTetrominos, DOWN, 200))
	{
		notActiveTetrominos.addTetrisShape(activeTetromino);
		return true;
	}
	else
	{
		return false;
	}
}
