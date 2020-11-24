#include "Brick.h"
#include "Utils.h"

CBrick::CBrick() {
	state = BRICK_STATE_QUESTION_ON;
}
void CBrick::Render()
{
	animation_set->at(state)->Render(x,y);
	RenderBoundingBox();
}

void CBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	PreviousState = state;
	CGameObject::Update(dt);
	if (OntoOff)
	{
		this->vy -= 0.001 * dt;
		if (this->vy < 3 * (-0.001 * dt))
			this->vy += 0.001 * dt;
		
	}
	y += dy;
}

void CBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + BRICK_BBOX_WIDTH;
	b = y + BRICK_BBOX_HEIGHT;
}

void CBrick::SetState(int state)
{
	if (PreviousState == BRICK_STATE_QUESTION_ON && state == BRICK_STATE_QUESTION_OFF)
		OntoOff = true;

	this->state = BRICK_STATE_QUESTION_ON;
	if (state == BRICK_STATE_QUESTION_OFF && this->state == BRICK_STATE_QUESTION_ON)
		OntoOff = true;
	if (state == BRICK_STATE_QUESTION_OFF)
		this->state = BRICK_STATE_QUESTION_OFF;
}
