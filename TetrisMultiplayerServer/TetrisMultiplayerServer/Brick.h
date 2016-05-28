#pragma once
#include "SFML\System\Vector2.hpp"
#include "SFML\Graphics\RectangleShape.hpp"

class Brick
{
public:
	Brick(sf::Vector2i & position, const sf::Color * color);
	Brick(int positionX, int positionY, const sf::Color * color);
	~Brick();
	void move(sf::Vector2i & moveVector);
	void setPosition(sf::Vector2i & newPosition);
	const sf::Vector2i getPosition()const;
private: 
	sf::Vector2i position;
	const sf::Color * color;
};

