#pragma once
#include "GameObject.h"
#include "QuestionBrick.h"
//#include "Items.h"
#define BRICK_BBOX_WIDTH		16.0f
#define BRICK_VX_BREAK			0.0002f
#define BRICK_VY_BREAK			0.0005f
#define BRICK_VY_START_BREAK	0.01f

#define BRICK_TIME_OFF			7000
class Brick : public CGameObject
{
	float dxBreak, dyBreak;
	float vxBreak, vyBreak;
	float xBreak, yBreak;
	LPGAMEOBJECT items;
	int Sleep;
	bool count = false;
	DWORD Sleep_start;
	void StartSleep() { Sleep = 1; Sleep_start = GetTickCount64(); }
public:
	int IdItemOfBrick = -1;
	int CountCollBrickMulMoney = 5;
	bool IsBreaked, SwitchOff, StoreItemQBrick;
	QuestionBrick* QBick = NULL;
	Brick(LPGAMEOBJECT item);
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

