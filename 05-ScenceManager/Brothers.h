#pragma once
#include "GameObject.h"
#include "BoomerangOfBrother.h"

#define BROTHER_BOOMERANG_ANI_WALKING	0
#define BOOMERANG_ANI_ATTACK			1

#define BROTHER_STATE_ATTACK		100
#define BROTHER_STATE_DIE			999
class Brothers : public CGameObject
{
	BoomerangOfBrother* Boomerang1 = NULL;
	BoomerangOfBrother* Boomerang2 = NULL;
	int IDTypeBrother;
	int WaitAttackBoom1, WaitAttackBoom2;
	DWORD WaitAttackBoom1_start, WaitAttackBoom2_start;
public:
	Brothers(int IDType, BoomerangOfBrother* Boomerang1, BoomerangOfBrother* Boomerang2);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
	void StartWaitAttackBoom1() { WaitAttackBoom1 = 1; WaitAttackBoom1_start = GetTickCount64(); }
	void StartWaitAttackBoom2() { WaitAttackBoom2 = 1; WaitAttackBoom2_start = GetTickCount64(); }
};

