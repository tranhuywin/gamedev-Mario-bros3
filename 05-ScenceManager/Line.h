#pragma once
#include "GameObject.h"
#define LINE_BBOX_HEIGHT 1
#define LINE_BBOX_WIDTH_1 47
#define LINE_BBOX_WIDTH_2 47
#define LINE_BBOX_WIDTH_3 79
#define LINE_BBOX_WIDTH_4 64
#define LINE_BBOX_WIDTH_5 64
#define LINE_BBOX_WIDTH_6 95
#define LINE_BBOX_WIDTH_7 64
#define LINE_BBOX_WIDTH_8 111
#define LINE_BBOX_WIDTH_9 111
#define LINE_BBOX_WIDTH_10 111
#define LINE_BBOX_WIDTH_11 46
#define LINE_BBOX_WIDTH_12 46

#define X_LINE_1 240
#define X_LINE_2 274
#define X_LINE_3 400	
#define X_LINE_4 464	
#define X_LINE_5 512	
#define X_LINE_6 511	
#define X_LINE_7 1312	
#define X_LINE_8 1265	
#define X_LINE_9 1296	
#define X_LINE_10 1328	
#define X_LINE_11 2145	
#define X_LINE_12 2176	
class Line : public CGameObject
{
	float Width;
	float X_Left, X_Right;
public:
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	int GetX_Left() { return X_Left; };
	float GetX_Right() { return X_Right; };
};

