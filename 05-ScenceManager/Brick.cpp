#include "Brick.h"

void Brick::Render()
{
	animation_set->at(0)->Render(x, y);
}

void Brick::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
}

void Brick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + 16;
	b = y + 16;
}
