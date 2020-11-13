#pragma once
#include "GameObject.h"

#define TUBE_BBOX_WIDTH		32

#define X_TUBE_1			352	
#define X_TUBE_2			1792
#define X_TUBE_3			1856
#define X_TUBE_4			2256
#define X_TUBE_5			2255
#define X_TUBE_6			2320

#define TUBE_BBOX_HIGHT_1	48
#define TUBE_BBOX_HIGHT_2	32
#define TUBE_BBOX_HIGHT_3	48
#define TUBE_BBOX_HIGHT_4	192
#define TUBE_BBOX_HIGHT_5	32
#define TUBE_BBOX_HIGHT_6	32
class Tube : public CGameObject
{
public:
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

