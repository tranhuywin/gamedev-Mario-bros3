#pragma once
#include "GameObject.h"

#define BULLET_BBOX						8
#define SPEED_VX_BULLET					0.0025
#define SPEED_VY_BULLET_TOP				0.0025
#define SPEED_VY_BULLET_BOT				0.0005

#define BULLET_XRIGHT					376.0f
#define BULLET_XLEFT					352.0f

#define BULLET_YTOP						336.0f
#define BULLET_YBOT						344.0f

#define OUTSIDE_MAP						500.0f

class BulletPiranhaPlant : public CGameObject
{

public:
	bool AllowAttack, IsAttack;
	BulletPiranhaPlant();
	BulletPiranhaPlant(int ani);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void Attack(int PosAttack, bool AttackIsAtive);
};

