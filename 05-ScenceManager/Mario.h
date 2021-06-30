#pragma once
#include "GameObject.h"
#include "Game.h"
#include "FireBullet.h"
#include "Tail.h"
#include "Koopas.h"

#define MARIO_WALKING_SPEED				0.0005f
#define MARIO_MAX_SPEED_RUNNING			0.0125f 
//0.1f
#define X_RETURN_WORLD_1			2336
#define Y_RETURN_WORLD_1			375

#define MARIO_JUMP_SPEED_Y			0.01f
#define MARIO_DROP_VY_MAX			0.0125f
#define MARIO_JUMP_DEFLECT_SPEED	0.2f
#define MARIO_GRAVITY				0.002f
#define MARIO_GRAVITY_TELEPORT		0.000015f
#define MARIO_FALL_SLOWLY_SPEED		0.0025f
#define MARIO_DIE_DEFLECT_SPEED		0.03125f
#define MARIO_INERTIA_DECREASE		0.0002f
#define MARIO_DISTANCE_INERTIA		30.0f
#define MARIO_DISTANCE_JUMP			60.0f
#define MARIO_DISTANCE_SLOW_DROP	20.0f
#define MARIO_FLY_SPEED_Y			0.008f
#define	MARIO_OVER_LINE				3.0f
#define	MARIO_BIG_STAND_Y			10.0f
#define MARIO_RACCOON_STAND_Y		14.0f
#define MARIO_Y_BEND_OVER			3.0f
#define MARIO_KILL_LEFT_TAIL		3.0f
#define MARIO_VY_ASCENDING_SLIP		0.00025f
#define MARIO_VY_ASCENDING_SLIP_MAX	0.00625f
#define MARIO_VY_ASCENDING_RUN		0.000093f
#define MARIO_START_TELEPORT_VY		0.001f
#define MARIO_DEFLECT_VX			0.001f

#define MARIO_STATE_IDLE				0
#define MARIO_STATE_WALKING_RIGHT		100
#define MARIO_STATE_WALKING_LEFT		200
#define MARIO_STATE_JUMP				300
#define MARIO_STATE_DROP				350
#define MARIO_STATE_DIE					400
#define MARIO_STATE_K_UP_WALKING_RIGHT	500
#define MARIO_STATE_K_UP_WALKING_LEFT	600
#define MARIO_STATE_KILL				700
#define MARIO_STATE_FAST_RUN			800
#define MARIO_STATE_BEND_OVER			900
#define MARIO_STATE_STAND				1000
#define MARIO_STATE_SKILL_ON			1100 
#define MARIO_STATE_SKILL_OFF			1200
#define MARIO_STATE_UP					1300
//start scence
#define MARIO_MINI_SPEED				0.1f
#define MARIO_MINI_STATE_UP				1300
#define MARIO_MINI_STATE_DOWN			1400
#define MARIO_MINI_STATE_LEFT			1500
#define MARIO_MINI_STATE_RIGHT			1600
#define MARIO_MINI_STATE_TELEPORT		1700

#define MARIO_MINI_ANI					0
//
#define MARIO_ANI_BIG_IDLE_RIGHT			0
#define MARIO_ANI_BIG_IDLE_LEFT				1
#define MARIO_ANI_SMALL_IDLE_RIGHT			2
#define MARIO_ANI_SMALL_IDLE_LEFT			3
#define MARIO_ANI_RACCOON_IDLE_RIGHT		4
#define MARIO_ANI_RACCOON_IDLE_LEFT			5

#define MARIO_ANI_BIG_WALKING_RIGHT			6
#define MARIO_ANI_BIG_WALKING_LEFT			7
#define MARIO_ANI_SMALL_WALKING_RIGHT		8
#define MARIO_ANI_SMALL_WALKING_LEFT		9
#define MARIO_ANI_RACCOON_WALKING_RIGHT		10
#define MARIO_ANI_RACCOON_WALKING_LEFT		11
#define MARIO_ANI_RACCOON_JUMP_RIGHT		12
#define MARIO_ANI_RACCOON_JUMP_LEFT			13
#define MARIO_ANI_RACCOON_KILL_RIGHT		14
#define MARIO_ANI_RACCOON_KILL_LEFT			15
#define MARIO_ANI_RACCOON_DROP_RIGHT		16
#define MARIO_ANI_RACCOON_DROP_LEFT			17
#define MARIO_ANI_RACCOON_PREPARE_FLY_RIGHT	18
#define MARIO_ANI_RACCOON_FLY_RIGHT			19
#define MARIO_ANI_RACCOON_DROP_SLOW_RIGHT	20
#define MARIO_ANI_RACCOON_DROP_SLOW_LEFT	21
#define MARIO_ANI_SLIP_RIGHT				22
#define MARIO_ANI_SLIP_LEFT					23
#define MARIO_ANI_BIG_JUMP_RIGHT			24
#define MARIO_ANI_BIG_JUMP_LEFT				25
#define	MARIO_ANI_RACCOON_SLIP_LEFT			26
#define	MARIO_ANI_RACCOON_SLIP_RIGHT		27
#define MARIO_ANI_BIG_BEND_OVER_RIGHT		28
#define MARIO_ANI_BIG_BEND_OVER_LEFT		29
#define MARIO_ANI_RACCOON_BEND_OVER_RIGHT	30
#define MARIO_ANI_RACCOON_BEND_OVER_LEFT	31
#define MARIO_ANI_RACCOON_CATCHING_IDLE_RIGHT		32
#define MARIO_ANI_RACCOON_CATCHING_WALKING_RIGHT	33
#define MARIO_ANI_RACCOON_CATCHING_IDLE_LEFT		34
#define MARIO_ANI_RACCOON_CATCHING_WALKING_LEFT		35
#define MARIO_ANI_BIG_MAX_SPEED_RUNNING_RIGHT		36
#define MARIO_ANI_BIG_MAX_SPEED_RUNNING_LEFT		37	// TO DO: set ani cho mario
#define MARIO_ANI_BIG_MAX_SPEED_JUMP_RIGHT			38
#define MARIO_ANI_BIG_MAX_SPEED_JUMP_LEFT			39
#define MARIO_ANI_RACCOON_KICK_RIGHT		40
#define MARIO_ANI_RACCOON_KICK_LEFT			41
#define MARIO_ANI_RACCOON_TELEPORT			42
#define MARIO_ANI_DIE						43			//599

#define MARIO_ANI_FIRE_IDLE_RIGHT			44
#define MARIO_ANI_FIRE_WALK_RIGHT			45
#define MARIO_ANI_FIRE_JUMP_RIGHT			46
#define MARIO_ANI_FIRE_IDLE_LEFT			47
#define MARIO_ANI_FIRE_WALK_LEFT			48
#define MARIO_ANI_FIRE_JUMP_LEFT			49
#define MARIO_ANI_FIRE_ATTACK_RIGHT			50
#define MARIO_ANI_FIRE_ATTACK_LEFT			51
#define MARIO_ANI_FIRE_SLIP_LEFT			52
#define MARIO_ANI_FIRE_SLIP_RIGHT			53
#define MARIO_ANI_FIRE_BEND_OVER_RIGHT		54
#define MARIO_ANI_FIRE_BEND_OVER_LEFT		55			//1011
#define MARIO_ANI_FIRE_MAX_SPEED_RUNNING_RIGHT		56
#define MARIO_ANI_FIRE_MAX_SPEED_RUNNING_LEFT		57
#define MARIO_ANI_FIRE_MAX_SPEED_JUMP_RIGHT			58
#define MARIO_ANI_FIRE_MAX_SPEED_JUMP_LEFT			59

#define MARIO_ANI_SMALL_JUMP_RIGHT					60
#define MARIO_ANI_SMALL_JUMP_LEFT					61

#define MARIO_LEVEL_MINI		0
#define	MARIO_LEVEL_SMALL		1
#define	MARIO_LEVEL_BIG			2
#define	MARIO_LEVEL_RACCOON		3
#define MARIO_LEVEL_FIRE		4

#define MARIO_BIG_BBOX_WIDTH			16
#define MARIO_BIG_BBOX_HEIGHT			27
#define MARIO_BIG_BBOX_BEND_OVER_WIDTH			14
#define MARIO_BIG_BBOX_BEND_OVER_HEIGHT			18

#define MARIO_SMALL_BBOX_WIDTH			13
#define MARIO_SMALL_BBOX_HEIGHT			15

#define MARIO_RACCOON_BBOX_WIDTH		20
#define MARIO_RACCOON_BBOX_HEIGHT		27
#define MARIO_RACCOON_KILL_BBOX_WIDTH	23
#define	MARIO_RACCOON_BBOX_TAIL			8
#define MARIO_RACCOON_BBOX_BEND_OVER_WIDTH		22
#define MARIO_RACCOON_BBOX_BEND_OVER_HEIGHT		18

#define MARIO_UNTOUCHABLE_TIME			5000
#define MARIO_FLY_TIME					500
#define MARIO_SLIP_TIME					1000
//#define MARIO_FIRE_ATTACK_TIME			200
class CMario : public CGameObject
{
	int level;
	int untouchable;
	int Kill;
	int Fly;
	int Slip;
	int FireAttack;
	int ani = -1;
	int LastAni = -1;
	int alpha = 255;
	int CountMiliSeconds = 0;
	int NoCardStartGame = 0;
	DWORD untouchable_start;
	DWORD Kill_start;
	DWORD Fly_start;
	DWORD Slip_start;
	DWORD FireAttack_start;
	DWORD Kick_start;

	float start_x;			// initial position of Mario at scene
	float start_y; 
	float XHolding;
	float YHolding;
	float mdy = 0;

	bool IsRendering;
	bool HaveInertia;
	bool IsJumping;
	bool IsSlowDropping;
	bool AllowJump;
	bool TeleUp;
	bool IsBendingOver;
	bool IsCatching;
	bool PrepareCatch;
	bool SkillOn;
	bool KickShell;
	bool isDeflect;
	FireBullet* firebullet_1 = new FireBullet();
	FireBullet* firebullet_2 = new FireBullet();
	CKoopas* Shell;
	Tail* TailofRaccoon = new Tail();
	
public: 
	bool OnPlatform;
	bool IsFlying;
	bool IsLimitRunning;
	bool IsLimitFlying;
	bool Iskilling;
	bool IsRunning, IsDropping;
	bool isJumped;
	bool IsWaitingTeleport;
	bool StartTeleport;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();

	CMario(float x = 0.0f, float y = 0.0f);
	bool CheckMarioInScreen();
	void SetState(int state);
	void SetLevel(int l);
	int GetLevel() { return level; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount64(); }
	void StartKill() { Kill = 1; Kill_start = GetTickCount64(); }
	void Startfly() { Fly = 1; Fly_start = GetTickCount64(); }
	void StartSlip() { Slip = 1; Slip_start = GetTickCount64(); }
	void StartFireAttack() { FireAttack = 1; FireAttack_start = GetTickCount64(); }
	void Reset();
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};