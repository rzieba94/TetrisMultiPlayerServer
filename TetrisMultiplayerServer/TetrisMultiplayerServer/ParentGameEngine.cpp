#include "stdafx.h"
#include "ParentGameEngine.h"

using namespace std;

ParentGameEngine::ParentGameEngine(shared_ptr<RemoteUser> ownerUser, int gameId, int playersNumber, GameType gameType)
	: notActiveTetrominos(sf::Vector2i(5, 10)), tetrominoFactory(), moveQueue(), gameId(gameId), playersNumber(playersNumber), gameType(gameType)
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
		msg.cmd = Cmds::move;
		msg.moveType = MoveType::DOWN;
		msg.userId = player->getNick(); //TODO: zmienic na id uzytkownika
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
	if (!newTetromino->checkColision(notActiveTetrominos, DOWN, 10))
	{
		player->setActiveTetromino(newTetromino);

		PlaceTetromino msg;
		msg.cmd = Cmds::placeTetromino;
		msg.tetrominoType = newTetromino->getTetrominoType();
		msg.userId = player->getNick();
		msg.positionX = newTetromino->getPosition->x;
		msg.positionY = newTetromino->getPosition->y;
		sf::Packet packet;
		packet << msg.cmd << msg.tetrominoType << msg.userId << msg.positionX << msg.positionY;

		for (shared_ptr<RemoteUser> playerr : usersList)
		{
			playerr->send(packet);
		}
		return true;
	}
	else
	{
		return false;
	}
}

int ParentGameEngine::getLineToClear()
{
	int lineNumber = -1;

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
			lineNumber = lineNo;
			break;
		}
	}

	if (lineNumber != -1)
	{
		ClearLine msg;
		msg.cmd = Cmds::clearLine;
		msg.lineNumber = lineNumber;
		sf::Packet packet;
		packet << msg.cmd << msg.lineNumber;
		for (shared_ptr<RemoteUser> playerr : usersList)
		{
			playerr->send(packet);
		}
	}

	return lineNumber;
}

bool ParentGameEngine::checkForInactiveBlock(shared_ptr<RemoteUser> player)
{
	shared_ptr<Tetromino> activeTetromino = player->getActiveTetromino();
	if (activeTetromino->checkColision(notActiveTetrominos, DOWN, 10))
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
		msg.cmd = Cmds::move;
		msg.userId = user->getNick();
		msg.dropCount = 0;
		
		switch (moveType)
		{
		case DOWN:
			if (activeTetromino->checkColision(notActiveTetrominos, moveType, 10))
			{
				msg.moveType = MoveType::DOWN;
				activeTetromino->moveDown();
				isSuccess = true;
			}
			break;
		case LEFT:
			if (activeTetromino->checkColision(notActiveTetrominos, moveType, 10))
			{
				msg.moveType = MoveType::LEFT;
				activeTetromino->moveLeft();
				isSuccess = true;
			}
			break;
		case RIGHT:
			if (activeTetromino->checkColision(notActiveTetrominos, moveType, 10))
			{
				msg.moveType = MoveType::RIGHT;
				activeTetromino->moveRight();
				isSuccess = true;
			}
			break;
		case DROP:
			int dropCount = activeTetromino->getDropCount(notActiveTetrominos, 10);
			if (dropCount > 0)
			{
				msg.moveType = MoveType::DROP;
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

void ParentGameEngine::sendStartGameMsg()
{
	StartGame msg;
	msg.cmd = Cmds::startGame;
	msg.gameType = gameType;
	msg.playersNumber = playersNumber;
	for (shared_ptr<RemoteUser> player : usersList)
	{
		msg.userIds += player->getNick() + ";";
	}
	sf::Packet packet;
	packet << msg.cmd << msg.gameType << msg.playersNumber << msg.userIds;

	for (shared_ptr<RemoteUser> player : usersList)
	{
		player->send(packet);
	}
}

void ParentGameEngine::sendEndGameMsg(shared_ptr<RemoteUser> player)
{
	SimpleCommand msg;
	msg.cmd = Cmds::endGame;
	sf::Packet packet;
	packet << msg.cmd;
	player->send(packet);
}