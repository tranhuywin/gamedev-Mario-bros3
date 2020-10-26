#pragma once
#include "GameObject.h"

#define GOOMBA_BBOX_WIDTH				24
#define GOOMBA_BBOX_HEIGHT				29
#define BOOMERANG_BBOX_WIDTH			16
#define BOOMERANG_BBOX_HEIGHT			16
#define GOOMBA_BBOX_HEIGHT_DIE			9

#define VELOCITY_BOOMERANG				0.04f
#define DISTANCE_BOOMERANG_GOOMBA		100.0f

#define GOOMBA_STATE_DIE				200
#define GOOMBA_STAGE_ATTACK				705
#define BOOMERANG_STAGE_ATTACK			706
#define GOOMBA_STAGE_PREPARE_ATTACK		707
#define BOOMERANG_STAGE_PREPARE_ATTACK	708

#define GOOMBA_ANI_PREPARE_ATTACK		0
#define BUMERANG_ANI_PREPARE_ATTACK		1
#define GOOMBA_ANI_ATTACK				2
#define BUMERANG_ANI_ATTACK				3

class GoombaBoomerang : public CGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
public:
	virtual void SetState(int state);
private:
	int BoomerangX;
	int BoomerangY;
	float Velocity_Boomerang = VELOCITY_BOOMERANG;
};

