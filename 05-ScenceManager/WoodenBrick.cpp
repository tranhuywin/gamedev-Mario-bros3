#include "WoodenBrick.h"
#include "Utils.h"

WoodenBrick::WoodenBrick()
{
	SetState(BRICK_STATE_IDLE);
}
WoodenBrick::WoodenBrick(int typeItem)
{
	typeBrick = BRICK_INCLUDE_ITEM;
	haveItem = true;
	SetState(BRICK_STATE_IDLE);
}
void WoodenBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* obj)
{
	if (typeBrick == BRICK_INCLUDE_ITEM)
	{
		if (isFirstUpdate)
		{
			initX = this->x;
			initY = this->y;
			isFirstUpdate = false;
		}

		if (GetState() == BRICK_STATE_IDLE && (this->x != initX || this->y != initY)) {
			this->x = initX; this->y = initY;
		}

		if (GetState() == BRICK_STATE_MOVE_LEFT_RIGHT)
		{
			if (initX - this->x <= 0)
			{
				SetState(BRICK_STATE_IDLE);
			}
		}
		else if (GetState() == BRICK_STATE_MOVE_RIGHT_RIGHT)
		{
			if (this->x - initX > BRICK_DISTANCE_MOVE)
			{
				SetState(BRICK_STATE_MOVE_LEFT_RIGHT);
			}
		}
		else if (GetState() == BRICK_STATE_MOVE_LEFT_LEFT)
		{
			if (initX - this->x > BRICK_DISTANCE_MOVE)
			{
				SetState(BRICK_STATE_MOVE_RIGHT_LEFT);
			}
		}
		else if (GetState() == BRICK_STATE_MOVE_RIGHT_LEFT)
		{
			if (initX - this->x <= 0)
			{
				SetState(BRICK_STATE_IDLE);
			}
		}

		x += dx;
		y += dy;
		CGameObject::Update(dt);
	}

}
void WoodenBrick::SetState(int state)
{
	switch (state)
	{
	case BRICK_STATE_IDLE:
		this->vy = 0;
		this->vx = 0;
		break;
	case BRICK_STATE_MOVE_LEFT_RIGHT:
		this->vx = -BRICK_VX * dt;
		break;
	case BRICK_STATE_MOVE_RIGHT_RIGHT:
		this->vx = BRICK_VX * dt;
		break;
	case BRICK_STATE_MOVE_LEFT_LEFT:
		this->vx = -BRICK_VX * dt;
		break;
	case BRICK_STATE_MOVE_RIGHT_LEFT:
		this->vx = BRICK_VX * dt;
		break;
	}
	CGameObject::SetState(state);
}
void WoodenBrick::Render()
{
	if(typeBrick == BRICK_INCLUDE_ITEM)
		animation_set->at(0)->Render(x, y);
	//RenderBoundingBox();
}

void WoodenBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + WOODEN_BRICK_WIDTH;
	b = y + WOODEN_BRICK_WIDTH;
}
