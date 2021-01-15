#include "BoomerangOfBrother.h"

void BoomerangOfBrother::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}

void BoomerangOfBrother::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (x - StartAttackX < 100 && IsAttacking && !IsCatching)
	{
		vx = BOOMERANG_SPEED_VX * dt;
		vy = -BOOMERANG_SPEED_VY * dt;
	}
	else
		IsCatching = true;
	
	if (IsCatching) {
		vx = -BOOMERANG_SPEED_VX * dt;
		vy = BOOMERANG_SPEED_VY * dt;
		if (x - StartAttackX > 95)
		{
			vx = -BOOMERANG_SPEED_SLOW_VX * dt;
		}
		if (this->y > CatchY)
			vy = 0;
		if (x < CatchX)
		{
			IsAttacking = false;
			CatchDone = true;
		}
	}
	if (StartAttackX > x && !IsAttacking)
		IsAttacking = false;
	CGameObject::Update(dt);
	x += dx;
	y += dy;
}

void BoomerangOfBrother::Render()
{
	if(RenderWeapon)
		animation_set->at(0)->Render(x, y);
}

void BoomerangOfBrother::SetState(int state)
{
}

BoomerangOfBrother::BoomerangOfBrother(int Ani)
{
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(Ani);

	this->SetAnimationSet(ani_set);

}

void BoomerangOfBrother::Attack(float x, float y)
{
	this->StartAttackX = x;
	this->StartAttackY = y;
	this->x = x;
	this->y = y;
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
