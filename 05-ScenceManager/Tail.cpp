#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Game.h"

#include "Tail.h"
#include "Goomba.h"

Tail::Tail()
{
}

void Tail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() != 0)
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;
		//TODO: tail attack
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CGoomba*>(e->obj)) // if e->obj is Goomba 
			{
				
				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
				if (goomba->GetState() != GOOMBA_STATE_DIE)
				{
					goomba->SetState(GOOMBA_STATE_DIE);
				}
			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}
void Tail::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - TAIL_BBOX_WIDTH;
	top = y;
	right = x + TAIL_BBOX_WIDTH;
	bottom = y + RACCOON_BBOX_HIGHT;
}
void Tail::Render()
{
	//RenderBoundingBox();
}
void Tail::Attack(float x, float y, bool IsKilling)
{
	this->IsKilling = IsKilling;
	if (this->IsKilling)
	{
		this->x = x;
		this->y = y;
	}
	else
	{
		this->x = -OUTSIDE_MAP;
		this->y = OUTSIDE_MAP;
	}
}
