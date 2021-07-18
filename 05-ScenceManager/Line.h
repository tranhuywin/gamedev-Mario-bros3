#pragma once
#include "GameObject.h"
#define LINE_BBOX_HEIGHT 1

class Line : public CGameObject
{
	float Width;
	float X_Left, X_Right;
public:
	Line(float Width);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	float GetX_Left() { return X_Left; };
	float GetX_Right() { return X_Right; };
};

