#pragma once
#include "GameObject.h"

#define MUSIC_NOTE_BBOX			16

#define MUSIC_NOTE_STATE_IDLE			100
#define MUSIC_NOTE_STATE_MOVE_UP		200
#define MUSIC_NOTE_STATE_MOVE_DOWN		300

#define MUSIC_NOTE_VY					0.005f

#define MUSIC_NOTE_ANI_IDLE				0
#define MUSIC_NOTE_ANI_MOVE				1

class MusicNote : public CGameObject
{
	float initX, initY;
public:
	MusicNote();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void SetState(int state);
};

