#pragma once
#include "GameObject.h"
#define FIRE_BULLET_BBOX_WIDTH			8
#define FIRE_BULLET_SPEED_Y				0.08f
#define FIRE_BULLET_SPEED_X				0.12f
#define FIRE_BULLET_DISTANCE_PLATFORM	20.0f
#define OUTSIDE_MAP						500.0f

#define FIRE_BULLET_ANI_EXPLODE			1
#define FIRE_BULLET_EXPLODE_TIME		400
class FireBullet : public CGameObject
{
	float PosPlatform;
	int Explode;
	DWORD Explode_start;
	float XExplode;
	float YExplode;
public:
	bool OnPlatform;
	bool IsBeingFired;
	FireBullet();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void attack(float x, float y, bool isRight);
	void StartExplode() {	Explode = 1; Explode_start = GetTickCount64();}
};

