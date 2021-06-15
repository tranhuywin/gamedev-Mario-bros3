#pragma once

#include "GameObject.h"

#define BOOMERANG_BBOX					14

#define BOOMERANG_SPEED_VX				0.005f
#define BOOMERANG_SPEED_SLOW_VX			0.0007f
#define BOOMERANG_SPEED_VY				0.002f
class BoomerangOfBrother : public CGameObject
{
public:
	float StartAttackX, StartAttackY;
	float CatchX, CatchY;
	bool IsAttacking, RenderWeapon;
	bool IsCatching, CatchDone;
	BoomerangOfBrother(int Ani);
	void Attack(float x, float y);
	void CatchBoomerang(float x, float y);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void SetState(int state);
};

