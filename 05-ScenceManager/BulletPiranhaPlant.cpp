#include "BulletPiranhaPlant.h"
#include "Mario.h"
#include "Utils.h"
#include "FirePiranhaPlant.h"

BulletPiranhaPlant::BulletPiranhaPlant()
{
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(4));
	x = -1;
	y = -1;
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
	x += dx;
	y += dy;
	if ((this->x < CGame::GetInstance()->GetCamPosX() || this->x > CGame::GetInstance()->GetCamPosX() + CGame::GetInstance()->GetScreenWidth()) && !IsAttack)
	{
		AllowAttack = true;
	}
	if ((this->y < CGame::GetInstance()->GetCamPosY() || this->y > CGame::GetInstance()->GetCamPosY() + CGame::GetInstance()->GetScreenHeight()) && !IsAttack)
		AllowAttack = true;
 	CGameObject::Update(dt);
	/*vector<LPCOLLISIONEVENT> coEvents;
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
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];*/
}

void BulletPiranhaPlant::Render()
{
	animation_set->at(0)->Render(x, y, 255);
	RenderBoundingBox();
}

void BulletPiranhaPlant::Attack(int PosAttack, bool AttackIsAtive)
{
	if(AllowAttack)
		if (AttackIsAtive)
		{
			IsAttack = true;
			switch (PosAttack)
			{
			case MARIO_LEFT_TOP_TOP:
				vx = -0.005 * dt;
				vy = -0.005 * dt;
				this->x = 352.0f;
				this->y = 336.0f;
				break;
			case MARIO_LEFT_TOP_BOT:
				vx = -0.005 * dt;
				vy = -0.001 * dt;
				this->x = 352.0f;
				this->y = 336.0f;
				break;
			case MARIO_LEFT_BOT_TOP:
				vx = -0.005 * dt;
				vy = 0.001 * dt;
				this->x = 352.0f;
				this->y = 344.0f;
				break;
			case MARIO_LEFT_BOT_BOT:
				vx = -0.005 * dt;
				vy = 0.005 * dt;
				this->x = 352.0f;
				this->y = 344.0f;
				break;

			case MARIO_RIGHT_TOP_TOP:
				vx = 0.005 * dt;
				vy = -0.005 * dt;
				this->x = 376.0f;
				this->y = 336.0f;
				break;
			case MARIO_RIGHT_TOP_BOT:
				vx = 0.005 * dt;
				vy = -0.001 * dt;
				this->x = 376.0f;
				this->y = 336.0f;
				break;
			case MARIO_RIGHT_BOT_TOP:
				vx = 0.005 * dt;
				vy = 0.001 * dt;
				this->x = 376.0f;
				this->y = 344.0f;
				break;
			case MARIO_RIGHT_BOT_BOT:
				vx = 0.005 * dt;
				vy = 0.005 * dt;
				this->x = 376.0f;
				this->y = 344.0f;
				break;
			}
		}
		else
		{
			x = -1;
			y = -1;
		}
}


