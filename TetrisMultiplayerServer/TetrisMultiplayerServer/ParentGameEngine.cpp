#include "stdafx.h"
#include "ParentGameEngine.h"

using namespace std;

ParentGameEngine::ParentGameEngine(shared_ptr<RemoteUser> ownerUser, int gameId, int playersNumber, GameType gameType, int columnsNumber)
	: notActiveTetrominos(sf::Vector2i(columnsNumber/2, 10)), tetrominoFactory(), moveQueue(), gameId(gameId), playersNumber(playersNumber), gameType(gameType), columnsNumber(columnsNumber)
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
		shared_ptr<Tetromino> activeTetromino = player->getActiveTetromino();
		if (!activeTetromino->isColision(notActiveTetrominos, DOWN, columnsNumber))
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
}

bool ParentGameEngine::placeNewTetromino(shared_ptr<RemoteUser> player)
{
	shared_ptr<Tetromino> newTetromino = tetrominoFactory.getRandomTetromino(player->getStartPosition());
	if (!newTetromino->isColision(notActiveTetrominos, DOWN, columnsNumber))
	{
		player->setActiveTetromino(newTetromino);

		PlaceTetromino msg;
		msg.cmd = Cmds::placeTetromino;
		msg.tetrominoType = newTetromino->getTetrominoType();
		msg.userId = player->getNick();
		sf::Vector2i newTetrominoPosition = newTetromino->getPosition();
		msg.positionX = newTetrominoPosition.x;
		msg.positionY = newTetrominoPosition.y;
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
		if (bricksInRowsCounter[lineNo] == columnsNumber)
		{
			lineNumber = lineNo;
			break;
		}
	}

	if (lineNumber != -1)
	{
		AddScore msgAddScore;
		msgAddScore.cmd = Cmds::addScore;
		sf::Packet packetAddScore;

		ClearLine msgClrLine;
		msgClrLine.cmd = Cmds::clearLine;
		msgClrLine.lineNumber = lineNumber;
		sf::Packet packetClrLine;
		packetClrLine << msgClrLine.cmd << msgClrLine.lineNumber;

		for (shared_ptr<RemoteUser> player : usersList)
		{
			int previousScore = player->getScore();
			player->setScore(previousScore + 100);
			msgAddScore.score = player->getScore();
			packetAddScore << msgAddScore.cmd << msgAddScore.score;

			player->send(packetAddScore);
			player->send(packetClrLine);
		}
	}

	return lineNumber;
}

bool ParentGameEngine::checkForInactiveBlock(shared_ptr<RemoteUser> player)
{
	shared_ptr<Tetromino> activeTetromino = player->getActiveTetromino();
	if (activeTetromino->isColision(notActiveTetrominos, DOWN, columnsNumber))
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
			if (activeTetromino->isColision(notActiveTetrominos, moveType, columnsNumber))
			{
				msg.moveType = MoveType::DOWN;
				activeTetromino->moveDown();
				isSuccess = true;
			}
			break;
		case LEFT:
			if (activeTetromino->isColision(notActiveTetrominos, moveType, columnsNumber))
			{
				msg.moveType = MoveType::LEFT;
				activeTetromino->moveLeft();
				isSuccess = true;
			}
			break;
		case RIGHT:
			if (activeTetromino->isColision(notActiveTetrominos, moveType, columnsNumber))
			{
				msg.moveType = MoveType::RIGHT;
				activeTetromino->moveRight();
				isSuccess = true;
			}
			break;
		case DROP:
			int dropCount = activeTetromino->getDropCount(notActiveTetrominos, columnsNumber);
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
			cout << "check" << endl;
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
	player->setScore(0);
}