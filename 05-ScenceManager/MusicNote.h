#pragma once
#include "GameObject.h"

#define MUSIC_NOTE_BBOX			16

#define MUSIC_NOTE_STATE_IDLE			100
#define MUSIC_NOTE_STATE_MOVE_UP		200
#define MUSIC_NOTE_STATE_MOVE_DOWN		300
#define MUSIC_NOTE_STATE_MOVE_LEFT_RIGHT		400
#define MUSIC_NOTE_STATE_MOVE_RIGHT_RIGHT		500
#define MUSIC_NOTE_STATE_MOVE_LEFT_LEFT			600
#define MUSIC_NOTE_STATE_MOVE_RIGHT_LEFT		700

#define MUSIC_NOTE_VY					0.006f
#define MUSIC_NOTE_VX					0.004f

#define MUSIC_NOTE_DISTANCE_MOVE		6

#define MARIO_IS_DEFLECTED_TOP			1
#define MARIO_IS_DEFLECTED_RIGHT		2
#define MARIO_IS_DEFLECTED_BOT			3
#define MARIO_IS_DEFLECTED_LEFT			4

#define MUSIC_NOTE_WHILE_ANI				0
#define MUSIC_NOTE_PINK_ANI					1

#define MUSIC_NOTE_TYPE_WHILE				1
#define MUSIC_NOTE_TYPE_PINK				2


class MusicNote : public CGameObject
{
	float initX, initY;
	bool isFirstUpdate = true;
public:
	bool appear = false;
	int typeNote = 1;
	float vxOfPlayer = 0;
	MusicNote(int typeNote);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void SetState(int state);
};

