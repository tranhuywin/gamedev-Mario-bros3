#include "Ground.h"
Ground::Ground(float Width)
{
	this->Width = Width;
}
void Ground::Render()
{
	RenderBoundingBox();
}

void Ground::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + Width;
	b = y + GROUND_BBOX_HEIGHT;
}	