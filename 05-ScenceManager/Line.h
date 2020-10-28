#pragma once
#include "GameObject.h"
#define LINE_BBOX_HEIGHT 1
#define LINE_BBOX_WIDTH 16
class Line : public CGameObject
{
	float Width;
public:
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

