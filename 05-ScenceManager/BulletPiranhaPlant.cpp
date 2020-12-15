#include "BulletPiranhaPlant.h"
#include "Mario.h"
#include "Utils.h"
#include "FirePiranhaPlant.h"

BulletPiranhaPlant::BulletPiranhaPlant(int ani)
{
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(ani));
	x = 0;
	y = 0;
	vx = 0; vy = 0;
}

void BulletPiranhaPlant::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BULLET_BBOX;
	bottom = y + BULLET_BBOX;
}

void BulletPiranhaPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	if ((this->x < CGame::GetInstance()->GetCamPosX() || this->x > CGame::GetInstance()->GetCamPosX() + CGame::GetInstance()->GetScreenWidth()))
	{
		AllowAttack = true;
		IsAttack = false;
	}
	if ((this->y < CGame::GetInstance()->GetCamPosY() || this->y > CGame::GetInstance()->GetCamPosY() + CGame::GetInstance()->GetScreenHeight()))
	{
		AllowAttack = true;
		IsAttack = false;
	}
 	CGameObject::Update(dt);
	x += dx;
	y += dy;
}

void BulletPiranhaPlant::Render()
{
	animation_set->at(0)->Render(x, y, 255);
	//RenderBoundingBox();
}

void BulletPiranhaPlant::Attack(int PosAttack, bool AttackIsAtive)
{
	if(AllowAttack && !IsAttack)
		if (AttackIsAtive)
		{
			IsAttack = true;
			switch (PosAttack)
			{
			case MARIO_LEFT_TOP_TOP:
				vx = -SPEED_VX_BULLET * dt;
				vy = -SPEED_VY_BULLET_TOP * dt;
				this->x = BULLET_XLEFT;
				this->y = BULLET_YTOP;
				break;
			case MARIO_LEFT_TOP_BOT:
				vx = -SPEED_VX_BULLET * dt;
				vy = -SPEED_VY_BULLET_BOT * dt;
				this->x = BULLET_XLEFT;
				this->y = BULLET_YTOP;
				break;
			case MARIO_LEFT_BOT_TOP:
				vx = -SPEED_VX_BULLET * dt;
				vy = SPEED_VY_BULLET_BOT * dt;
				this->x = BULLET_XLEFT;
				this->y = BULLET_YBOT;
				break;
			case MARIO_LEFT_BOT_BOT:
				vx = -SPEED_VX_BULLET * dt;
				vy = SPEED_VY_BULLET_TOP * dt;
				this->x = BULLET_XLEFT;
				this->y = BULLET_YBOT;
				break;

			case MARIO_RIGHT_TOP_TOP:
				vx = SPEED_VX_BULLET * dt;
				vy = -SPEED_VY_BULLET_TOP * dt;
				this->x = BULLET_XRIGHT;
				this->y = BULLET_YTOP;
				break;
			case MARIO_RIGHT_TOP_BOT:
				vx = SPEED_VX_BULLET * dt;
				vy = -SPEED_VY_BULLET_BOT * dt;
				this->x = BULLET_XRIGHT;
				this->y = BULLET_YTOP;
				break;
			case MARIO_RIGHT_BOT_TOP:
				vx = SPEED_VX_BULLET * dt;
				vy = SPEED_VY_BULLET_BOT * dt;
				this->x = BULLET_XRIGHT;
				this->y = BULLET_YBOT;
				break;
			case MARIO_RIGHT_BOT_BOT:
				vx = SPEED_VX_BULLET * dt;
				vy = SPEED_VY_BULLET_TOP * dt;
				this->x = BULLET_XRIGHT;
				this->y = BULLET_YBOT;
				break;
			}
		}
		else
		{
			x = -1;
			y = -1;
		}
}


