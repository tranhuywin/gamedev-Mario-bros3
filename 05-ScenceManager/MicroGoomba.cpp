#include "MicroGoomba.h"
#include "Game.h"
#include "Utils.h"

void MicroGoomba::setPositionFalling(float xStartFalling, float yStartFalling)
{
	this->xStartFalling = xStartFalling; 
	this->yStartFalling = yStartFalling;
	this->x = xStartFalling;
	this->y = yStartFalling;
	vx = 0.15f;
}

MicroGoomba::MicroGoomba()
{
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(3));
	SetState(MICRO_GOOMBA_STATE_WAIT_FALLING);
}

void MicroGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (BBox)
	{
		left = x;
		top = y;
		right = x + MICRO_GOOMBA_BBOX;
		bottom = y + MICRO_GOOMBA_BBOX;
	}
}

void MicroGoomba::Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (GetState() == MICRO_GOOMBA_STATE_FALLING)
	{
		vy = MICRO_GOOMBA_VY_FALLING * dt;

		if (this->x < xStartFalling - MICRO_GOOMBA_DISTANCE_X_FALLING)
			vx = MICRO_GOOMBA_VX_FALLING * dt;
		else if (this->x > xStartFalling + MICRO_GOOMBA_DISTANCE_X_FALLING)
			vx = -MICRO_GOOMBA_VX_FALLING * dt;
	}
	else if (GetState() == MICRO_GOOMBA_STATE_STICK_TO_MARIO)
	{
		//this->y = mario->y;
		if(this->x > mario->x + MARIO_BIG_BBOX_WIDTH + 5)
			vx = mario->vx - MICRO_GOOMBA_VX_STICK * dt;
		else if(this->x < mario->x - MICRO_GOOMBA_BBOX)
			vx = mario->vx + MICRO_GOOMBA_VX_STICK * dt;

		if (this->y > mario->y + MARIO_BIG_BBOX_HEIGHT - MICRO_GOOMBA_BBOX)
			vy = mario->vy - (MARIO_GRAVITY + MICRO_GOOMBA_VY_STICK) * dt;
		else if (this->y < mario->y)
			vy = mario->vy + MICRO_GOOMBA_VY_STICK * dt;
	}
	float YBotScreen = CGame::GetInstance()->GetCamPosY() + CGame::GetInstance()->GetScreenHeight() + 48;
	if (this->y > YBotScreen)
		SetState(MICRO_GOOMBA_STATE_WAIT_FALLING);
	if (!mario->isSticked && GetState() == MICRO_GOOMBA_STATE_STICK_TO_MARIO)
	{
		SetState(MICRO_GOOMBA_STATE_UNSTICK);
	}
	else if (GetState() == MICRO_GOOMBA_STATE_UNSTICK)
	{
		vy += MICRO_GOOMBA_VY_UNSTICK * dt;
		if(vx < 0.0f)
			vx += MICRO_GOOMBA_VX_UNSTICK * dt;
		else
			vx -= MICRO_GOOMBA_VX_UNSTICK * dt;
	}
	
	x += dx;
	y += dy;
	CGameObject::Update(dt);
}

void MicroGoomba::Render()
{
	if(isRender)
		animation_set->at(8)->Render(x, y);
}

void MicroGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		case MICRO_GOOMBA_STATE_FALLING:
			BBox = true;
			isRender = true;
			break;
		case MICRO_GOOMBA_STATE_WAIT_FALLING:
			BBox = false;
			isRender = false;
			break;
		case MICRO_GOOMBA_STATE_STICK_TO_MARIO:
			BBox = false;
			vx = mario->vx + MICRO_GOOMBA_VX_STICK * dt;
			vy = mario->vy + MICRO_GOOMBA_VY_STICK * dt;
			this->y = mario->y;
			break;
		case MICRO_GOOMBA_STATE_UNSTICK:
			vy = -MICRO_GOOMBA_DEFLECT_VY_UNSTICK * dt;
			if(this->x < mario->x + MARIO_BIG_BBOX_WIDTH/2)
				vx = -MICRO_GOOMBA_DEFLECT_VX_UNSTICK * dt;
			else
				vx = MICRO_GOOMBA_DEFLECT_VX_UNSTICK * dt;
			break;
	}
}
