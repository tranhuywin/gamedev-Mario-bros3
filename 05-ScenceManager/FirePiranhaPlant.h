#pragma once
#include "GameObject.h"
#include "BulletPiranhaPlant.h"
#include "Mario.h"

#define FIRE_PIRANHA_PLANT_WIDTH		16
#define FIRE_PIRANHA_PLANT_HIGHT		32
#define FIRE_PIRANHA_PLANT_DISTANCE_ATTACK_MARIO	80

#define FIRE_PIRANHA_PLANT_SPEED_VY		0.001f
#define FIRE_PIRANHA_PLANT_TIME_ATTACK	3000
#define FIRE_PIRANHA_PLANT_ANI_RIGHT_UP		0
#define FIRE_PIRANHA_PLANT_ANI_RIGHT_DOWN	1
#define FIRE_PIRANHA_PLANT_ANI_LEFT_UP		2
#define FIRE_PIRANHA_PLANT_ANI_LEFT_DOWN	3

#define MARIO_LEFT_TOP_TOP		1
#define MARIO_LEFT_TOP_BOT		2
#define MARIO_LEFT_BOT_TOP		3
#define MARIO_LEFT_BOT_BOT		4
#define MARIO_RIGHT_TOP_TOP		5
#define MARIO_RIGHT_TOP_BOT		6
#define MARIO_RIGHT_BOT_TOP		7
#define MARIO_RIGHT_BOT_BOT		8

#define FIRE_PIRANHA_PLANT_TIME_ATTACK				5000
#define FIRE_PIRANHA_PLANT_TIME_WAIT_ATTACK			4500
class FirePiranhaPlant : public CGameObject
{
	bool AttackLeft, IsAtack;
	int Attack, WaitAttack;
	int PosAttack;
	BulletPiranhaPlant* Bullet;
	DWORD Attack_start, WaitAttack_start;
	CMario* mario;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	int ani;
public:
	FirePiranhaPlant(CMario *mario, BulletPiranhaPlant* bullet);
	void StartAttack() { Attack = 1; Attack_start = GetTickCount(); }
	void StartWaitAttack() { WaitAttack = 1; WaitAttack_start = GetTickCount(); }
	void SetState(int state);
	void CheckPositionMarioToAttack();
};

