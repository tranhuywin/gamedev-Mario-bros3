#include "GoombaBoomerang.h"

void GoombaBoomerang::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + GOOMBA_BBOX_WIDTH;

	if (state == GOOMBA_STATE_DIE)
		bottom = y + GOOMBA_BBOX_HEIGHT_DIE;
	else
		bottom = y + GOOMBA_BBOX_HEIGHT;
}

void GoombaBoomerang::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	// 

	x += dx;
	y += dy;
	if (x - BoomerangX > DISTANCE_BOOMERANG_GOOMBA)
	{
		Velocity_Boomerang = -Velocity_Boomerang;
	}
	else
	{
		Velocity_Boomerang = Velocity_Boomerang;
	}
	BoomerangX -= Velocity_Boomerang * dt;
	
}

void GoombaBoomerang::Render()
{
	int ani = GOOMBA_ANI_ATTACK;
	int aniBum = BUMERANG_ANI_ATTACK;
	if (state == GOOMBA_STAGE_ATTACK) {
		ani = GOOMBA_ANI_PREPARE_ATTACK;
	}
	animations[aniBum]->Render(BoomerangX, BoomerangY);
	animations[ani]->Render(x, y);
	
	RenderBoundingBox();
}

void GoombaBoomerang::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GOOMBA_STATE_DIE:
		y += GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE + 1;
		vx = 0;
		vy = 0;
		break;
	case GOOMBA_STAGE_PREPARE_ATTACK:
		vx = 0;
		vy = 0;
		BoomerangX = x;
		BoomerangY = y;
		break;
	}
}