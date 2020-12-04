#include "BulletPiranhaPlant.h"
#include "Mario.h"
#include "Utils.h"

BulletPiranhaPlant::BulletPiranhaPlant()
{
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(4));
}

void BulletPiranhaPlant::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + 8;
	bottom = y + 8;
}

void BulletPiranhaPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
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
		float vyLine = vy;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CMario*>(e->obj))
			{
				CMario* mario = dynamic_cast<CMario*>(e->obj);
				if (mario->GetLevel() != MARIO_LEVEL_SMALL)
				{
					mario->SetLevel(MARIO_LEVEL_SMALL);
					mario->StartUntouchable();
				}
				else
					mario->SetState(MARIO_STATE_DIE);
			}
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void BulletPiranhaPlant::Render()
{
	animation_set->at(0)->Render(x, y, 255);
	RenderBoundingBox();
}

void BulletPiranhaPlant::Attack(float XStart, float YStart, float Xtarget, float Ytarget, bool IsRight, bool AttackIsAtive)
{
	// TODO: Fixing attack
	if (AttackIsAtive)
	{
		x = XStart;
		y = YStart;
		vx = 0.00001f * dt;
		vy = 0.00001f * dt;
	}
	else
	{
		x = -OUTSIDE_MAP;
		y = OUTSIDE_MAP;
	}
}

