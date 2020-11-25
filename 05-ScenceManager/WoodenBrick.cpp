#include "WoodenBrick.h"

void WoodenBrick::Render()
{
	//animation_set->at(0)->Render(x, y);
}

void WoodenBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + WOODEN_BRICK_WIDTH;
	b = y + WOODEN_BRICK_WIDTH;
}
