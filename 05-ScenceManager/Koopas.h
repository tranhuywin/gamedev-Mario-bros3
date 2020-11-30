#pragma once

#include "GameObject.h"

#define KOOPAS_WALKING_SPEED	0.05f
#define KOOPAS_ROTATORY_SPEED	0.2f
#define KOOPAS_DIE_DEFLECT_SPEED 0.02f
#define KOOPAS_GRAVITY			0.001f
#define KOOPAS_BBOX_WIDTH 16
#define KOOPAS_BBOX_HEIGHT  26
#define KOOPAS_BBOX_HEIGHT_SHELL 16

#define KOOPAS_STATE_WALKING		100
#define KOOPAS_STATE_SHELL			200
#define KOOPAS_STATE_ROTATORY		300
#define KOOPAS_STATE_PREPARE_WAKE_UP	400
#define KOOPAS_STATE_DIE			9999

#define KOOPAS_ANI_WALKING_RIGHT		0
#define KOOPAS_ANI_WALKING_LEFT			1
#define KOOPAS_ANI_ROTATORY_RIGHT		2
#define KOOPAS_ANI_ROTATORY_LEFT		3
#define KOOPAS_ANI_SHELL				4
#define KOOPAS_ANI_SHELL_TAIL_ATTACK	5
#define KOOPAS_ANI_SHELL_TAIL_ATTACK_ROTATORY	6
#define KOOPAS_ANI_SHELL_TAIL_ATTACK_PREPARE_WAKE_UP	7
class CKoopas : public CGameObject
{
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	bool ShakingLeft;
	int Sleep, PrepareWakeUp;
	DWORD Sleep_start, PrepareWakeUp_start;
public:
	bool TailAttack;
	bool IsCatching;
	CKoopas();
	virtual void SetState(int state);
	void BeCatch(LPGAMEOBJECT mario, float YShell);
	void StartSleep() { Sleep = 1; Sleep_start = GetTickCount(); }
	void StartPrepareWakeUp() { PrepareWakeUp = 1; PrepareWakeUp_start = GetTickCount(); }
};