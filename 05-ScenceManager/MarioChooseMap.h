#pragma once
#include "GameObject.h"
#define MARIO_STATE_IDLE		10
#define MARIO_STATE_UP			20
#define MARIO_STATE_DOWN		30
#define MARIO_STATE_LEFT		40
#define MARIO_STATE_RIGHT		50

#define MARIO_SPEED				0.1f
class MarioChooseMap :public CGameObject
{
public:
	MarioChooseMap(float x, float y);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

