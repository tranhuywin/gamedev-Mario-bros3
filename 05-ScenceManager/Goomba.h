#pragma once
#include "GameObject.h"
#include "Effect.h"
#define GOOMBA_WALKING_SPEED 0.05f
#define GOOMBA_GRAVITY		0.001
#define PARAGOOMBA_JUMP_DEFLECT_HEIGHT_SPEED		0.25f
#define PARAGOOMBA_JUMP_DEFLECT_SPEED				0.01f
#define PARAGOOMBA_VY_WING_BIG						0.05f
#define GOOMBA_BBOX_WIDTH 16
#define GOOMBA_BBOX_HEIGHT 15
#define GOOMBA_BBOX_HEIGHT_DIE 8
#define PARAGOOMBA_WINGED_BBOX_WIDTH		20
#define PARAGOOMBA_WINGED_BBOX_HEIGHT		18
#define PARAGOOMBA_WINGED_BIG_BBOX_HEIGHT	24

#define GOOMBA_SCORE						100

#define GOOMBA_STATE_WALKING					100
#define GOOMBA_STATE_DIE						200
#define PARAGOOMBA_WINGED_STATE_JUMP			300
#define PARAGOOMBA_WINGED_STATE_JUMP_HEIGHT		400

#define GOOMBA_ANI_WALKING						0
#define GOOMBA_ANI_DIE							1
#define	PARAGOOMBA_WINGED_BIG_ANI				2
#define	PARAGOOMBA_WINGED_ANI					3
#define	PARAGOOMBA_WALKING_ANI					4
#define	PARAGOOMBA_DIE_ANI						5
#define GOOMBA_NORMAL		1
#define PARA_GOOMBA			2

#define PARAGOOMBA_LEVEL					0
#define PARAGOOMBA_WINGED_LEVEL				1
#define PARAGOOMBA_JUMP_COUNT				3
class CGoomba : public CGameObject
{
	int ani;
	DWORD Die_start;
	bool Died;
	int Die , JumpCount;
	
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	void StartDie() { Die = 1; Die_start = GetTickCount64(); }

public: 	
	Effect* effect = NULL;
	int TypeGoomba, LevelParaGoomba = 1;
	CGoomba(int TypeGoomba);
	virtual void SetState(int state);
};