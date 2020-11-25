#pragma once
#include "GameObject.h"
#define WOODEN_BRICK_WIDTH 16
class WoodenBrick : public CGameObject
{
public:
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

