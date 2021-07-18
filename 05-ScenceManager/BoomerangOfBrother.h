#pragma once

#include "GameObject.h"

#define BOOMERANG_BBOX					14

#define BOOMERANG_SPEED_VX				0.005f
#define BOOMERANG_SPEED_SLOW_VX			0.0007f
#define BOOMERANG_SPEED_VY				0.00225f
#define BOOMERANG_SPEED_VY_RETURN		0.005f

#define BOOMERANG_ATTACK_LEFT			100
#define BOOMERANG_ATTACK_RIGHT			200
#define BOOMERANG_HOLDING_LEFT			300
#define BOOMERANG_HOLDING_RIGHT			400

#define BOOMERANG_DISTANCES_ATTACK		100
#define BOOMERANG_DISTANCE_SLOW			95

#define BOOMERANG_ANI_HOLDING_LEFT			0
#define BOOMERANG_ANI_ATTACK_RIGHT			1
#define BOOMERANG_ANI_HOLDING_RIGHT			2
#define BOOMERANG_ANI_ATTACK_LEFT			3
class BoomerangOfBrother : public CGameObject
{
public:
	float StartAttackX = -1.0f, StartAttackY = -1.0f;
	float CatchX = -1.0f, CatchY = -1.0f;
	bool IsAttacking = false , RenderWeapon = false;
	bool IsCatching = false, CatchDone = true, holdingLeft = false, holdingRight = false;
	BoomerangOfBrother(int Ani);
	void Attack(float x, float y, int marioPos);
	void CatchBoomerang(float x, float y);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void SetState(int state);
};

