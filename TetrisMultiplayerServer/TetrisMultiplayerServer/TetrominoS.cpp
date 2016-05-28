#include "stdafx.h"
#include "TetrominoS.h"


TetrominoS::TetrominoS(sf::Vector2i & position, const sf::Color * color) : Tetromino(position, color)
{
	bricksList.push_front(std::shared_ptr<Brick>(new Brick(position.x - 1, position.y - 1, color)));
	bricksList.push_front(std::shared_ptr<Brick>(new Brick(position.x, position.y - 1, color)));
	bricksList.push_front(std::shared_ptr<Brick>(new Brick(position.x - (1 * 2), position.y, color)));
	bricksList.push_front(std::shared_ptr<Brick>(new Brick(position.x - 1, position.y, color)));
}


TetrominoS::~TetrominoS()
{
}
