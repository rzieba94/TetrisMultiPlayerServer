#include "stdafx.h"
#include "TetrominoT.h"


TetrominoT::TetrominoT(sf::Vector2i & position, const sf::Color * color) : Tetromino(position, color, TetrominoTypeT)
{
	bricksList.push_front(std::shared_ptr<Brick>(new Brick(position.x, position.y - 1, color)));
	bricksList.push_front(std::shared_ptr<Brick>(new Brick(position.x - 1, position.y, color)));
	bricksList.push_front(std::shared_ptr<Brick>(new Brick(position.x, position.y, color)));
	bricksList.push_front(std::shared_ptr<Brick>(new Brick(position.x + 1, position.y, color)));
}


TetrominoT::~TetrominoT()
{
}
