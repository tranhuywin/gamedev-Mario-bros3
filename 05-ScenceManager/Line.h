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

#define X_LINE_1 231
#define X_LINE_2 265
#define X_LINE_3 391
#define X_LINE_4 455
#define X_LINE_5 503
#define X_LINE_6 502
#define X_LINE_7 1303
#define X_LINE_8 1256
#define X_LINE_9 1287
#define X_LINE_10 1319
#define X_LINE_11 2136
#define X_LINE_12 2167
class Line : public CGameObject
{
	float Width;
public:
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

