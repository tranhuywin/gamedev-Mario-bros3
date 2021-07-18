#pragma once
#include "GameObject.h"

#define BULLET_BBOX						8
#define SPEED_VX_BULLET					0.0025f
#define SPEED_VY_BULLET_TOP				0.0025f
#define SPEED_VY_BULLET_BOT				0.0005f


#define OUTSIDE_MAP						500.0f

class BulletPiranhaPlant : public CGameObject
{

public:
	bool AllowAttack, IsAttack;
	BulletPiranhaPlant();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void Attack(int PosAttack, bool AttackIsAtive, float bulletX, float bulletY);
};

