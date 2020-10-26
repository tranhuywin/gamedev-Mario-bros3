#include "Mario.h"

#include "debug.h"
void CMario::Update(DWORD dt)
{
	CGameObject::Update(dt);

	// simple fall down
	vy += MARIO_GRAVITY * dt;
	//DebugOut(L"[INFO] Vy: %d\n", dt);
	if (y > 100) 
	{
		vy = 0;
		y = 100.0f;
	}
	//IsRuning ? vx = vx + MARIO_RUNNING_SPEED : IsRuning = false;
	IsJumping ? vy = -MARIO_JUMP_SPEED_Y : vy += MARIO_GRAVITY * dt;
	if (y <= 0.f) {
		vy += MARIO_GRAVITY * dt / 10000;
		IsJumping = false;
	}
	// simple screen edge collision!!!
	if (vx > 0 && x > 290) x = 290;
		if (vx < 0 && x < 0) x = 0;
	if (this->state == MARIO_STATE_BEND_DOWN)
		this->SetPosition(this->x = this->x, this->y += 10.0f);

}

void CMario::Render()
{
	int ani;
	if (this->state == MARIO_STATE_IDLE)
	{
		if (nx > 0) ani = MARIO_ANI_IDLE_RIGHT;
		else ani = MARIO_ANI_IDLE_LEFT;
	}
	else if (vx > 0)
		ani = MARIO_ANI_WALKING_RIGHT;
	else if (this->state == MARIO_STATE_BEND_DOWN)
		 (nx > 0) ? ani = MARIO_ANI_BEND_DOWN_RIGHT : ani = MARIO_ANI_BEND_DOWN_LEFT;	
	else ani = MARIO_ANI_WALKING_LEFT;

	animations[ani]->Render(x, y);
}

void CMario::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case MARIO_STATE_FAST:
		this->IsRuning = true;
	case MARIO_STATE_WALKING_RIGHT:
		if (IsRuning)
		{
			vx += MARIO_WALKING_SPEED + MARIO_RUNNING_SPEED;
			nx = 1;
			break;
		}
		else
		{
			vx += MARIO_WALKING_SPEED ;
			nx = 1;
			break;
		}
	case MARIO_STATE_WALKING_LEFT: 
		vx = -MARIO_WALKING_SPEED;
		nx = -1;
		break; 
	case MARIO_STATE_JUMP: 
		IsJumping = true;
		//vy = -MARIO_JUMP_SPEED_Y;
		break;

	case MARIO_STATE_DROP:
		IsJumping = false;
		break;
	case MARIO_STATE_IDLE: 
		vx = 0;
		break;
	case MARIO_STATE_BEND_DOWN:
		vx = 0;
		break;
	}
	
}

