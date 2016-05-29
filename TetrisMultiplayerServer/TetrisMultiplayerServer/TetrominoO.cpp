#include "stdafx.h"
#include "TetrominoO.h"


TetrominoO::TetrominoO(sf::Vector2i & position, const sf::Color * color) : Tetromino(position, color, TetrominoTypeO)
{
	bricksList.push_front(std::shared_ptr<Brick>(new Brick(position.x - 1, position.y - 1, color)));
	bricksList.push_front(std::shared_ptr<Brick>(new Brick(position.x, position.y - 1, color)));
	bricksList.push_front(std::shared_ptr<Brick>(new Brick(position.x - 1, position.y, color)));
	bricksList.push_front(std::shared_ptr<Brick>(new Brick(position.x, position.y, color)));
}


TetrominoO::~TetrominoO()
{
}
