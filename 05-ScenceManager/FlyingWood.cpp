#include "FlyingWood.h"

void FlyingWood::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	vx = -FLYING_WOOD_VX * dt;
	if (IsCollMario)
	{
		vy = FLYING_WOOD_VY * dt;
		vx = 0;
	}
	x += dx;
	y += dy;
	CGameObject::Update(dt);
}

void FlyingWood::Render()
{
	animation_set->at(0)->Render(x, y);
}

void FlyingWood::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + FLYING_WOOD_BBOX_WIDTH;
	b = y + FLYING_WOOD_BBOX_HEIGHT;
}
