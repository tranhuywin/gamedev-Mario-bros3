#pragma once
#include "GameObject.h"

#define FLYING_WOOD_BBOX_WIDTH		48
#define FLYING_WOOD_BBOX_HEIGHT		16

#define FLYING_WOOD_VX				0.002f
#define FLYING_WOOD_VY				0.003f
class FlyingWood : public CGameObject
{
public:
	bool IsCollMario;
	virtual void Update(ULONGLONG dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

