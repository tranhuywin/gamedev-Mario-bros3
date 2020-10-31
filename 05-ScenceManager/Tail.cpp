#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Game.h"

#include "Tail.h"
#include "Goomba.h"

Tail::Tail()
{
	//this->SetPosition(-100, 100);
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(2));
}

void Tail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//if (!IsKilling)
	//	return;
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
		DebugOut(L"collision Goomba\n");
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			DebugOut(L"collision Goomba\n");
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
	left = x;
	top = y;
	right = x + 50.0f;
	bottom = y + 28.0f;
}
void Tail::Render()
{
	int ani = 0;
	int alpha = 255;
	animation_set->at(ani)->Render(x, y, alpha);
	RenderBoundingBox();
}
void Tail::Attack(float x, float y)
{
	this->x = x;
	this->y = y;
}
