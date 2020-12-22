#include "Line.h"
Line::Line(float Width)
{
	this->Width = Width;
}
void Line::Render()
{
}

void Line::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + Width;
	b = y + LINE_BBOX_HEIGHT;
	X_Left = l;
	X_Right = r;
}