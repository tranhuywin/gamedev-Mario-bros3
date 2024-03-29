#pragma once

#include "GameObject.h"
#include "Effect.h"
#include "ObjCheckFalling.h"
#define KOOPAS_WALKING_SPEED				0.001f
#define KOOPAS_PARATROOPA_GREEN_WALKING_SPEED				0.0022f
#define KOOPAS_PARATROOPA_WALKING_SPEED		0.01f
#define KOOPAS_ROTATORY_SPEED	0.2f
#define KOOPAS_DIE_DEFLECT_SPEED 0.02f
#define KOOPAS_SHIEL_DEFLECT_SPEED_VY 0.013f
#define KOOPAS_SHIEL_DEFLECT_SPEED_VX 0.003f
#define KOOPAS_GRAVITY			0.001f	//0.0019f
#define KOOPAS_PARATROOPA_GREEN_DEFLECT_SPEED  0.017f	//
#define KOOPAS_PARATROOPA_GREEN_FALL_SPEED  0.01f
#define KOOPAS_DIE				0.1f
#define KOOPAS_BBOX_WIDTH		 15
#define KOOPAS_BBOX_HEIGHT		 26
#define KOOPAS_BBOX_HEIGHT_SHELL 16

#define KOOPA_PARATROOPA_RED_FLY_X_MIN			32
#define KOOPA_PARATROOPA_RED_FLY_X_MAX			128
#define KOOPA_PARATROOPA_RED_FLY_VY				0.005f
#define KOOPAS_STATE_WALKING		100
#define KOOPAS_STATE_SHELL			200
#define KOOPAS_STATE_ROTATORY		300
#define KOOPAS_STATE_PREPARE_WAKE_UP	400
#define KOOPAS_STATE_FLYING_UP		500
#define KOOPAS_STATE_FLYING_DOWN	600
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
#define KOOPA_PARATROOPA_RED_ANI_FLYING_LEFT				20

#define KOOPAS_TIME_WAKE_UP								2000
#define KOOPAS_TIME_SLEEP								5000
#define KOOPAS_TYPE_KOOPA_TROOPA_RED					0
#define KOOPAS_TYPE_KOOPA_TROOPA_GREEN					1
#define KOOPAS_TYPE_KOOPA_PARATROOPA_GREEN				2
#define KOOPAS_TYPE_KOOPA_PARATROOPA_RED				3
class CKoopas : public CGameObject
{
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(ULONGLONG dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	bool ShakingLeft;
	int Sleep, PrepareWakeUp;
	float XLeftBrick = 0, XRightBrick = 0;
	ULONGLONG Sleep_start, PrepareWakeUp_start;
public:
	bool TailAttack, IsCatching;
	int TypeKoopas;
	Effect* effect = NULL;
	CKoopas(int TypeKoopas);
	ObjCheckFalling* objCheckFalling = new ObjCheckFalling();
	virtual void SetState(int state);
	void BeCatch(LPGAMEOBJECT mario, float YShell);
	void StartSleep() { Sleep = 1; Sleep_start = GetTickCount64(); }
	void StartPrepareWakeUp() { PrepareWakeUp = 1; PrepareWakeUp_start = GetTickCount64(); }
};