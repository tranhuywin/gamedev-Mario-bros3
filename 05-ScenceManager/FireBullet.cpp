#include "FireBullet.h"
#include <algorithm>
#include <assert.h>
#include "Utils.h"
#include "Game.h"
#include "Line.h"
#include "Goomba.h"
#include "Koopas.h"
#include "Ground.h"
#include "Tube.h"

FireBullet::FireBullet()
{
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(4));
}
void FireBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!IsBeingFired)
	{
		x = -OUTSIDE_MAP; y = OUTSIDE_MAP;
		vx = 0; vy = 0;
	}
	if (x < CGame::GetInstance()->GetCamPosX())
		IsBeingFired = false;
	if (x > CGame::GetInstance()->GetCamPosX() + CGame::GetInstance()->GetScreenWidth())
 		IsBeingFired = false;
	CGameObject::Update(dt);
	if ( PosPlatform - y > FIRE_BULLET_DISTANCE_PLATFORM)
	{
		vy = FIRE_BULLET_SPEED_Y;
	}
	if (GetTickCount() - Explode_start > FIRE_BULLET_EXPLODE_TIME)
	{
		Explode_start = 0;
		Explode = 0;
	}
	else IsBeingFired = false;
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
		if (ny == -1)
		{
			OnPlatform = true;
			PosPlatform = y;
		}
		else
			OnPlatform = false;
		vy = -FIRE_BULLET_SPEED_Y;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CGoomba*>(e->obj)) // if e->obj is Goomba 
			{
				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
				if(goomba->TypeGoomba == GOOMBA_NORMAL)
					goomba->SetState(GOOMBA_STATE_DIE);
				else if (goomba->TypeGoomba == PARA_GOOMBA)
				{
					if (goomba->LevelParaGoomba == 1)
					{
						goomba->SetState(GOOMBA_STATE_WALKING);
						goomba->LevelParaGoomba--;
					}
				}
				StartExplode();
				XExplode = x;
				YExplode = y;
			}
			else if (dynamic_cast<Line*>(e->obj))
			{
				if (e->ny > 0)		// o duoi len
				{
					vy = vyLine;
					y += dy;
				}
				if (e->nx != 0)
				{
					StartExplode();
					XExplode = x;
					YExplode = y;
				}

			}
			else if (dynamic_cast<Ground*>(e->obj))
			{
				if (e->nx != 0)
				{
					StartExplode();
					XExplode = x;
					YExplode = y;
				}
			}
			else if (dynamic_cast<Tube*>(e->obj))
			{
				if (e->nx != 0)
				{
					StartExplode();
					XExplode = x;
					YExplode = y;
				}
			}
			else if (dynamic_cast<CKoopas*>(e->obj))
			{
				CKoopas* Koopas = dynamic_cast<CKoopas*>(e->obj);
				if(Koopas->GetState() == KOOPAS_STATE_SHELL && !Explode)
					Koopas->SetState(KOOPAS_STATE_DIE);
				else
				Koopas->SetState(KOOPAS_STATE_SHELL);
				StartExplode();
				XExplode = x;
				YExplode = y;
			}
		}

	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}
void FireBullet::Render()
{
	int ani = 0;
	int alpha = 255;
	if (Explode)
	{
		ani = FIRE_BULLET_ANI_EXPLODE;
		this->x = XExplode;
		this->y = YExplode;
	}
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
	this->IsBeingFired = true;
	this->x = x;
	this->y = y;
	if (isRight)
		vx = FIRE_BULLET_SPEED_X;
	else
		vx = -FIRE_BULLET_SPEED_X;
	vy = FIRE_BULLET_SPEED_Y;
}
