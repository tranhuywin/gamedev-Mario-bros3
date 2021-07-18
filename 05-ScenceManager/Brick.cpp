#include "Brick.h"
#include "Items.h"
#include "QuestionBrick.h"
#include "Utils.h"
void Brick::Render()
{
	if(!SwitchOff && !isDispread)
		if(!IsBreaked)
			animation_set->at(0)->Render(x, y);
		else
		{
			animation_set->at(1)->Render(xBreak + dxBreak, yBreak + dyBreak);
			animation_set->at(1)->Render(xBreak - dxBreak, yBreak + dyBreak);
			animation_set->at(1)->Render(xBreak + dxBreak / 2, yBreak + dyBreak / 2);
			animation_set->at(1)->Render(xBreak - dxBreak / 2, yBreak + dyBreak / 2);
		}
	else {
		//RenderBoundingBox();
	}
}

void Brick::Update(ULONGLONG dt, vector<LPGAMEOBJECT>* colliable_objects)
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

	if (SwitchOff && !count)
	{
		BBox = false;
		StartSleep();
		count = true;
	}
	if (GetTickCount64() - Sleep_start > BRICK_TIME_OFF && SwitchOff)
	{
		Sleep_start = 0;
		Sleep = 0;
		SwitchOff = false;
		BBox = true;
	}

	vector<LPGAMEOBJECT> coEventsResult;
	coEventsResult.clear();

	CalCollisions(colliable_objects, coEventsResult);
	unsigned int sizeCo = coEventsResult.size();
	if (sizeCo != 0)
	{
		for (unsigned int i = 0; i < sizeCo; i++)
		{
			LPGAMEOBJECT e = coEventsResult[i];
			if (dynamic_cast<QuestionBrick*>(e))
			{
				QuestionBrick* brick = dynamic_cast<QuestionBrick*>(e);
				StoreItemQBrick = true;
				QBick = brick;
				if (!this->IsBreaked)
					this->y = brick->y;
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
			l = 0.0f; t = 0.0f;
			r = 0.0f;
			b = 0.0f;
		}
	}
}

Brick::Brick(LPGAMEOBJECT item)
{
	this->items = item;
}


