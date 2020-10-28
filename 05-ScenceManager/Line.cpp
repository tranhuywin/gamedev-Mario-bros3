#include "Line.h"
void Line::Render()
{
	//animation_set->at(5)->Render(x, y);
	RenderBoundingBox();
}

void Line::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + LINE_BBOX_WIDTH;
	b = y + LINE_BBOX_HEIGHT;
}