#pragma once
#include "GameObject.h"

#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16

#define BRICK_STATE_QUESTION_ON			0
#define BRICK_STATE_QUESTION_OFF		1


class CBrick : public CGameObject
{
	bool OntoOff;
	bool PreviousState;
public:
	virtual void Render();
	CBrick();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	void SetState(int state);
};