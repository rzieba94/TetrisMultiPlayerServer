#include "stdafx.h"
#include "TetrominosGroup.h"


TetrominosGroup::TetrominosGroup(sf::Vector2i & position) : TetrisShape(position), tetrisShapesList()
{
}


TetrominosGroup::~TetrominosGroup()
{
}

void TetrominosGroup::clearLine(int lineNumber)
{
	for (shared_ptr<TetrisShape> tetrisShape : tetrisShapesList)
	{
		tetrisShape->clearLine(lineNumber);
	}
	deleteUnusedShapes();
}

list<shared_ptr<Brick>> TetrominosGroup::getBricksList()
{
	list<shared_ptr<Brick>> bricksList;

	for (shared_ptr<TetrisShape> tetrisShape : tetrisShapesList)
	{
		for (shared_ptr<Brick> brick : tetrisShape->getBricksList())
		{
			bricksList.push_back(brick);
		}
	}

	return bricksList;
}

void TetrominosGroup::addTetrisShape(shared_ptr<TetrisShape> tetrisShape) 
{
	tetrisShapesList.push_back(tetrisShape);
}

void TetrominosGroup::deleteTetrisShape(shared_ptr<TetrisShape> shapeToDelete)
{
	tetrisShapesList.remove(shapeToDelete);
}

bool TetrominosGroup::isEmptyBricksList()
{
	return getBricksList().empty();
}

void TetrominosGroup::deleteUnusedShapes()
{
	list<shared_ptr<TetrisShape>>::iterator it = tetrisShapesList.begin();
	while (it != tetrisShapesList.end())
	{
		if ((*it)->isEmptyBricksList())
		{
			it = tetrisShapesList.erase(it);
		}
		else if (it != tetrisShapesList.end())
		{
			it++;
		}
	}
}
