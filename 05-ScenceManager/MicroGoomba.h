#pragma once
#include "GameObject.h"
#include "Mario.h"

#define MICRO_GOOMBA_BBOX				8

#define MICRO_GOOMBA_VY_FALLING			0.0015f
#define MICRO_GOOMBA_VX_FALLING			0.003f
#define MICRO_GOOMBA_DISTANCE_X_FALLING	10.0f
#define MICRO_GOOMBA_VX_STICK			0.003f
#define MICRO_GOOMBA_VY_STICK			0.001f
#define MICRO_GOOMBA_VY_UNSTICK			0.0005f
#define MICRO_GOOMBA_VX_UNSTICK			0.0002f
#define MICRO_GOOMBA_DEFLECT_VY_UNSTICK			0.005f
#define MICRO_GOOMBA_DEFLECT_VX_UNSTICK			0.002f

#define MICRO_GOOMBA_STATE_FALLING			100
#define MICRO_GOOMBA_STATE_WAIT_FALLING		200
#define MICRO_GOOMBA_STATE_STICK_TO_MARIO	300
#define MICRO_GOOMBA_STATE_UNSTICK			400
class MicroGoomba : public CGameObject
{
public:
	float xStartFalling, yStartFalling;
	bool isRender = true;
	CMario* mario;
	void setPositionFalling(float xStartFalling, float yStartFalling);
	MicroGoomba();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
};

