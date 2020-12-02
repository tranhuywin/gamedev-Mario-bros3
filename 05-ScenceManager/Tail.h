#pragma once
#pragma once
#include "GameObject.h"
#define OUTSIDE_MAP						500.0f
#define TAIL_BBOX_WIDTH					20.0f
#define RACCOON_BBOX_HIGHT				25.0f
class Tail : public CGameObject
{
	
public:
	bool IsKilling;
	Tail();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Render();
	void Attack(float x, float y, bool IsKilling,int CurrentFrame);
};


