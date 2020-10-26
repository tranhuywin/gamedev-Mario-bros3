#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Game.h"

#include "Tail.h"
#include "Goomba.h"

Tail::Tail(float x, float y) : CGameObject()
{
	this->x = x - 5.0f;
	this->y = y;
}

void Tail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//if (!IsKilling)
	//	return;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	if (coEvents.size() != 0)
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
		DebugOut(L"Prepare collision\n");
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CGoomba*>(e->obj)) // if e->obj is Goomba 
			{
				DebugOut(L"collision Goomba\n");
				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
				if (goomba->GetState() != GOOMBA_STATE_DIE)
				{
					goomba->SetState(GOOMBA_STATE_DIE);
				}
			}
		}
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
}
void Tail::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + 30.0f;
	top = y;
	right = x + 70.0f;
	bottom = y + 28.0f;
	DebugOut(L"BBox tail\n");
}
void Tail::Render()
{
	RenderBoundingBox();
}