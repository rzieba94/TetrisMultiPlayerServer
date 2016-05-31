#include "stdafx.h"
#include "Tetromino.h"

Tetromino::Tetromino(sf::Vector2i & position, const sf::Color * color, TetrominoType tetrominoType) : TetrisShape(position), tetrominoType(tetrominoType)
{
	this->color = color;
}

Tetromino::~Tetromino()
{
}

void Tetromino::rotate()
{
	for (shared_ptr<Brick> brick : bricksList)
	{
		int newX = position.x + position.y - brick->getPosition().y - 1;
		int newY = brick->getPosition().x + position.y - position.x;
		brick->setPosition(sf::Vector2i(newX, newY));
	}
}

void Tetromino::moveRight()
{
	position.x += 1;
	for (shared_ptr<Brick> brick : bricksList)
	{
		brick->move(sf::Vector2i(1, 0));
	}
}

void Tetromino::moveLeft()
{
	position.x -= 1;
	for (shared_ptr<Brick> brick : bricksList)
	{
		brick->move(sf::Vector2i(-1, 0));
	}
}

void Tetromino::moveDown()
{
	position.y += 1;
	for (shared_ptr<Brick> brick : bricksList)
	{
		brick->move(sf::Vector2i(0, 1));
	}
}

void Tetromino::drop(int rowsCount)
{
	for (shared_ptr<Brick> brick : bricksList)
	{
		brick->move(sf::Vector2i(0, 1 * rowsCount));
	}
}

int Tetromino::getDropCount(TetrisShape & tetrisShape, int boardWidth)
{
	list<shared_ptr<Brick>> otherBricksList = tetrisShape.getBricksList();
	
	list<sf::Vector2i> brickPositions;
	for (shared_ptr<Brick> brick : bricksList)
	{
		brickPositions.push_back(brick->getPosition());
	}
	
	int dropCounter;
	for (dropCounter = 0; dropCounter < 20; dropCounter++)
	{
		list<sf::Vector2i>::iterator it;

		for (it = brickPositions.begin(); it != brickPositions.end(); ++it)
		{
			it->y += 1;
			if (checkColision(*it, boardWidth, otherBricksList))
			{
				return dropCounter;
			}
		}
	}

	return dropCounter;
}

bool Tetromino::checkColision(TetrisShape & tetrisShape, MoveType moveType, int boardWidth)
{
	sf::Vector2i moveVector;
	list<shared_ptr<Brick>> otherBricksList = tetrisShape.getBricksList();
	switch (moveType)
	{
	case DOWN:
		moveVector = sf::Vector2i(0, 1);
		break;
	case LEFT:
		moveVector = sf::Vector2i(-1, 0);
		break;
	case RIGHT:
		moveVector = sf::Vector2i(1, 0);
		break;
	}
	for (shared_ptr<Brick> brick : bricksList)
	{
		sf::Vector2i currentBrickPosition = brick->getPosition();
		if (moveType == ROTATE)
		{
			int newX = position.x + position.y - brick->getPosition().y - 1;
			int newY = brick->getPosition().x + position.y - position.x;
			currentBrickPosition = sf::Vector2i(newX, newY);
		}
		else
		{
			currentBrickPosition.x += moveVector.x;
			currentBrickPosition.y += moveVector.y;
		}
		if (checkColision(currentBrickPosition, boardWidth, otherBricksList))
		{
			return true;
		}
	}
	return false;
}

bool Tetromino::checkColision(sf::Vector2i currentBrickPosition, int boardWidth, list<shared_ptr<Brick>> otherBricksList)
{
	if (currentBrickPosition.x < 0 || currentBrickPosition.x >= boardWidth || currentBrickPosition.y >= 20) // TODO: wysokosc planszy powinna byc stala, nie zahardcodowana
	{
		return true;
	}
	if (!otherBricksList.empty())
	{
		for (shared_ptr<Brick> otherBrick : otherBricksList)
		{
			sf::Vector2i otherBrickPosition = otherBrick->getPosition();
			if (currentBrickPosition.x == otherBrickPosition.x && currentBrickPosition.y == otherBrickPosition.y)
			{
				return true;
			}
		}
	}
	return false;
}

void Tetromino::clearLine(int lineNumber)
{
	if (lineNumber != -1)
	{
		list<shared_ptr<Brick>>::iterator it = bricksList.begin();
		while (it != bricksList.end())
		{
			int brickPositionY = (*it)->getPosition().y;
			int clearedLine = lineNumber * 1;
			if (brickPositionY == clearedLine)
			{
				it = bricksList.erase(it);
			}
			else
			{
				if (brickPositionY < clearedLine)
				{
					(*it)->move(sf::Vector2i(0, 1));
				}
				it++;
			}
		}
	}
}

list<shared_ptr<Brick>> Tetromino::getBricksList()
{
	return bricksList;
}

bool Tetromino::isEmptyBricksList()
{
	return getBricksList().empty();
}

TetrominoType Tetromino::getTetrominoType()
{
	return tetrominoType;
}

sf::Vector2i Tetromino::getPosition()
{
	return position;
}