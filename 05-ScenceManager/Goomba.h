#pragma once
#include "GameObject.h"
#include "Effect.h"
#include "MicroGoomba.h"
#define GOOMBA_WALKING_SPEED 0.003125f
#define GOOMBA_GRAVITY		0.001f
#define PARAGOOMBA_JUMP_DEFLECT_HEIGHT_SPEED		0.015625f
#define PARAGOOMBA_JUMP_DEFLECT_SPEED				0.01f
#define PARAGOOMBA_VY_WING_BIG						0.05f
#define GOOMBA_BBOX_WIDTH 16
#define GOOMBA_BBOX_HEIGHT 15
#define GOOMBA_BBOX_HEIGHT_DIE 8
#define PARAGOOMBA_WINGED_BBOX_WIDTH		20
#define PARAGOOMBA_WINGED_BBOX_HEIGHT		18
#define PARAGOOMBA_WINGED_BIG_BBOX_HEIGHT	24
#define BROWN_PARAGOOMBA_DISTANCE_X_FLY		100.0f
#define BROWN_PARAGOOMBA_DISTANCE_Y_FLY		10.0f
#define BROWN_PARAGOOMBA_VY_FLYING			0.005f
#define X_FALLING							5.0f 
#define Y_FALLING							15.0f

#define GOOMBA_SCORE						100

#define GOOMBA_STATE_WALKING					100
#define GOOMBA_STATE_DIE						200
#define PARAGOOMBA_WINGED_STATE_JUMP			300
#define PARAGOOMBA_WINGED_STATE_JUMP_HEIGHT		400
#define BROWN_PARAGOOMBA_STATE_FLYING			500

#define GOOMBA_ANI_WALKING						0
#define GOOMBA_ANI_DIE							1
#define	RED_PARAGOOMBA_WINGED_BIG_ANI				2
#define	RED_PARAGOOMBA_WINGED_ANI					3
#define	RED_PARAGOOMBA_WALKING_ANI					4
#define	RED_PARAGOOMBA_DIE_ANI						5
#define	BROWN_PARAGOOMBA_WINGED_BIG_ANI				6
#define	BROWN_PARAGOOMBA_WINGED_ANI					7
#define MICRO_GOOMBA								8

#define GOOMBA_NORMAL				1
#define RED_PARA_GOOMBA				2
#define BROWN_PARA_GOOMBA			3

#define PARAGOOMBA_LEVEL					0
#define PARAGOOMBA_WINGED_LEVEL				1

#define PARAGOOMBA_JUMP_COUNT				3
class CGoomba : public CGameObject
{
	MicroGoomba* MGoomba_1 = NULL;
	MicroGoomba* MGoomba_2 = NULL;
	MicroGoomba* MGoomba_3 = NULL;
	MicroGoomba* MGoomba_4 = NULL;
	int ani;
	ULONGLONG Die_start;
	bool Died, firstUpdate = true;
	int Die, JumpCount;
	float xInit, yInit;
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(ULONGLONG dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	void StartDie() { Die = 1; Die_start = GetTickCount64(); }

public:
	CMario* mario = NULL;
	Effect* effect = NULL;
	int TypeGoomba, LevelParaGoomba = 1;
	CGoomba(int TypeGoomba);
	CGoomba(int TypeGoomba, CMario* mario);
	virtual void SetState(int state);
};