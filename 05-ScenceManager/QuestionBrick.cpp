#include "QuestionBrick.h"
#include "Utils.h"
#include "Mario.h"

QuestionBrick::QuestionBrick() {
	this->SetState(BRICK_STATE_QUESTION_ON);
	YCollition = y;
}
void QuestionBrick::Render()
{
	int ani = -1;
	switch (state)
	{
	case BRICK_STATE_QUESTION_OFF:
		ani = BRICK_ANI_QUESTION_OFF;
		break;
	default:
		ani = BRICK_ANI_QUESTION_ON;
		break;
	}
	animation_set->at(ani)->Render(x,y);
	//RenderBoundingBox();
}

void QuestionBrick::Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	y += dy;

	if ( YCollition - y >= BRICK_DISTANCE_DEFLECT && state == BRICK_STATE_QUESTION_ON_UP)
		this->SetState(BRICK_STATE_QUESTION_ON_DOWN);
	if (y > YCollition && state == BRICK_STATE_QUESTION_ON_DOWN)
		this->SetState(BRICK_STATE_QUESTION_OFF);

}

void QuestionBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x + 0.5f;
	t = y + 1.0f;
	r = x + QBRICK_BBOX_WIDTH;
	b = y + QBRICK_BBOX_HEIGHT - 0.5f;
}

void QuestionBrick::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
		case BRICK_STATE_QUESTION_OFF:
			vy = 0;
			y = YCollition;
 			break;
		case BRICK_STATE_QUESTION_ON_UP:
			vy = -BRICK_DEFLECT_SPEED * dt;
			break;
		case BRICK_STATE_QUESTION_ON_DOWN:
			vy = BRICK_DEFLECT_SPEED * dt;
			break;
		case BRICK_STATE_QUESTION_ON:
			vy = 0;
			break;
	}

}
