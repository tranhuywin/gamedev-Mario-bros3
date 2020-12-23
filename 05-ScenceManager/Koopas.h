#pragma once

#include "GameObject.h"

#define KOOPAS_WALKING_SPEED				0.04f
#define KOOPAS_PARATROOPA_WALKING_SPEED		0.01f
#define KOOPAS_ROTATORY_SPEED	0.2f
#define KOOPAS_DIE_DEFLECT_SPEED 0.02f
#define KOOPAS_GRAVITY			0.001f
#define KOOPAS_DIE				0.1f
#define KOOPAS_BBOX_WIDTH 16
#define KOOPAS_BBOX_HEIGHT  26
#define KOOPAS_BBOX_HEIGHT_SHELL 16

#define KOOPAS_STATE_WALKING		100
#define KOOPAS_STATE_SHELL			200
#define KOOPAS_STATE_ROTATORY		300
#define KOOPAS_STATE_PREPARE_WAKE_UP	400
#define KOOPAS_STATE_DIE			9999

#define KOOPAS_ANI_WALKING_RIGHT_RED						0
#define KOOPAS_ANI_WALKING_LEFT_RED							1
#define KOOPAS_ANI_ROTATORY_RIGHT_RED						2
#define KOOPAS_ANI_ROTATORY_LEFT_RED						3
#define KOOPAS_ANI_SHELL_RED								4
#define KOOPAS_ANI_SHELL_TAIL_ATTACK_RED					5
#define KOOPAS_ANI_SHELL_TAIL_ATTACK_ROTATORY_RED			6
#define KOOPAS_ANI_SHELL_TAIL_ATTACK_PREPARE_WAKE_UP_RED	7
#define KOOPAS_ANI_WALKING_RIGHT_GREEN						8
#define KOOPAS_ANI_WALKING_LEFT_GREEN						9
#define KOOPAS_ANI_ROTATORY_RIGHT_GREEN						10
#define KOOPAS_ANI_ROTATORY_LEFT_GREEN						11
#define KOOPAS_ANI_SHELL_GREEN								12
#define KOOPAS_ANI_SHELL_TAIL_ATTACK_GREEN					13
#define KOOPAS_ANI_SHELL_TAIL_ATTACK_ROTATORY_GREEN			14
#define KOOPAS_ANI_SHELL_TAIL_ATTACK_PREPARE_WAKE_UP_GREEN	15
#define KOOPA_PARATROOPA_GREEN_ANI_WING_RIGHT				16
#define KOOPA_PARATROOPA_GREEN_ANI_JUMP_RIGHT				17
#define KOOPA_PARATROOPA_GREEN_ANI_WING_LEFT				18
#define KOOPA_PARATROOPA_GREEN_ANI_JUMP_LEFT				19

#define KOOPAS_TIME_WAKE_UP								2000
#define KOOPAS_TIME_SLEEP								5000
#define KOOPAS_TYPE_KOOPA_TROOPA_RED					0
#define KOOPAS_TYPE_KOOPA_TROOPA_GREEN					1
#define KOOPAS_TYPE_KOOPA_PARATROOPA_GREEN				2
class CKoopas : public CGameObject
{
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	bool ShakingLeft;
	int Sleep, PrepareWakeUp;
	DWORD Sleep_start, PrepareWakeUp_start;
public:
	bool TailAttack, IsCatching;
	int TypeKoopas;
	CKoopas(int TypeKoopas);
	virtual void SetState(int state);
	void BeCatch(LPGAMEOBJECT mario, float YShell);
	void StartSleep() { Sleep = 1; Sleep_start = GetTickCount(); }
	void StartPrepareWakeUp() { PrepareWakeUp = 1; PrepareWakeUp_start = GetTickCount(); }
};