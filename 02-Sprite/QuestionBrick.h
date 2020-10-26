#pragma once
#include "GameObject.h"

//#define MARIO_WIDTH 14

class CQuestionBrick : public CGameObject
{
public:
	CQuestionBrick(float x, float y);
	void Update(DWORD dt);
	void Render();
};
