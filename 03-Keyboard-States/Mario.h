#pragma once
#include "GameObject.h"

#define MARIO_WALKING_SPEED		0.1f
#define MARIO_JUMP_SPEED_Y		0.2f
#define MARIO_GRAVITY			0.005f
#define MARIO_RUNNING_SPEED		0.05f

#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200
#define MARIO_STATE_JUMP			300
#define MARIO_STATE_BEND_DOWN		400
#define MARIO_STATE_DROP			500
#define MARIO_STATE_FAST			600

#define MARIO_ANI_IDLE_RIGHT		0
#define MARIO_ANI_IDLE_LEFT			1
#define MARIO_ANI_WALKING_RIGHT		2
#define MARIO_ANI_WALKING_LEFT		3
#define MARIO_ANI_BEND_DOWN_RIGHT	4
#define MARIO_ANI_BEND_DOWN_LEFT	5

class CMario : public CGameObject
{
private: 
	bool IsJumping = false;
	bool IsRuning = false;
public: 

	void Update(DWORD dt);
	void Render();
	void SetState(int state);
};