#pragma once
#include "GameObject.h"
#include "Mario.h"

#define ITEM_TREE_LEAF			0
#define ITEM_MONEY				1
#define ITEM_SWITCH				2

#define ITEM_BBOX_L				4
#define ITEM_BBOX_T				4
#define ITEM_BBOX_R_ACTIVE		7
#define ITEM_BBOX_B_ACTIVE		7
#define ITEM_BBOX_R				14
#define ITEM_BBOX_B				17
#define ITEM_BBOX_SWITCH		16
#define ITEM_BBOX_SWITCH_B		7

#define ITEM_DEFLECT_SPEED						0.02f
#define ITEM_GRAVITY							0.0015f
#define ITEM_TREE_LEAF_VX						0.0005f
#define ITEM_TREE_LEAF_X_DISTANCE_X_START		20.0f
#define ITEM_SWITCH_VY							0.001f
#define ITEM_SWITCH_YSTART_DISTANCE_Y			14.0f
#define ITEM_STATE_LISTEN			100
#define ITEM_STATE_ATIVE			200
#define ITEM_SWITCH_STATE_OFF		300

#define ITEM_ANI_TREE_LEAF			0
#define ITEM_ANI_MONEY				1
#define ITEM_ANI_SWITCH_ON			2
#define ITEM_ANI_SWITCH_OFF			3
class Items : public CGameObject
{
public:
	int IdItem;
	bool Active, SetPosStart;
	float X_Start, Y_Start;
	//vector <LPGAMEOBJECT> ListEffects;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	Items(int IdItem);
	virtual void SetState(int state);
};

