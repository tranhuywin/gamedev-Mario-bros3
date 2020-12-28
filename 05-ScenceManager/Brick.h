#pragma once
#include "GameObject.h"
#include "QuestionBrick.h"
//#include "Items.h"
#define BRICK_BBOX_WIDTH		16.0f
#define BRICK_VX_BREAK			0.0002f
#define BRICK_VY_BREAK			0.0005f
#define BRICK_VY_START_BREAK	0.01f
class Brick : public CGameObject
{
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	float dxBreak, dyBreak;
	float vxBreak, vyBreak;
	float xBreak, yBreak;
	LPGAMEOBJECT items;
public:
	bool IsBreaked, SwitchOff, StoreItemQBrick;
	QuestionBrick* QBick = NULL;
	Brick(LPGAMEOBJECT item);
};

