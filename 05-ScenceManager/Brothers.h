#pragma once
#include "GameObject.h"
#include "BoomerangOfBrother.h"
#include "Mario.h"

#define BROTHER_BBOX_HEIGHT				23
#define BROTHER_BBOX_WIDTH				16

#define BROTHER_BOOMERANG_ANI_WALKING_RIGHT		0
#define BOOMERANG_ANI_ATTACK_RIGHT				1
#define BROTHER_BOOMERANG_ANI_WALKING_LEFT		2
#define BOOMERANG_ANI_ATTACK_LEFT				3

#define BROTHER_SPEED_VX			0.001f
#define BROTHER_GRAVITY				0.001f
#define BROTHER_X_DISTANCES			48.0f
#define BROTHER_X_MIN				16.0f
#define BROTHER_DEFLECT_SPEED		0.02f
#define BOOMERANG_POSX				12.0f
#define BOOMERANG_POSY				4.0f

#define BOOMERANG_1_TIME_WAIT_ATTACK	1200
#define BOOMERANG_2_TIME_WAIT_ATTACK	2000
#define BOOMERANG_TIME_WATING_THROW		1000

#define BROTHER_STATE_ATTACK_RIGHT		100
#define BROTHER_STATE_ATTACK_LEFT		200
#define BROTHER_STATE_WALKING_LEFT		300
#define BROTHER_STATE_WALKING_RIGHT		400
#define BROTHER_STATE_DIE				999
class Brothers : public CGameObject
{
	BoomerangOfBrother* Boomerang1 = NULL;
	BoomerangOfBrother* Boomerang2 = NULL;
	bool isSetPossInit;
	float xInit, yInit;
	int IDTypeBrother;
	int ani = 0;
	int WaitAttackBoom1, WaitAttackBoom2, WaitThrow;
	DWORD WaitAttackBoom1_start, WaitAttackBoom2_start, Attack_start;
	DWORD WaitThrow_start;
	CMario* mario;
public:
	Brothers(int IDType, BoomerangOfBrother* Boomerang1, BoomerangOfBrother* Boomerang2, CMario* player);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
	void StartWaitAttackBoom1() { WaitAttackBoom1 = 1; WaitAttackBoom1_start = GetTickCount64(); }
	void StartWaitAttackBoom2() { WaitAttackBoom2 = 1; WaitAttackBoom2_start = GetTickCount64(); }
	void StartWaitThrow() { WaitThrow = 1; WaitThrow_start = GetTickCount64(); }
	//void StartWaitThrow2() { WaitThrow2 = 1; WaitThrow2_start = GetTickCount64(); }
};

