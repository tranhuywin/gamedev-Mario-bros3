#include "VenusFireTrap.h"
#include "Tube.h"
#include "Ground.h"
#include "Game.h"
#include "PlayScence.h"
#include "BulletPiranhaPlant.h"

void VenusFireTrap::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + VENUS_FIRE_TRAP_WIDTH;
	if (TypeVenusFireTrap == TYPE_VENUS_FIRE_TRAP_RED)
	{
		bottom = y + RED_VENUS_FIRE_TRAP_HIGHT;
	}
	else {
		bottom = y + GREEN_VENUS_FIRE_TRAP_HIGHT;
	}
}

void VenusFireTrap::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	y += dy;
	CGameObject::Update(dt);

	CheckPositionMarioToAttack();

	Bullet->Update(dt, coObjects);
	int TimeAttack = 1950, TimeWaitAttack = 0;
	if (TypeVenusFireTrap != TYPE_VENUS_FIRE_TRAP_GREEN_BITE)
	{
		TimeAttack = VENUS_FIRE_TRAP_TIME_ATTACK;
		TimeWaitAttack = VENUS_FIRE_TRAP_TIME_WAIT_ATTACK;
	}
	if (GetTickCount() - Attack_start > TimeAttack)
	{
		Attack_start = 0;
		Attack = 0;
		IsAtack = false;
	}
	if (GetTickCount() - WaitAttack_start > TimeWaitAttack)
	{
		WaitAttack_start = 0;
		WaitAttack = 0;
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
		vy = -VENUS_FIRE_TRAP_SPEED_VY * dt;
		if ((coEventsResultCo.size() == 1 || coEventsResultCo.size() == 2) && !IsAtack)
			vy = VENUS_FIRE_TRAP_SPEED_VY * dt;
		if (Attack && coEventsResultCo.size() == 1)
		{
			vy = 0;
			if (!WaitAttack)
			{
				float Bullet_X, Bullet_Y;
				if (PosAttack == MARIO_LEFT_TOP_TOP || PosAttack == MARIO_LEFT_TOP_BOT || PosAttack == MARIO_LEFT_BOT_TOP || PosAttack == MARIO_LEFT_BOT_BOT)
					Bullet_X = this->x - BULLET_BBOX;
				else
					Bullet_X = this->x + VENUS_FIRE_TRAP_WIDTH;
				if (PosAttack == MARIO_LEFT_TOP_TOP || PosAttack == MARIO_LEFT_TOP_BOT || PosAttack == MARIO_RIGHT_TOP_TOP || PosAttack == MARIO_RIGHT_TOP_BOT)
					Bullet_Y = this->y + BULLET_BBOX / 2;
				else
					Bullet_Y = this->y + BULLET_BBOX  + BULLET_BBOX / 2;
				if(TypeVenusFireTrap != TYPE_VENUS_FIRE_TRAP_GREEN_BITE)
					Bullet->Attack(PosAttack, Attack, Bullet_X, Bullet_Y);
			}
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
				vy = -VENUS_FIRE_TRAP_SPEED_VY * dt;
				IsAtack = true;
				StartAttack();
				StartWaitAttack();

			}
			else if (vy != VENUS_FIRE_TRAP_SPEED_VY * dt || vy != -VENUS_FIRE_TRAP_SPEED_VY * dt)
				vy = VENUS_FIRE_TRAP_SPEED_VY *dt;
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void VenusFireTrap::Render()
{
	Bullet->Render();
	if (TypeVenusFireTrap == TYPE_VENUS_FIRE_TRAP_RED) {
		switch (state)
		{
		case VENUS_FIRE_TRAP_ANI_LEFT_UP:
			ani = VENUS_FIRE_TRAP_ANI_LEFT_UP;
			break;
		case VENUS_FIRE_TRAP_ANI_LEFT_DOWN:
			ani = VENUS_FIRE_TRAP_ANI_LEFT_DOWN;
			break;
		case VENUS_FIRE_TRAP_ANI_RIGHT_UP:
			ani = VENUS_FIRE_TRAP_ANI_RIGHT_UP;
			break;
		case VENUS_FIRE_TRAP_ANI_RIGHT_DOWN:
			ani = VENUS_FIRE_TRAP_ANI_RIGHT_DOWN;
			break;
		}
	}
	else if(TypeVenusFireTrap == TYPE_VENUS_FIRE_TRAP_GREEN) {
		switch (state)
		{
		case VENUS_FIRE_TRAP_ANI_LEFT_UP:
			ani = GREEN_VENUS_FIRE_TRAP_ANI_LEFT_UP;
			break;
		case VENUS_FIRE_TRAP_ANI_LEFT_DOWN:
			ani = GREENVENUS_FIRE_TRAP_ANI_LEFT_DOWN;
			break;
		case VENUS_FIRE_TRAP_ANI_RIGHT_UP:
			ani = GREEN_VENUS_FIRE_TRAP_ANI_RIGHT_UP;
			break;
		case VENUS_FIRE_TRAP_ANI_RIGHT_DOWN:
			ani = GREEN_VENUS_FIRE_TRAP_ANI_RIGHT_DOWN;
			break;
		}
	}
	else {
		ani = GREENVENUS_FIRE_TRAP_ANI_BITE;
	}
	animation_set->at(ani)->Render(x, y);
}

VenusFireTrap::VenusFireTrap(CMario* mario, BulletPiranhaPlant* bullet, int TypeVenusFireTrap)
{

	this->mario = mario;
	this->Bullet = bullet;
	this->TypeVenusFireTrap = TypeVenusFireTrap;
}

void VenusFireTrap::SetState(int state)
{
	CGameObject::SetState(state);
	/*switch (state)
	{
	case VENUS_FIRE_TRAP_ANI_LEFT_UP:
		break;
	case VENUS_FIRE_TRAP_ANI_LEFT_DOWN:
		break;
	case VENUS_FIRE_TRAP_ANI_RIGHT_UP:
		break;
	case VENUS_FIRE_TRAP_ANI_RIGHT_DOWN:
		break;
	}*/

}

void VenusFireTrap::CheckPositionMarioToAttack()
{
	if (mario->x < this->x)	// LEFT
	{
		if (mario->y < this->y)	//TOP
		{
			if(vy == 0)
				SetState(VENUS_FIRE_TRAP_ANI_LEFT_UP);
			if (this->x - mario->x > VENUS_FIRE_TRAP_DISTANCE_ATTACK_MARIO)
				PosAttack = MARIO_LEFT_TOP_TOP;
			else
				PosAttack = MARIO_LEFT_TOP_BOT;
		}
		else
		{
			if (vy == 0)
				SetState(VENUS_FIRE_TRAP_ANI_LEFT_DOWN);
			if (this->x - mario->x > VENUS_FIRE_TRAP_DISTANCE_ATTACK_MARIO)
				PosAttack = MARIO_LEFT_BOT_TOP;
			else
				PosAttack = MARIO_LEFT_BOT_BOT;
		}
	}
	else
	{
		if (mario->y < this->y)
		{
			if (vy == 0)
				SetState(VENUS_FIRE_TRAP_ANI_RIGHT_UP);
			if ( mario->x - this->x > VENUS_FIRE_TRAP_DISTANCE_ATTACK_MARIO)
				PosAttack = MARIO_RIGHT_TOP_TOP;
			else
				PosAttack = MARIO_RIGHT_TOP_BOT;
		}
		else
		{
			if (vy == 0)
				SetState(VENUS_FIRE_TRAP_ANI_RIGHT_DOWN);
			if (mario->x - this->x > VENUS_FIRE_TRAP_DISTANCE_ATTACK_MARIO)
				PosAttack = MARIO_RIGHT_TOP_TOP;
			else
				PosAttack = MARIO_RIGHT_TOP_BOT;
		}
	}
}
