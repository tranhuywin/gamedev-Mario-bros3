#pragma once
#pragma once
#include "GameObject.h"
class Tail : public CGameObject
{
	
public:
	bool IsKilling;
	Tail(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Render();
};


