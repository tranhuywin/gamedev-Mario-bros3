#include "FirePiranhaPlant.h"
#include "Tube.h"
#include "Ground.h"
#include "Game.h"
#include "PlayScence.h"

void FirePiranhaPlant::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + 16;
	bottom = y + 32;
}

void FirePiranhaPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	y += dy;
	CGameObject::Update(dt);
	coObjects->push_back(mario);
	Bullet->Update(dt, coObjects);

	if (GetTickCount() - Attack_start > 5000)
	{
		Attack_start = 0;
		Attack = 0;
		IsAtack = false;
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);

	vector<LPGAMEOBJECT> coEventsResultCo;
	coEventsResultCo.clear();
	CalCollisions(coObjects, coEventsResultCo);

	if (coEvents.size() == 0)
	{
		vy = -FIRE_PIRANHA_PLANT_SPEED_VY * dt;
		if ((coEventsResultCo.size() == 1 || coEventsResultCo.size() == 2) && !IsAtack)
			vy = FIRE_PIRANHA_PLANT_SPEED_VY * dt;
		if (Attack && coEventsResultCo.size() == 1)
		{
			vy = 0;
			Bullet->Attack(this->x, this->y, mario->x, mario->y, false, Attack);
		}		
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<Ground*>(e->obj))
			{
				vy = -FIRE_PIRANHA_PLANT_SPEED_VY * dt;
				IsAtack = true;
				StartAttack();
			}
			else if (vy != FIRE_PIRANHA_PLANT_SPEED_VY * dt || vy != -FIRE_PIRANHA_PLANT_SPEED_VY * dt)
				vy = FIRE_PIRANHA_PLANT_SPEED_VY *dt;
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void FirePiranhaPlant::Render()
{
	Bullet->Render();
	if(this->x < CGame::GetInstance()->GetCamPosX() + CGame::GetInstance()->GetScreenWidth()/2)
		animation_set->at(0)->Render(x, y);
	else
		animation_set->at(3)->Render(x, y);  
}

FirePiranhaPlant::FirePiranhaPlant(CMario* mario)
{
	this->mario = mario;
}

void FirePiranhaPlant::SetState(int state)
{
	CGameObject::SetState(state);

}
