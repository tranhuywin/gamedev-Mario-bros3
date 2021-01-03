#include "Grid.h"

Grid::Grid(float xGrid, float yGrid, float Height, float Width)
{
	this->XGrid = xGrid;
	this->YGrid = yGrid;
	this->Height = Height;
	this->Width = Width;
}

void Grid::Add(LPGAMEOBJECT object)
{
	this->objects.push_back(object);
}

LPGAMEOBJECT Grid::Move(LPGAMEOBJECT object)
{
	if (object->x > XGrid && object->x <= XGrid + Width && object->y >YGrid && object->y <= YGrid + Height)
		return NULL;
	else
		return object;
}
