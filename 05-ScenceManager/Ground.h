#pragma once
#include "GameObject.h"

#define GROUND_BBOX_HEIGHT 32
#define GROUND_BBOX_WIDTH 16

class Ground : public CGameObject
{
	float Width;
public:
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};
