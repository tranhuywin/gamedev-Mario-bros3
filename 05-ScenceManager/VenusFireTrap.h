#pragma once
#include "GameObject.h"
#include "Mario.h"
#include "BulletPiranhaPlant.h"

#define VENUS_FIRE_TRAP_WIDTH			15
#define RED_VENUS_FIRE_TRAP_HIGHT		32
#define GREEN_VENUS_FIRE_TRAP_HIGHT		24
#define VENUS_FIRE_TRAP_DISTANCE_ATTACK_MARIO	80

#define VENUS_FIRE_TRAP_SPEED_VY		0.001f
#define VENUS_FIRE_TRAP_TIME_ATTACK	3000
#define VENUS_FIRE_TRAP_ANI_RIGHT_UP			0
#define VENUS_FIRE_TRAP_ANI_RIGHT_DOWN			1
#define VENUS_FIRE_TRAP_ANI_LEFT_UP				2
#define VENUS_FIRE_TRAP_ANI_LEFT_DOWN			3
#define GREEN_VENUS_FIRE_TRAP_ANI_RIGHT_UP		4
#define GREEN_VENUS_FIRE_TRAP_ANI_RIGHT_DOWN	5
#define GREEN_VENUS_FIRE_TRAP_ANI_LEFT_UP		6
#define GREENVENUS_FIRE_TRAP_ANI_LEFT_DOWN		7

#define MARIO_LEFT_TOP_TOP		1
#define MARIO_LEFT_TOP_BOT		2
#define MARIO_LEFT_BOT_TOP		3
#define MARIO_LEFT_BOT_BOT		4
#define MARIO_RIGHT_TOP_TOP		5
#define MARIO_RIGHT_TOP_BOT		6
#define MARIO_RIGHT_BOT_TOP		7
#define MARIO_RIGHT_BOT_BOT		8
#define VENUS_FIRE_TRAP_TIME_ATTACK					5000
#define VENUS_FIRE_TRAP_TIME_WAIT_ATTACK			4500

#define TYPE_VENUS_FIRE_TRAP_RED			0
#define TYPE_VENUS_FIRE_TRAP_GREEN			1
class VenusFireTrap : public CGameObject
{
	bool AttackLeft, IsAtack;
	int Attack, WaitAttack;
	int PosAttack, TypeVenusFireTrap;
	BulletPiranhaPlant* Bullet;
	DWORD Attack_start, WaitAttack_start;
	CMario* mario;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	int ani;
public:
	VenusFireTrap(CMario *mario, BulletPiranhaPlant* bullet, int TypeVenusFireTrap);
	void StartAttack() { Attack = 1; Attack_start = GetTickCount(); }
	void StartWaitAttack() { WaitAttack = 1; WaitAttack_start = GetTickCount(); }
	void SetState(int state);
	void CheckPositionMarioToAttack();
};

