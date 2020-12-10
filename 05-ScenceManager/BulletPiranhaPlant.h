#pragma once
#include "GameObject.h"

#define SPEED_VX_BULLET 0.01f
#define SPEED_VY_BULLET 0.01f
#define OUTSIDE_MAP						500.0f

class BulletPiranhaPlant : public CGameObject
{

public:
	bool AllowAttack, IsAttack;
	BulletPiranhaPlant();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void Attack(int PosAttack, bool AttackIsAtive);
};

