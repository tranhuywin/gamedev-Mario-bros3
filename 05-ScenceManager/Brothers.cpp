#include "Brothers.h"
#include "Ground.h"

void Brothers::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + 16;
	bottom = y + 23;
}

void Brothers::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (WaitAttackBoom1)
		Boomerang1->Attack(this->x, this->y);
	else {
		Boomerang1->CatchBoomerang(this->x, this->y);
	}
	if(WaitAttackBoom2)
		Boomerang2->Attack(this->x + 10.0f, this->y);
	else {
		Boomerang2->CatchBoomerang(this->x, this->y);
	}
	/*Boomerang1->Update(dt);
	Boomerang2->Update(dt);*/
	if (Boomerang1->CatchDone && Boomerang2->CatchDone) {
		StartWaitAttackBoom1();
		StartWaitAttackBoom2();
	}
	/*if (Boomerang2->CatchDone) {
		StartWaitAttackBoom2();
	}*/
	if (WaitAttackBoom1 && GetTickCount64() - WaitAttackBoom1_start > 1500)
	{
		WaitAttackBoom1 = 0;
		WaitAttackBoom1_start = 0;
	}
	if (WaitAttackBoom2 && GetTickCount64() - WaitAttackBoom2_start > 1500)
	{
		WaitAttackBoom2 = 0;
		WaitAttackBoom2_start = 0;
	}
	CGameObject::Update(dt);
	
	vy += 0.001f * dt;
	if (x > 80)
		vx = -0.001f *dt;
	if(x < 15.0)
		vx = 0.001f * dt;
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	if (state != BROTHER_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny = 0;
		float rdx = 0;
		float rdy = 0;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;
		if (nx != 0)
			vx = -vx;
		if (ny != 0)
			vy = 0;
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Brothers::Render()
{
	if (!WaitAttackBoom1)
		Boomerang1->RenderWeapon = true;
	else
		Boomerang1->RenderWeapon = false;
	if (!WaitAttackBoom2)
		Boomerang2->RenderWeapon = true;
	else
		Boomerang2->RenderWeapon = false;

	animation_set->at(0)->Render(x, y);
}

void Brothers::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		case BROTHER_STATE_ATTACK:
		{
			this->vx = 0.015f;
		}
		break;
	}
}

Brothers::Brothers(int IDType, BoomerangOfBrother* Boomerang1, BoomerangOfBrother* Boomerang2)
{
	this->IDTypeBrother = IDType;
	this->Boomerang1 = Boomerang1;
	this->Boomerang2 = Boomerang2;
	SetState(BROTHER_STATE_ATTACK);

}
