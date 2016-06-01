#pragma once
#include "TetrisShape.h"
#include "Brick.h"
#include <list>
#include <memory>
#include <SFML/Graphics/Color.hpp>
#include "TetrominoType.h"

class Tetromino : public TetrisShape
{
public:
	virtual ~Tetromino();
	void rotate();
	void moveRight();
	void moveLeft();
	void moveDown();
	void drop(int rowsCount);
	int getDropCount(shared_ptr<TetrisShape> tetrisShape, int boardWidth);
	bool isColision(shared_ptr<TetrisShape> tetrisShape, MoveType moveType, int boardWidth);
	void clearLine(int lineNumber);
	list<shared_ptr<Brick>> getBricksList();
	TetrominoType getTetrominoType();
	sf::Vector2i getPosition();
	bool isEmptyBricksList();
protected:
	Tetromino(sf::Vector2i & position, const sf::Color * color, TetrominoType tetrominoType);
	TetrominoType tetrominoType;
	const sf::Color * color;
	list<shared_ptr<Brick>> bricksList;
private:
	bool isColision(sf::Vector2i currentBrickPosition, int boardWidth, list <shared_ptr<Brick>> otherBricksList);
};

