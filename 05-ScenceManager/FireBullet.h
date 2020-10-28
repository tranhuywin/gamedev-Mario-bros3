#pragma once
#include "GameObject.h"
#define FIRE_BULLET_BBOX_WIDTH			8
#define FIRE_BULLET_SPEED_Y				0.1f
#define FIRE_BULLET_SPEED_X				0.1f
#define FIRE_BULLET_DISTANCE_PLATFORM	30.0f
#define OUTSIDE_MAP						500.0f
class FireBullet : public CGameObject
{
	float PosPlatform;
	
public:
	bool OnPlatform;
	bool IsBeingFired;
	FireBullet();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void attack(float x, float y, bool isRight);
};

