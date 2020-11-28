#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Game.h"

#include "Tail.h"
#include "Goomba.h"
#include "Koopas.h"

Tail::Tail()
{
}

void Tail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	vector<LPGAMEOBJECT> coEventsResult;
	coEventsResult.clear();
	if(IsKilling)
		CalCollisions(coObjects, coEventsResult);
	int sizeCo = coEventsResult.size();

	if (sizeCo != 0)
	{
		for (UINT i = 0; i < sizeCo; i++)
		{
			LPGAMEOBJECT e = coEventsResult[i];
			if (dynamic_cast<CGoomba*>(e)) // if e->obj is Goomba 
			{
				CGoomba* goomba = dynamic_cast<CGoomba*>(e);
				if (goomba->GetState() != GOOMBA_STATE_DIE)
				{
					goomba->SetState(GOOMBA_STATE_DIE);
				}
			}
			else if (dynamic_cast<CKoopas*>(e))
			{
				CKoopas* Koopas = dynamic_cast<CKoopas*>(e);
				Koopas->TailAttack = true;
				if (Koopas->GetState() == KOOPAS_STATE_SHELL)
				{
					Koopas->vy = -KOOPAS_DIE_DEFLECT_SPEED * dt;
				}
				else
				{
					Koopas->SetState(KOOPAS_STATE_SHELL);
					Koopas->vy = -KOOPAS_DIE_DEFLECT_SPEED * dt;
				}

			}
		}
		IsKilling = false;
	}
}
void Tail::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - TAIL_BBOX_WIDTH/2;
	top = y + RACCOON_BBOX_HIGHT / 2;
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

