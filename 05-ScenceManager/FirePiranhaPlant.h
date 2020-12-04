#pragma once
#include "GameObject.h"
#include "BulletPiranhaPlant.h"
#include "Mario.h"

#define FIRE_PIRANHA_PLANT_SPEED_VY		0.001f
#define FIRE_PIRANHA_PLANT_TIME_ATTACK	3000
#define FIRE_PIRANHA_PLANT_RIGHT_UP		0
#define FIRE_PIRANHA_PLANT_RIGHT_DOWN	1
#define FIRE_PIRANHA_PLANT_LEFT_UP		2
#define FIRE_PIRANHA_PLANT_LEFT_DOWN	3
class FirePiranhaPlant : public CGameObject
{
	bool AttackLeft, IsAtack;
	int Attack;
	BulletPiranhaPlant* Bullet = new BulletPiranhaPlant();
	DWORD Attack_start;
	CMario* mario;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
public:
	FirePiranhaPlant(CMario *mario);
	void StartAttack() { Attack = 1; Attack_start = GetTickCount(); }
	void SetState(int state);
};

