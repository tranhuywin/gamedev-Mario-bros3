#include "FireBullet.h"
#include <algorithm>
#include <assert.h>
#include "Utils.h"
#include "Game.h"

FireBullet::FireBullet()
{
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(4));
}
void FireBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	CGameObject::Update(dt);
	if ( PosPlatform - y > FIRE_BULLET_DISTANCE_PLATFORM)
	{
		vy = FIRE_BULLET_SPEED_Y;
	}
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;
		if (ny == -1)
		{
			OnPlatform = true;
			PosPlatform = y;
		}
		else
			OnPlatform = false;
		vy = -FIRE_BULLET_SPEED_Y;

	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}
void FireBullet::Render()
{
	int ani = 0;
	int alpha = 255;
	animation_set->at(ani)->Render(x, y, alpha);
	//RenderBoundingBox();
}
void FireBullet::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + FIRE_BULLET_BBOX_WIDTH;
	bottom = y + FIRE_BULLET_BBOX_WIDTH;
}
void FireBullet::attack(float x, float y, bool isRight)
{
	this->x = x;
	this->y = y;
	if (isRight)
		vx = FIRE_BULLET_SPEED_X;
	else
		vx = -FIRE_BULLET_SPEED_X;
	vy = FIRE_BULLET_SPEED_Y;
}
