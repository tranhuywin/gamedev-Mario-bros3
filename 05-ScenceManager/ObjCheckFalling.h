#pragma once
#include "GameObject.h"
class ObjCheckFalling :public CGameObject
{
	float mdy;
public:
	bool isFalling = false;
	ObjCheckFalling();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* listMapObj);
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};