#include "MusicNote.h"
#include "Utils.h"
#include "Textures.h"
#include "Game.h"

MusicNote::MusicNote(int typeNote)
{
	this->typeNote = typeNote;
	SetState(MUSIC_NOTE_STATE_IDLE);
}

void MusicNote::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	if (typeNote == MUSIC_NOTE_TYPE_WHILE)
	{
		right = x + MUSIC_NOTE_BBOX;
		bottom = y + MUSIC_NOTE_BBOX;
	}
	else if (typeNote == MUSIC_NOTE_TYPE_PINK)
	{
		right = x + MUSIC_NOTE_BBOX - 1;
		bottom = y + MUSIC_NOTE_BBOX;
		if (!appear)
			bottom = y + 1;
	}

}

void MusicNote::Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isFirstUpdate)
	{
		initX = this->x;
		initY = this->y;
		isFirstUpdate = false;
	}
	if (GetState() == MUSIC_NOTE_STATE_IDLE && (this->x != initX || this->y != initY)) {
		this->x = initX; this->y = initY;
	}
	
	if (GetState() == MUSIC_NOTE_STATE_MOVE_UP)
	{
		
		if (this->y - initY <= 0)
		{
			SetState(MUSIC_NOTE_STATE_IDLE);
			//this->y = initY; 
		}
	}
	else if(GetState() == MUSIC_NOTE_STATE_MOVE_DOWN)
	{
		
		if (this->y - initY >= MUSIC_NOTE_DISTANCE_MOVE)
		{
			SetState(MUSIC_NOTE_STATE_MOVE_UP);
		}
	}
	else if (GetState() == MUSIC_NOTE_STATE_MOVE_LEFT_RIGHT)
	{
		if (initX - this->x <= 0)
		{
			SetState(MUSIC_NOTE_STATE_IDLE);
		}
	}
	else if (GetState() == MUSIC_NOTE_STATE_MOVE_RIGHT_RIGHT)
	{
		if (this->x - initX > MUSIC_NOTE_DISTANCE_MOVE + 2)
		{
			SetState(MUSIC_NOTE_STATE_MOVE_LEFT_RIGHT);
		}
	}
	// con` bi loi bat lai cua note
	else if (GetState() == MUSIC_NOTE_STATE_MOVE_LEFT_LEFT)
	{
		if (initX - this->x > MUSIC_NOTE_DISTANCE_MOVE + 2)
		{
			SetState(MUSIC_NOTE_STATE_MOVE_RIGHT_LEFT);
		}
	}
	else if (GetState() == MUSIC_NOTE_STATE_MOVE_RIGHT_LEFT)
	{
		if (initX -this->x <= 0)
		{
			SetState(MUSIC_NOTE_STATE_IDLE);
		}
	}

	x += dx;
	y += dy;
	CGameObject::Update(dt);
}

void MusicNote::Render()
{
	if(typeNote == MUSIC_NOTE_TYPE_WHILE)
		animation_set->at(MUSIC_NOTE_WHILE_ANI)->Render(x, y);
	else if (typeNote == MUSIC_NOTE_TYPE_PINK && appear)
	{
		if(GetState() == MUSIC_NOTE_STATE_IDLE)
			animation_set->at(MUSIC_NOTE_PINK_ANI)->Render(x, y);
		else
			animation_set->at(MUSIC_NOTE_WHILE_ANI)->Render(x, y);
	}
}
	
void MusicNote::SetState(int state)
{
	switch (state)
	{
	case MUSIC_NOTE_STATE_IDLE:
		this->vy = 0;
		this->vx = 0;
		break;
	case MUSIC_NOTE_STATE_MOVE_UP:
		this->vy = -MUSIC_NOTE_VY * dt;
		break;
	case MUSIC_NOTE_STATE_MOVE_DOWN:
		this->vy = MUSIC_NOTE_VY * dt;
		//initY = this->y; // pre commit
		break;
	case MUSIC_NOTE_STATE_MOVE_LEFT_RIGHT:
		this->vx = -MUSIC_NOTE_VX * dt;
		break;
	case MUSIC_NOTE_STATE_MOVE_RIGHT_RIGHT:
		this->vx = MUSIC_NOTE_VX * dt;
		break;
	case MUSIC_NOTE_STATE_MOVE_LEFT_LEFT:
		this->vx = -MUSIC_NOTE_VX * dt;
		break;
	case MUSIC_NOTE_STATE_MOVE_RIGHT_LEFT:
		this->vx = MUSIC_NOTE_VX * dt;
		break;
	}
	CGameObject::SetState(state);
}
