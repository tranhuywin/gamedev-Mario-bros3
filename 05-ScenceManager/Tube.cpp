
#include "Tube.h"

Tube::Tube(float Height, int SprTure)
{
	this->Height = Height;
	this->SprTure = SprTure;
}

void Tube::Render()
{
	//RenderBoundingBox();
	if(SprTure != -1)
		animation_set->at(SprTure)->Render(x, y);
}

void Tube::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + TUBE_BBOX_WIDTH;
	b = y + Height;
}
