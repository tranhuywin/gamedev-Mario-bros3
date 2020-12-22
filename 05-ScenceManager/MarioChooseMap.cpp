#include "MarioChooseMap.h"

MarioChooseMap::MarioChooseMap(float x, float y)
{
	this->x = x;
	this->y = y;
}

void MarioChooseMap::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	CGameObject::Update(dt);
	x += dx;
	y += dy;
}

void MarioChooseMap::Render()
{
	animation_set->at(0)->Render(x, y);
}

void MarioChooseMap::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case MARIO_STATE_IDLE:
		vx = 0;
		vy = 0;
		break;
	case MARIO_STATE_UP:
		vy = -MARIO_SPEED;
		break;
	case MARIO_STATE_DOWN:
		vy = MARIO_SPEED;
		break;
	case MARIO_STATE_LEFT:
		vx = -MARIO_SPEED;
		break;
	case MARIO_STATE_RIGHT:
		vx = MARIO_SPEED;
		break;
	}
}

void MarioChooseMap::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + 16;
	bottom = y + 16;
	
}
