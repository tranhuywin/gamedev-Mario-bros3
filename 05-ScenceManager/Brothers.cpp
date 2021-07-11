#include "Brothers.h"
#include "Ground.h"
#include "Utils.h"

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
	this->dt = dt;
	if (x > mario->x)
	{
		SetState(BROTHER_STATE_ATTACK_LEFT);
	}
	else {
		SetState(BROTHER_STATE_ATTACK_RIGHT);
	}

	if (!isSetPossInit)
	{
		xInit = this->x; yInit = this->y;
		vx = BROTHER_SPEED_VX * dt;
		isSetPossInit = true;
	}
	Boomerang1->holdingLeft = false;
	Boomerang1->holdingRight = false;
	Boomerang2->holdingLeft = false;
	Boomerang2->holdingRight = false;
	if (WaitAttackBoom1)
	{
		if (GetState() == BROTHER_STATE_ATTACK_LEFT)
		{
			Boomerang1->Attack(this->x + BOOMERANG_POSX, this->y - BOOMERANG_POSY, -1);
			Boomerang1->holdingLeft = true;
			
		}
		else
		{
			Boomerang1->Attack(this->x - BOOMERANG_POSX, this->y - BOOMERANG_POSY, 1);
			Boomerang1->holdingRight = true;
		}
	}
	else {
		Boomerang1->CatchBoomerang(this->x, this->y);
	}
	if (WaitAttackBoom2)
	{
		if (GetState() == BROTHER_STATE_ATTACK_LEFT)
		{
			Boomerang2->Attack(this->x + BOOMERANG_POSX, this->y - BOOMERANG_POSY, -1);
			Boomerang2->holdingLeft = true;
		}
		else
		{
			Boomerang2->Attack(this->x - BOOMERANG_POSX, this->y - BOOMERANG_POSY, 1);
			Boomerang2->holdingRight = true;
		}
	}
	else {
		Boomerang2->CatchBoomerang(this->x, this->y);
	}
	if(Boomerang1->CatchDone && Boomerang2->CatchDone)
	{
		StartWaitAttackBoom1();
		StartWaitAttackBoom2();
		StartWaitThrow(); // an vu khi
	}
	if (WaitAttackBoom1 && GetTickCount64() - WaitAttackBoom1_start > BOOMERANG_1_TIME_WAIT_ATTACK)
	{
		WaitAttackBoom1 = 0;
		WaitAttackBoom1_start = 0;		
	}
	if (WaitAttackBoom2 && GetTickCount64() - WaitAttackBoom2_start > BOOMERANG_2_TIME_WAIT_ATTACK)
	{
		WaitAttackBoom2 = 0;
		WaitAttackBoom2_start = 0;
	}
	if (WaitThrow && GetTickCount64() - WaitThrow_start > BOOMERANG_TIME_WATING_THROW)
	{
		WaitThrow = 0;
		WaitThrow_start = 0;
	}

	Boomerang2->Update(dt);
	Boomerang1->Update(dt);
	//DebugOut(L"vx%f\n", this->vx);
	CGameObject::Update(dt);
	//di chuyen cua brother
	vy += BROTHER_GRAVITY * dt;
	if (x >= xInit + BROTHER_X_DISTANCES)
		vx = -BROTHER_SPEED_VX *dt;
	else if(x < xInit - BROTHER_X_DISTANCES)
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
		//for (UINT i = 0; i < coEventsResult.size(); i++)
		//{
		//	//LPCOLLISIONEVENT e = coEventsResult[i];
		//}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	
}

void Brothers::Render()
{
	Boomerang1->Render();
	Boomerang2->Render();
	if (!WaitThrow)
	{
		Boomerang1->RenderWeapon = true;
		Boomerang2->RenderWeapon = true;
	}
	else
	{
		Boomerang1->RenderWeapon = false;
		Boomerang2->RenderWeapon = false;
	}
	
	if (GetState() == BROTHER_STATE_ATTACK_RIGHT)
	{
		if (WaitAttackBoom1 || WaitAttackBoom2)
			animation_set->at(BROTHER_BOOMERANG_ANI_WALKING_RIGHT)->Render(x, y);
		else
			animation_set->at(BOOMERANG_ANI_ATTACK_RIGHT)->Render(x, y); 
	}
	else if (GetState() == BROTHER_STATE_ATTACK_LEFT)
	{
		if (!WaitAttackBoom2)
			animation_set->at(BOOMERANG_ANI_ATTACK_LEFT)->Render(x, y);
		else
			animation_set->at(BROTHER_BOOMERANG_ANI_WALKING_LEFT)->Render(x, y);
	}
}

void Brothers::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		case BROTHER_STATE_ATTACK_RIGHT:
		{
			nx = 1;
		}
		break;
		case BROTHER_STATE_ATTACK_LEFT:
		{
			nx = -1;
		}
		break;
		case BROTHER_STATE_DIE:
		{
			vy = -BROTHER_DEFLECT_SPEED * dt;
			BBox = false;
		}
		break;
	}
}

Brothers::Brothers(int IDType, BoomerangOfBrother* Boomerang1, BoomerangOfBrother* Boomerang2, CMario* player)
{
	this->IDTypeBrother = IDType;
	this->Boomerang1 = Boomerang1;
	this->Boomerang2 = Boomerang2;
	this->mario = player;
	SetState(BROTHER_STATE_ATTACK_RIGHT);

}
