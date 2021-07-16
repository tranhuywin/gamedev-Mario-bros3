#pragma once

#include "GameObject.h"
#define PLAYER_MOVE_UP		-1
#define PLAYER_MOVE_IDLE	0
#define PLAYER_MOVE_DOWN	1
/*
	Object that triggers scene switching
*/
class CPortal : public CGameObject
{
	int scene_id;	// target scene to switch to 
	int width;
	int height; 
public:
	int playerMove;
	CPortal(float l, float t, float r, float b, int scene_id, int playerMove);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	int GetSceneId() { return scene_id;  }
};