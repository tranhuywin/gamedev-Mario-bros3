#include "Brick.h"
#include "Items.h"
void Brick::Render()
{
	if(!SwitchOff)
		if(!IsBreaked)
			animation_set->at(0)->Render(x, y);
		else
		{
			animation_set->at(1)->Render(xBreak + dxBreak, yBreak + dyBreak);
			animation_set->at(1)->Render(xBreak - dxBreak, yBreak + dyBreak);
			animation_set->at(1)->Render(xBreak + dxBreak / 2, yBreak + dyBreak / 2);
			animation_set->at(1)->Render(xBreak - dxBreak / 2, yBreak + dyBreak / 2);
		}
}

void Brick::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	if (IsBreaked) {
		vyBreak += BRICK_VX_BREAK * dt;
		vxBreak += BRICK_VY_BREAK * dt;

		dyBreak += vyBreak * dt;
		dxBreak += vxBreak * dt;
		x = -200; y = -200; vy = 0;
	}
	else {
		vyBreak = -BRICK_VY_START_BREAK * dt;
		xBreak = this->x;
		yBreak = this->y;
	}
}

void Brick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	if (!IsBreaked) {
		r = x + BRICK_BBOX_WIDTH;
		b = y + BRICK_BBOX_WIDTH;
	}
	else {
		r = x;
		b = y;
	}
}

Brick::Brick(LPGAMEOBJECT item)
{
	this->items = item;
}


