#include "stdafx.h"
#include "TetrominoL.h"


TetrominoL::TetrominoL(sf::Vector2i & position, const sf::Color * color) : Tetromino(position, color, TetrominoTypeL)
{
	bricksList.push_front(std::shared_ptr<Brick>(new Brick(position.x - (1 * 2), position.y, color)));
	bricksList.push_front(std::shared_ptr<Brick>(new Brick(position.x - 1, position.y, color)));
	bricksList.push_front(std::shared_ptr<Brick>(new Brick(position.x, position.y, color)));
	bricksList.push_front(std::shared_ptr<Brick>(new Brick(position.x, position.y - 1, color)));
}


TetrominoL::~TetrominoL()
{
}
