#pragma once
#include "GameObject.h"

#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 15

#define BRICK_DEFLECT_SPEED				0.01
#define BRICK_DISTANCE_DEFLECT			10

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
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	void SetState(int state);
};