#include "Goomba.h"
#include "Utils.h"
CGoomba::CGoomba(int TypeGoomba)
{
	this->TypeGoomba = TypeGoomba;
	SetState(GOOMBA_STATE_WALKING);
}

void CGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	if (TypeGoomba == GOOMBA_NORMAL)
	{
		right = x + GOOMBA_BBOX_WIDTH;

		if (state == GOOMBA_STATE_DIE)
			bottom = y + GOOMBA_BBOX_HEIGHT_DIE;
		else
			bottom = y + GOOMBA_BBOX_HEIGHT;
	}
	else if (TypeGoomba == PARA_GOOMBA)
	{
		if (LevelParaGoomba == PARAGOOMBA_LEVEL)
		{
			right = x + GOOMBA_BBOX_WIDTH;
			bottom = y + GOOMBA_BBOX_WIDTH;
		}
		else if (LevelParaGoomba == PARAGOOMBA_WINGED_LEVEL)
		{
			right = x + PARAGOOMBA_WINGED_BBOX_WIDTH;
			bottom = y + PARAGOOMBA_WINGED_BBOX_HEIGHT;
		}
		else {
			right = x + GOOMBA_BBOX_WIDTH;
			bottom = y + GOOMBA_BBOX_HEIGHT_DIE;
		}
	}
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	vy += GOOMBA_GRAVITY * dt;
	CGameObject::Update(dt);
	if(state == GOOMBA_STATE_DIE)
		if (Die && GetTickCount() - Die_start > animation_set->at(GOOMBA_ANI_DIE)->GettotalFrameTime())
		{
			Die_start = 0;
			Die = 0;
			Died = true;
		}
	
	if (TypeGoomba == PARA_GOOMBA) {
		if (LevelParaGoomba == PARAGOOMBA_WINGED_LEVEL)
			if (vy > -0.05f && vy < 0.05f)
			{
				ani = PARAGOOMBA_WINGED_BIG_ANI;
			}
			else ani = PARAGOOMBA_WINGED_ANI;

		if (JumpCount == 3)
		{
			SetState(PARAGOOMBA_WINGED_STATE_JUMP_HEIGHT);
			JumpCount = 0;
		}
	}
	if (vx < 0 && x < 16) {
		x = 16; vx = -vx;
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	if (!Died)	//neu khac chet == song
		CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;
		if (nx != 0)
			vx = -vx;
		if (ny != 0)
		{
			vy = 0;
			if (TypeGoomba == PARA_GOOMBA)
				if (LevelParaGoomba == PARAGOOMBA_WINGED_LEVEL) {
					vy = -PARAGOOMBA_JUMP_DEFLECT_SPEED;
					JumpCount++;
				}
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CGoomba::Render()
{
	int YRender = y;
	if (TypeGoomba == GOOMBA_NORMAL)
	{
		ani = GOOMBA_ANI_WALKING;
		if (state == GOOMBA_STATE_DIE) {
			ani = GOOMBA_ANI_DIE;
		}
	}
	else if (TypeGoomba == PARA_GOOMBA)
	{
		if (LevelParaGoomba == PARAGOOMBA_WINGED_LEVEL)
		{
			if (ani == PARAGOOMBA_WINGED_BIG_ANI)
				YRender -= PARAGOOMBA_WINGED_BIG_BBOX_HEIGHT - PARAGOOMBA_WINGED_BBOX_HEIGHT;
		}
		else {
			ani = PARAGOOMBA_WALKING_ANI;
			if (state == GOOMBA_STATE_DIE)
				ani = PARAGOOMBA_DIE_ANI;
		}
	}
	animation_set->at(ani)->Render(x, YRender);
	RenderBoundingBox();
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		case GOOMBA_STATE_DIE:
			StartDie();
			y += GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE;
			vx = 0;
			break;
		case GOOMBA_STATE_WALKING: 
			vx = -GOOMBA_WALKING_SPEED;
			Died = false;
			break;
		case PARAGOOMBA_WINGED_STATE_JUMP:
			vx = GOOMBA_WALKING_SPEED;
			LevelParaGoomba = PARAGOOMBA_WINGED_LEVEL;
			break;
		case PARAGOOMBA_WINGED_STATE_JUMP_HEIGHT:
			vy = -PARAGOOMBA_JUMP_DEFLECT_HEIGHT_SPEED;
			break;
	}
}
