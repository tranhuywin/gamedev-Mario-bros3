#include "BoomerangOfBrother.h"
#include "Utils.h"

void BoomerangOfBrother::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BOOMERANG_BBOX;	
	bottom = y + BOOMERANG_BBOX;
}

void BoomerangOfBrother::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (abs(x - StartAttackX) < BOOMERANG_DISTANCES_ATTACK && IsAttacking && !IsCatching)
	{
		vx = nx * BOOMERANG_SPEED_VX * dt;
		vy = -BOOMERANG_SPEED_VY * dt;
	}
	else
		IsCatching = true;
	
	if (IsCatching) {
		vx = nx * (-1) * BOOMERANG_SPEED_VX * dt;
		vy = BOOMERANG_SPEED_VY_RETURN * dt;
		if (abs(x - StartAttackX)> BOOMERANG_DISTANCE_SLOW)
		{
			vx = nx * (-1) * BOOMERANG_SPEED_SLOW_VX * dt;
		}
		if (this->y > CatchY + 4)
			vy = 0;
		if (nx == 1 && x < CatchX) {
			IsAttacking = false;
			CatchDone = true;
			RenderWeapon = false;
		}
		if(nx == -1  && x > CatchX)
		{
			IsAttacking = false;
			CatchDone = true;
			RenderWeapon = false;
		}
	}
	if (abs(StartAttackX - x)> 0 && !IsAttacking)
		IsAttacking = false;
	if (!RenderWeapon) {
		x = CatchX; y = CatchY;
	}
	CGameObject::Update(dt);
	x += dx;
	y += dy;
}

void BoomerangOfBrother::Render()
{
	int ani = 0;
	if (RenderWeapon)
	{
		if (GetState() == BOOMERANG_ATTACK_LEFT)
			ani = BOOMERANG_ANI_ATTACK_LEFT;
		else if (GetState() == BOOMERANG_ATTACK_RIGHT)
			ani = BOOMERANG_ANI_ATTACK_RIGHT;

		if (holdingLeft)
			ani = BOOMERANG_ANI_HOLDING_LEFT;
		else if (holdingRight)
			ani = BOOMERANG_ANI_HOLDING_RIGHT;

		animation_set->at(ani)->Render(x, y);
	}
		
}

void BoomerangOfBrother::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		case BOOMERANG_ATTACK_LEFT:
		{
			nx = -1;
		}
		break;
		case BOOMERANG_ATTACK_RIGHT:
		{
			nx = 1;
		}
		break;
		case BOOMERANG_HOLDING_LEFT:
			nx = -1;
			break;
		case BOOMERANG_HOLDING_RIGHT:
			nx = 1;
			break;
	}
}

BoomerangOfBrother::BoomerangOfBrother(int Ani)
{
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(Ani);

	this->SetAnimationSet(ani_set);

}

void BoomerangOfBrother::Attack(float x, float y, int attactDirection)
{
	this->StartAttackX = x;
	this->StartAttackY = y;
	this->x = x;
	this->y = y;
	if (attactDirection == -1)
	{
		SetState(BOOMERANG_ATTACK_LEFT);
	}
	else
		SetState(BOOMERANG_ATTACK_RIGHT);
	if (!IsAttacking)
	{
		IsAttacking = true;
		IsCatching = false;
		CatchDone = false;
	}
}

void BoomerangOfBrother::CatchBoomerang(float x, float y)
{
	CatchX = x;
	CatchY = y;
}
