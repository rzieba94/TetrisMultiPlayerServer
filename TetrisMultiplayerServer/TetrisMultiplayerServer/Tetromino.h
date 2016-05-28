#pragma once
#include "TetrisShape.h"
#include "Brick.h"
#include <list>
#include <memory>
#include <SFML/Graphics/Color.hpp>

class Tetromino : public TetrisShape
{
public:
	virtual ~Tetromino();
	void rotate();
	void moveRight();
	void moveLeft();
	void moveDown();
	void drop(int rowsCount);
	int getDropCount(TetrisShape & tetrisShape, int boardWidth);
	bool checkColision(TetrisShape & tetrisShape, MoveType moveType, int boardWidth);
	void clearLine(int lineNumber);
	list<shared_ptr<Brick>> getBricksList();
	bool isEmptyBricksList();
protected:
	Tetromino(sf::Vector2i & position, const sf::Color * color);
	const sf::Color * color;
	list<shared_ptr<Brick>> bricksList;
private:
	bool checkColision(sf::Vector2i currentBrickPosition, int boardWidth, list <shared_ptr<Brick>> otherBricksList);
};

