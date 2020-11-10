#pragma once
#include "GameObject.h"

#define GROUND_BBOX_HEIGHT 32
#define GROUND_BBOX_WIDTH_1 615
#define GROUND_BBOX_WIDTH_2 464
#define GROUND_BBOX_WIDTH_3 352
#define GROUND_BBOX_WIDTH_4 80
#define GROUND_BBOX_WIDTH_5 574
#define GROUND_BBOX_WIDTH_6 558

#define XGROUND_1 0
#define XGROUND_2 615
#define XGROUND_3 1143
#define XGROUND_4 1527
#define XGROUND_5 1655
#define XGROUND_6 2247

class Ground : public CGameObject
{
public:
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};
