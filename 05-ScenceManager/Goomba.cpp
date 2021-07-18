#include "Goomba.h"
#include "Utils.h"
#include "Items.h"
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
		bottom = y + GOOMBA_BBOX_HEIGHT;
	}
	else if (TypeGoomba == PARA_GOOMBA)
	{
		if (LevelParaGoomba == PARAGOOMBA_LEVEL)
		{
			right = x + GOOMBA_BBOX_WIDTH;
			bottom = y + GOOMBA_BBOX_HEIGHT;
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

	if (state == GOOMBA_STATE_DIE)
		bottom = y + GOOMBA_BBOX_HEIGHT_DIE;
}

void CGoomba::Update(ULONGLONG dt, vector<LPGAMEOBJECT> *coObjects)
{
	if (effect != NULL) {
		effect->Update(dt);
	}

	if (state != GOOMBA_STATE_DIE)
		vy += GOOMBA_GRAVITY * dt;
	else
	{
		vx = 0; vy = 0;
	}
	CGameObject::Update(dt);
	if(state == GOOMBA_STATE_DIE)
		//if(ani == GOOMBA_ANI_DIE || ani == PARAGOOMBA_DIE_ANI)
		if (Die && GetTickCount() - Die_start > animation_set->at(ani)->GettotalFrameTime())
		{
			Die_start = 0;
			Die = 0;
			Died = true;
		}
	
	if (TypeGoomba == PARA_GOOMBA) {
		if (LevelParaGoomba == PARAGOOMBA_WINGED_LEVEL)
			if (vy > -PARAGOOMBA_VY_WING_BIG && vy < PARAGOOMBA_VY_WING_BIG)
			{
				ani = PARAGOOMBA_WINGED_BIG_ANI;
			}
			else ani = PARAGOOMBA_WINGED_ANI;

		if (JumpCount == PARAGOOMBA_JUMP_COUNT)
		{
			SetState(PARAGOOMBA_WINGED_STATE_JUMP_HEIGHT);
			JumpCount = 0;
		}
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	if (!Died)	//neu khac chet == song
	//if(state != GOOMBA_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);
	else {
		x = 0; y = 0; vx = 0; vy = 0;
	}

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
					vy = -PARAGOOMBA_JUMP_DEFLECT_SPEED * dt;
					JumpCount++;
				}
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CGoomba::Render()
{
	float YRender = y;
	if (effect != NULL) {
		effect->Render();
	}
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
	if (!Died)
		animation_set->at(ani)->Render(x, YRender);
	//RenderBoundingBox();
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		case GOOMBA_STATE_DIE:
			StartDie();
			vx = 0;
			y += GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE;
			if (effect == NULL)
			{
				effect = new Effect(this->x, this->y - GOOMBA_BBOX_HEIGHT, 80100);
				int CurrentScore = CGame::GetInstance()->GetScore();
				CGame::GetInstance()->SetScore(CurrentScore + 100);
			}
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
