#include "Ground.h"
void Ground::Render()
{
	//animation_set->at(1)->Render(x, y);
	RenderBoundingBox();
}

void Ground::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + Width;
	b = y + GROUND_BBOX_HEIGHT;
}	