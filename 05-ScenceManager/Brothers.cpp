#include "Brothers.h"
#include "Ground.h"

void Brothers::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (BBox) {
		left = x;
		top = y;
		right = x + BROTHER_BBOX_WIDTH;
		bottom = y + BROTHER_BBOX_HEIGHT;
	}
}

void Brothers::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (WaitAttackBoom1)
	{
			Boomerang1->Attack(this->x, this->y);
			/*if (!WaitThrow1) {
				Boomerang1->x = this->x - 5;
			}*/
	}
	else {
		Boomerang1->CatchBoomerang(this->x, this->y);
	}
	if(WaitAttackBoom2)
		Boomerang2->Attack(this->x, this->y);
	else {
		Boomerang2->CatchBoomerang(this->x, this->y);
	}
	/*Boomerang1->Update(dt);
	Boomerang2->Update(dt);*/
	if (Boomerang1->CatchDone /*&& Boomerang2->CatchDone*/) {
		StartWaitAttackBoom1();
		
	}
	if (Boomerang2->CatchDone) {
		StartWaitAttackBoom2();
	}
	ani = 0;
	if (WaitAttackBoom1 && GetTickCount64() - WaitAttackBoom1_start > BOOMERANG_1_TIME_WAIT_ATTACK)
	{
		WaitAttackBoom1 = 0;
		WaitAttackBoom1_start = 0;
		if(WaitThrow1 == 0)
		StartWaitThrow1();
		
	}
	if (WaitAttackBoom2 && GetTickCount64() - WaitAttackBoom2_start > BOOMERANG_2_TIME_WAIT_ATTACK)
	{
		WaitAttackBoom2 = 0;
		WaitAttackBoom2_start = 0;
	}
	if (WaitThrow1 && GetTickCount64() - WaitThrow1_start > 1000)
	{
		WaitThrow1 = 0;
		WaitThrow1_start = 0;
	}
	if (WaitThrow2 && GetTickCount64() - WaitThrow2_start > 1000)
	{
		WaitThrow2 = 0;
		WaitThrow2_start = 0;
	}
	Boomerang2->Update(dt);
	Boomerang1->Update(dt);

	CGameObject::Update(dt);
	
	vy += BROTHER_SPEED_VX * dt;
	if (x > BROTHER_X_MAX)
		vx = -BROTHER_SPEED_VX *dt;
	if(x < BROTHER_X_MIN)
		vx = BROTHER_SPEED_VX * dt;
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
	Boomerang2->Render();
	Boomerang1->Render();

	if (!WaitAttackBoom1)
		Boomerang1->RenderWeapon = true;
	else
		Boomerang1->RenderWeapon = false;
	if (!WaitAttackBoom2)
		Boomerang2->RenderWeapon = true;
	else
		Boomerang2->RenderWeapon = false;
	if (WaitAttackBoom1 || WaitAttackBoom2)
		animation_set->at(0)->Render(x, y);
	else
		animation_set->at(1)->Render(x, y);

}

void Brothers::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		case BROTHER_STATE_ATTACK:
		{
			this->vx = BROTHER_SPEED_VX_START;
		}
		break;
		case BROTHER_STATE_DIE:
		{
			vy = -BROTHER_DEFLECT_SPEED;
			BBox = false;
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
