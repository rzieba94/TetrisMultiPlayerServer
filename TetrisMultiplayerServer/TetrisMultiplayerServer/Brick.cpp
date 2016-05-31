#include "stdafx.h"
#include "Brick.h"


Brick::Brick(sf::Vector2i & position, const sf::Color * color) : color(color), position(position)
{
}

Brick::Brick(int positionX, int positionY, const sf::Color * color) : position(positionX, positionY), color(color)
{
}

Brick::~Brick()
{
}

void Brick::move(sf::Vector2i & moveVector)
{
	this->position.x += moveVector.x;
	this->position.y += moveVector.y;
}

const sf::Vector2i Brick::getPosition()const
{
	return position;
}

void Brick::setPosition(sf::Vector2i & newPosition)
{
	this->position = newPosition;
}