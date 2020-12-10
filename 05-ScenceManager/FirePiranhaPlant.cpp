#include "FirePiranhaPlant.h"
#include "Tube.h"
#include "Ground.h"
#include "Game.h"
#include "PlayScence.h"

void FirePiranhaPlant::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + FIRE_PIRANHA_PLANT_WIDTH;
	bottom = y + FIRE_PIRANHA_PLANT_HIGHT;
}

void FirePiranhaPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	y += dy;
	CGameObject::Update(dt);
	//coObjects->push_back(mario);

	CheckPositionMarioToAttack();
	
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
			Bullet->Attack(PosAttack, Attack);
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
	switch (state)
	{
	case FIRE_PIRANHA_PLANT_ANI_LEFT_UP:
		animation_set->at(FIRE_PIRANHA_PLANT_ANI_LEFT_UP)->Render(x, y);
		break;
	case FIRE_PIRANHA_PLANT_ANI_LEFT_DOWN:
		animation_set->at(FIRE_PIRANHA_PLANT_ANI_LEFT_DOWN)->Render(x, y);
		break;
	case FIRE_PIRANHA_PLANT_ANI_RIGHT_UP:
		animation_set->at(FIRE_PIRANHA_PLANT_ANI_RIGHT_UP)->Render(x, y);
		break;
	case FIRE_PIRANHA_PLANT_ANI_RIGHT_DOWN:
		animation_set->at(FIRE_PIRANHA_PLANT_ANI_RIGHT_DOWN)->Render(x, y);
		break;
	}
}

FirePiranhaPlant::FirePiranhaPlant(CMario* mario)
{
	this->mario = mario;
}

void FirePiranhaPlant::SetState(int state)
{
	CGameObject::SetState(state);
	/*switch (state)
	{
	case FIRE_PIRANHA_PLANT_ANI_LEFT_UP:
		break;
	case FIRE_PIRANHA_PLANT_ANI_LEFT_DOWN:
		break;
	case FIRE_PIRANHA_PLANT_ANI_RIGHT_UP:
		break;
	case FIRE_PIRANHA_PLANT_ANI_RIGHT_DOWN:
		break;
	}*/

}

void FirePiranhaPlant::CheckPositionMarioToAttack()
{
	if (mario->x < this->x)	// LEFT
	{
		if (mario->y < this->y)	//TOP
		{
			SetState(FIRE_PIRANHA_PLANT_ANI_LEFT_UP);
			if (this->x - mario->x > 80)
				PosAttack = MARIO_LEFT_TOP_TOP;
			else
				PosAttack = MARIO_LEFT_TOP_BOT;
		}
		else
		{
			SetState(FIRE_PIRANHA_PLANT_ANI_LEFT_DOWN);
			if (this->x - mario->x > 80)
				PosAttack = MARIO_LEFT_BOT_TOP;
			else
				PosAttack = MARIO_LEFT_BOT_BOT;
		}
	}
	else
	{
		if (mario->y < this->y)
		{
			SetState(FIRE_PIRANHA_PLANT_ANI_RIGHT_UP);
			if ( mario->x - this->x > 80)
				PosAttack = MARIO_RIGHT_TOP_TOP;
			else
				PosAttack = MARIO_RIGHT_TOP_BOT;
		}
		else
		{
			SetState(FIRE_PIRANHA_PLANT_ANI_RIGHT_DOWN);
			if (mario->x - this->x > 80)
				PosAttack = MARIO_RIGHT_TOP_TOP;
			else
				PosAttack = MARIO_RIGHT_TOP_BOT;
		}
	}
}
