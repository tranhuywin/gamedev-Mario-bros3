#include "MusicNote.h"
#include "Utils.h"

MusicNote::MusicNote()
{
	SetState(MUSIC_NOTE_STATE_IDLE);
}

void MusicNote::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + MUSIC_NOTE_BBOX;
	bottom = y + MUSIC_NOTE_BBOX;
}

void MusicNote::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (GetState() == MUSIC_NOTE_STATE_MOVE_UP)
	{
		this->vy = -MUSIC_NOTE_VY * dt;
		if (this->y - initY <= 0)
		{
			SetState(MUSIC_NOTE_STATE_IDLE);
			this->y = initY;
		}
	}
	else if(GetState() == MUSIC_NOTE_STATE_MOVE_DOWN)
	{
		this->vy = MUSIC_NOTE_VY * dt;
		if (this->y - initY >= 8)
		{
			SetState(MUSIC_NOTE_STATE_MOVE_UP);
		}
	}

	x += dx;
	y += dy;
	CGameObject::Update(dt);
}

void MusicNote::Render()
{
	animation_set->at(0)->Render(x, y);
}
	
void MusicNote::SetState(int state)
{
	switch (state)
	{
	case MUSIC_NOTE_STATE_IDLE:
		vy = 0;		
		break;
	case MUSIC_NOTE_STATE_MOVE_UP:
		break;
	case MUSIC_NOTE_STATE_MOVE_DOWN:
		initY = this->y;
		break;
	}
	CGameObject::SetState(state);
}
