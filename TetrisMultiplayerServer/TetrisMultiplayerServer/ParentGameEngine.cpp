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
		MoveMsg msg;
		msg.cmd = "move";
		msg.moveType = "DOWN";
		msg.userId =  player->getNick(); //TODO: zmienic na id uzytkownika
		msg.dropCount = 0;
		sf::Packet packet;
		packet << msg.cmd << msg.moveType << msg.userId << msg.dropCount;

		for (shared_ptr<RemoteUser> playerr : usersList)
		{
			playerr->send(packet);
		}
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

void ParentGameEngine::checkPlayersMove()
{
	shared_ptr<UserMove> move;
	if (moveQueue.try_pop(move))
	{
		bool isSuccess = false;
		shared_ptr<RemoteUser> user = move->getUser();
		shared_ptr<Tetromino> activeTetromino = user->getActiveTetromino();
		MoveType moveType = move->getMoveType();
		
		MoveMsg msg;
		msg.cmd = "move";
		msg.userId = user->getNick();
		msg.dropCount = 0;
		
		switch (moveType)
		{
		case DOWN:
			if (activeTetromino->checkColision(notActiveTetrominos, moveType, 10))
			{
				msg.moveType = "DOWN";
				activeTetromino->moveDown();
				isSuccess = true;
			}
			break;
		case LEFT:
			if (activeTetromino->checkColision(notActiveTetrominos, moveType, 10))
			{
				msg.moveType = "LEFT";
				activeTetromino->moveLeft();
				isSuccess = true;
			}
			break;
		case RIGHT:
			if (activeTetromino->checkColision(notActiveTetrominos, moveType, 10))
			{
				msg.moveType = "RIGHT";
				activeTetromino->moveRight();
				isSuccess = true;
			}
			break;
		case DROP:
			int dropCount = activeTetromino->getDropCount(notActiveTetrominos, 10);
			if (dropCount > 0)
			{
				msg.moveType = "DROP";
				msg.dropCount = dropCount;
				activeTetromino->drop(dropCount);
				isSuccess = true;
			}
			break;
		}

		if (isSuccess)
		{
			sf::Packet packet;
			packet << msg.cmd << msg.moveType << msg.userId << msg.dropCount;

			for (shared_ptr<RemoteUser> playerr : usersList)
			{
				playerr->send(packet);
			}
		}	
	}
}

void ParentGameEngine::addPlayer(shared_ptr<RemoteUser> player)
{
	usersList.push_back(player);
}

void ParentGameEngine::registerMove(shared_ptr<UserMove> userMove)
{
	moveQueue.push(userMove);
}
