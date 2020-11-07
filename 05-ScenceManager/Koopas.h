#pragma once

#include "GameObject.h"

#define KOOPAS_WALKING_SPEED 0.03f;
#define KOOPAS_ROTATORY_SPEED 0.1f;

#define KOOPAS_BBOX_WIDTH 16
#define KOOPAS_BBOX_HEIGHT  26
#define KOOPAS_BBOX_HEIGHT_SHELL 16

#define KOOPAS_STATE_WALKING		100
#define KOOPAS_STATE_SHELL			200
#define KOOPAS_STATE_ROTATORY		300
#define KOOPAS_STATE_DIE			9999

#define KOOPAS_ANI_WALKING_RIGHT		0
#define KOOPAS_ANI_WALKING_LEFT			1
#define KOOPAS_ANI_ROTATORY_RIGHT		2
#define KOOPAS_ANI_ROTATORY_LEFT		3
#define KOOPAS_ANI_SHELL				4
class CKoopas : public CGameObject
{
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();

public:
	CKoopas();
	virtual void SetState(int state);
	void BeCatch(LPGAMEOBJECT mario, float YShell);
};