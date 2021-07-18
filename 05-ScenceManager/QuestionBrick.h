#pragma once
#include "GameObject.h"

#define QBRICK_BBOX_WIDTH  15.5f
#define QBRICK_BBOX_HEIGHT 15.0f

#define BRICK_DEFLECT_SPEED				0.01f
#define BRICK_DISTANCE_DEFLECT			10.0f

#define BRICK_STATE_QUESTION_ON			0
#define BRICK_STATE_QUESTION_OFF		1
#define BRICK_STATE_QUESTION_ON_UP		2
#define BRICK_STATE_QUESTION_ON_DOWN	3

#define BRICK_ANI_QUESTION_ON			0
#define BRICK_ANI_QUESTION_OFF			1
class QuestionBrick : public CGameObject
{
	bool OntoOff;
public:
	float YCollition = 0;
	QuestionBrick();
	virtual void Render();
	virtual void Update(ULONGLONG dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	void SetState(int state);
};