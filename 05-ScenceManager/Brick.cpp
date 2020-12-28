#include "Brick.h"
#include "Items.h"
#include "QuestionBrick.h"
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
		vyBreak += BRICK_VY_BREAK * dt;
		vxBreak += BRICK_VX_BREAK * dt;

		dyBreak += vyBreak * dt;
		dxBreak += vxBreak * dt;
		BBox = false;
	}
	else {
		vyBreak = -BRICK_VY_START_BREAK * dt;
		xBreak = this->x;
		yBreak = this->y;
	}
	if (SwitchOff)
	{
		BBox = false;
	}
	vector<LPGAMEOBJECT> coEventsResult;
	coEventsResult.clear();

	CalCollisions(colliable_objects, coEventsResult);
	int sizeCo = coEventsResult.size();
	if (sizeCo != 0)
	{
		for (UINT i = 0; i < sizeCo; i++)
		{
			LPGAMEOBJECT e = coEventsResult[i];
			if (dynamic_cast<QuestionBrick*>(e)) // if e->obj is Goomba 
			{
				QuestionBrick* Brick = dynamic_cast<QuestionBrick*>(e);
				StoreItemQBrick = true;
				QBick = Brick;
			}
		}
	}
}

void Brick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (BBox) {
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
}

Brick::Brick(LPGAMEOBJECT item)
{
	this->items = item;
}


