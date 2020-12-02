#pragma once
#include "GameObject.h"
#include "FireBullet.h"
#define FIRE_PIRANHA_PLANT_SPEED_VY		0.001f
#define FIRE_PIRANHA_PLANT_TIME_ATTACK	3000

class FirePiranhaPlant : public CGameObject
{
	bool AttackLeft, IsAtack;
	int Attack;
	FireBullet* Bullet = new FireBullet();
	DWORD Attack_start;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
public:
	void StartAttack() { Attack = 1; Attack_start = GetTickCount(); }
};

