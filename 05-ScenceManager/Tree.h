#pragma once
#include "GameObject.h"
#define BBOX_TREE 16
class Tree : public CGameObject
{
	int IdObj;
public:
	virtual void Update(ULONGLONG dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	Tree(int IdObj);
};

