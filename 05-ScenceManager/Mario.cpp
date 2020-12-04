#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Portal.h"
#include "QuestionBrick.h"
#include "Tail.h"
#include "Line.h"
#include "Koopas.h"
#include "Ground.h"
#include "BulletPiranhaPlant.h"

CMario::CMario(float x, float y) : CGameObject()
{
	level = MARIO_LEVEL_RACCOON;
	untouchable = 0;
	SetState(MARIO_STATE_IDLE);

	start_x = x; 
	start_y = y; 
	this->x = x; 
	this->y = y; 
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (x < CGame::GetInstance()->GetCamPosX())
		x = CGame::GetInstance()->GetCamPosX();
	if(ani != -1)
		TailofRaccoon->Attack(this->x, this->y, Iskilling, animation_set->at(ani)->GetcurrentFrame());
	if (!Iskilling && SkillOn)
	{
		PrepareCatch = true;
	}
	firebullet_1->Update(dt, coObjects);
	firebullet_2->Update(dt, coObjects);
	TailofRaccoon->Update(dt, coObjects);
	if (y < 0.0f && IsFlying && level == MARIO_LEVEL_RACCOON)
	{
		y = 0.0f;
 		IsLimitFlying = true;
	}
	CGameObject::Update(dt);
	// Simple fall down
	if (vx >= MARIO_MAX_SPEED_RUNNING)
	{
		vx = MARIO_MAX_SPEED_RUNNING;
		IsLimitRunning = true;
	}
	else if (vx <= -MARIO_MAX_SPEED_RUNNING)
	{
		vx = -MARIO_MAX_SPEED_RUNNING;
		IsLimitRunning = true;
	}
	else IsLimitRunning = false;
	if (IsFlying && Fly)
	{
		if (!IsLimitFlying)
		{
			vy = -MARIO_FLY_SPEED_Y * dt;
		}
		else if (nx == 1)
		{
			IsLimitRunning = false;
			IsFlying = false;
			vx = MARIO_WALKING_SPEED;
			vy = 0;
		}
		else if (nx == -1)
		{
			IsLimitRunning = false;
			IsFlying = false;
			vx = -MARIO_WALKING_SPEED;
			vy = 0;
		}
	}
	if (AllowJump)
	{
		vy = -MARIO_JUMP_SPEED_Y * dt;
	}
	else
	{
		if (IsSlowDropping && (y - YHolding < MARIO_DISTANCE_SLOW_DROP) && !OnPlatform)
		{
			vy += MARIO_FALL_SLOWLY_SPEED * dt;
		}
		else
		{
			vy += MARIO_GRAVITY * dt;
			IsSlowDropping = false;
		}	
	}
	if (YHolding - y > MARIO_DISTANCE_JUMP)
		AllowJump = false;
	if (IsCatching && Shell->GetState() != KOOPAS_STATE_WALKING)
	{
		Shell->BeCatch(this, this->y + MARIO_RACCOON_BBOX_HEIGHT / 4);
	}
	else if (IsCatching && Shell->GetState() == KOOPAS_STATE_WALKING)
	{
		if (level > MARIO_LEVEL_SMALL)
		{
			level = MARIO_LEVEL_SMALL;
			StartUntouchable();
		}
		else
			SetState(MARIO_STATE_DIE);
	}
	if(Shell != NULL)
		if (!IsCatching && Shell->GetState() == KOOPAS_STATE_SHELL && Shell->IsCatching)
		{
			Shell->SetState(KOOPAS_STATE_ROTATORY);
			Shell->vx = KOOPAS_ROTATORY_SPEED * nx;
			Shell->IsCatching = false;
		}
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	// turn off collision when die 
	if (state!=MARIO_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);
	// reset untouchable timer if untouchable time has passed
	if (KickShell && GetTickCount() - Kick_start > animation_set->at(ani)->GettotalFrameTime())
		KickShell = false;
	//DebugOut(L"animation_set->at(ani)%d\n", animation_set->at(MARIO_ANI_RACCOON_KILL_RIGHT)->GetcurrentFrame());
	if (Kill && GetTickCount() - Kill_start > animation_set->at(ani)->GettotalFrameTime())
	{
		Kill_start = 0;
		Kill = 0;
		Iskilling = false;
		TailofRaccoon->Attack(this->x, this->y, Iskilling, animation_set->at(ani)->GetcurrentFrame());
		animation_set->at(MARIO_ANI_RACCOON_KILL_RIGHT)->ResetcurrentFrame();		// loi currentFrame co luc k phai la -1
		animation_set->at(MARIO_ANI_RACCOON_KILL_LEFT)->ResetcurrentFrame();
	}
	if (FireAttack && GetTickCount() - FireAttack_start > animation_set->at(ani)->GettotalFrameTime())
	{
		FireAttack_start = 0;
		FireAttack = 0;
	}
	if (GetTickCount() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}
	if (GetTickCount() - Fly_start > MARIO_FLY_TIME)	// con thoi gian thi dc bay, nhan 1 cai tinh thoi gian lai
	{
		Fly_start = 0;
		Fly = 0;
		IsFlying = false;
	}
	if (GetTickCount() - Slip_start > MARIO_SLIP_TIME)
	{
		Slip_start = 0;
		Slip = 0;
	}
	//if (GetTickCount() - Slip_start > MARIO_SLIP_TIME)
	//{
	//	Slip_start = 0;
	//	Slip = 0;
	//}
	//ny = 0;
	// No collision occured, proceed normally
	//DebugOut(L"Vy%f\n", vy);
	if (coEvents.size()==0)
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

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 
		// block every object first!
		x += min_tx*dx + nx*0.3f;
		y += min_ty*dy + ny*0.3f;

		if (nx!=0 && !OnPlatform)
			vx = 0;
		if (ny!=0)
			vy = 0;
		// check mario on a platform, because Mario is pushed, so need to check IsJunping
		if (ny == -1) 
		{
			if(!AllowJump)
				OnPlatform = true;
			IsLimitFlying = false;
		}
		else
			OnPlatform = false;

		if (ny == 1)
			AllowJump = false;

		for (UINT i = 0; i < coEventsResult.size(); i++) 
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			
			if (dynamic_cast<CGoomba *>(e->obj)) // if e->obj is Goomba 
			{
				CGoomba *goomba = dynamic_cast<CGoomba *>(e->obj);
				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					if (goomba->GetState()!= GOOMBA_STATE_DIE)
					{
						goomba->SetState(GOOMBA_STATE_DIE);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
				}
				else if (e->nx != 0 && Iskilling)
				{
					goomba->SetState(GOOMBA_STATE_DIE);
				}
				else if (e->nx != 0 && !Iskilling)
				{
					if (untouchable == 0)
					{
						if (goomba->GetState() != GOOMBA_STATE_DIE)
						{
							if (level > MARIO_LEVEL_SMALL)
							{
								level = MARIO_LEVEL_SMALL;
								StartUntouchable();
							}
							else
								SetState(MARIO_STATE_DIE);	
						}
					}
				}
			}
			else if (dynamic_cast<Line*>(e->obj))
			{
				if (e->ny > 0)		// o duoi len
				{
					AllowJump = true;
					vy = vyLine;
					y += dy;
				}

			}
			else if (dynamic_cast<CKoopas*>(e->obj))
			{
				Shell = dynamic_cast<CKoopas*>(e->obj);
				if (Shell->GetState() == KOOPAS_STATE_SHELL && !PrepareCatch && e->nx != 0)
				{
					Shell->SetState(KOOPAS_STATE_ROTATORY);
					if(this->nx == 1)
						Shell->vx = KOOPAS_ROTATORY_SPEED;
					if(this->nx == -1)
						Shell->vx = -KOOPAS_ROTATORY_SPEED;
					KickShell = true;
					Kick_start = GetTickCount();
				}
				else if (e->nx != 0 && Iskilling)
				{
					Shell->SetState(KOOPAS_STATE_SHELL);
					Shell->vy -= KOOPAS_DIE_DEFLECT_SPEED;
				}
				else if (e->nx != 0 && Shell->GetState() == KOOPAS_STATE_SHELL)
				{
					if (PrepareCatch)
					{
						IsCatching = true;
					}
				}
				else if (e->ny < 0)
				{
					if (Shell->GetState() == KOOPAS_STATE_SHELL)
					{
						Shell->SetState(KOOPAS_STATE_ROTATORY);
						if (this->x <= Shell->x)
							Shell->vx = KOOPAS_ROTATORY_SPEED;
						if (this->x > Shell->x)
							Shell->vx = -KOOPAS_ROTATORY_SPEED;
					}
					else if (Shell->GetState() == KOOPAS_STATE_ROTATORY)
					{
						Shell->SetState(KOOPAS_STATE_SHELL);
						this->vy = -MARIO_JUMP_DEFLECT_SPEED - 0.2f;
					}
					else
					{
						Shell->SetState(KOOPAS_STATE_SHELL);
						this->vy = -MARIO_JUMP_DEFLECT_SPEED - 0.2f;
					}	
				}
				else
					StartUntouchable();
			}
			else if (dynamic_cast<QuestionBrick*>(e->obj))
			{
				QuestionBrick* brick = dynamic_cast<QuestionBrick*>(e->obj);
				if (brick->GetState() == BRICK_STATE_QUESTION_ON && e->ny > 0)
					brick->SetState(BRICK_STATE_QUESTION_OFF);
			}
			else if (dynamic_cast<BulletPiranhaPlant*>(e->obj))
			{
				BulletPiranhaPlant* Bullet = dynamic_cast<BulletPiranhaPlant*>(e->obj);
				if (untouchable == 0)
				{
					if (level > MARIO_LEVEL_SMALL)
					{
						level = MARIO_LEVEL_SMALL;
						StartUntouchable();
					}
					else
						SetState(MARIO_STATE_DIE);
				}
			}
			else if (dynamic_cast<CPortal *>(e->obj))
			{
				CPortal *p = dynamic_cast<CPortal *>(e->obj);
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CMario::Render()
{
	firebullet_1->Render();
	firebullet_2->Render();
	TailofRaccoon->Render();
	int alpha = 255;
	if (untouchable) alpha = 128;

	if (state == MARIO_STATE_DIE)
		ani = MARIO_ANI_DIE;
	else
	if (level == MARIO_LEVEL_BIG)
	{
		if (vx == 0)
		{
			if (nx > 0)
			{
				ani = MARIO_ANI_BIG_IDLE_RIGHT;
				if (AllowJump)
					ani = MARIO_ANI_BIG_JUMP_RIGHT;
				if (IsBendingOver)
					ani = MARIO_ANI_BIG_BEND_OVER_RIGHT;
			}

			else {
				ani = MARIO_ANI_BIG_IDLE_LEFT;
				if (AllowJump)
					ani = MARIO_ANI_BIG_JUMP_LEFT;
				if (IsBendingOver)
					ani = MARIO_ANI_BIG_BEND_OVER_LEFT;
			}
			
		}
		else if (vx > 0) {
			ani = MARIO_ANI_BIG_WALKING_RIGHT;
			if (vx < 0.1 && nx == -1)
				ani = MARIO_ANI_SLIP_RIGHT;
			if (AllowJump)
				ani = MARIO_ANI_BIG_JUMP_RIGHT;
			if (IsBendingOver)
				ani = MARIO_ANI_BIG_BEND_OVER_RIGHT;
			if (IsLimitRunning)
			{
				ani = MARIO_ANI_BIG_MAX_SPEED_RUNNING_RIGHT;
				if (AllowJump)
					ani = MARIO_ANI_BIG_MAX_SPEED_JUMP_RIGHT;
			}

		}
		else {
			ani = MARIO_ANI_BIG_WALKING_LEFT;
			if (vx > -0.1 && nx == 1)
				ani = MARIO_ANI_SLIP_LEFT;
			if (AllowJump)
				ani = MARIO_ANI_BIG_JUMP_LEFT;
			if (IsBendingOver)
				ani = MARIO_ANI_BIG_BEND_OVER_LEFT;
			if (IsLimitRunning)
			{
				ani = MARIO_ANI_BIG_MAX_SPEED_RUNNING_LEFT;
				if (AllowJump)
					ani = MARIO_ANI_BIG_MAX_SPEED_JUMP_LEFT;
			}
		}
	}
	else if (level == MARIO_LEVEL_SMALL)
	{
		if (vx == 0)
		{
			if (nx>0) ani = MARIO_ANI_SMALL_IDLE_RIGHT;
			else ani = MARIO_ANI_SMALL_IDLE_LEFT;
		}
		else if (vx > 0)
			ani = MARIO_ANI_SMALL_WALKING_RIGHT;
		else ani = MARIO_ANI_SMALL_WALKING_LEFT;
	}
	else if (level == MARIO_LEVEL_RACCOON)
	{
		if (vx == 0)
		{
			if (nx > 0)
			{
				ani = MARIO_ANI_RACCOON_IDLE_RIGHT;
				if (AllowJump)
					ani = MARIO_ANI_RACCOON_JUMP_RIGHT;
				if(!AllowJump && !OnPlatform)
					ani = MARIO_ANI_RACCOON_DROP_RIGHT;
				if (Iskilling && Kill) 
				{
					ani = MARIO_ANI_RACCOON_KILL_RIGHT;
				}
				if (KickShell)
					ani = MARIO_ANI_RACCOON_KICK_RIGHT;
				if (IsCatching)
					ani = MARIO_ANI_RACCOON_CATCHING_IDLE_RIGHT;
				if (IsBendingOver)
					ani = MARIO_ANI_RACCOON_BEND_OVER_RIGHT;
			}
			else 
			{
				ani = MARIO_ANI_RACCOON_IDLE_LEFT;
				if (AllowJump)
					ani = MARIO_ANI_RACCOON_JUMP_LEFT;
				if (!AllowJump && !OnPlatform)
					ani = MARIO_ANI_RACCOON_DROP_LEFT;
				if (Iskilling && Kill) {
					ani = MARIO_ANI_RACCOON_KILL_LEFT;
				}
				if (KickShell)
					ani = MARIO_ANI_RACCOON_KICK_LEFT;
				if (IsCatching)
					ani = MARIO_ANI_RACCOON_CATCHING_IDLE_LEFT;
				if (IsBendingOver)
					ani = MARIO_ANI_RACCOON_BEND_OVER_LEFT;
			}
		}
		else if (vx > 0)
		{
			ani = MARIO_ANI_RACCOON_WALKING_RIGHT;
			if (AllowJump)
				ani = MARIO_ANI_RACCOON_JUMP_RIGHT;
			if (!AllowJump && !OnPlatform)
				ani = MARIO_ANI_RACCOON_DROP_RIGHT;
			if (Iskilling && Kill) {
				ani = MARIO_ANI_RACCOON_KILL_RIGHT;
			}
			if (vx < 0.1 && nx == -1)
				ani = MARIO_ANI_RACCOON_SLIP_RIGHT;
			if (KickShell)
				ani = MARIO_ANI_RACCOON_KICK_RIGHT;
			if (IsCatching)
				ani = MARIO_ANI_RACCOON_CATCHING_WALKING_RIGHT;
			if (IsBendingOver)
				ani = MARIO_ANI_RACCOON_BEND_OVER_RIGHT;
		}
		else 
		{
			ani = MARIO_ANI_RACCOON_WALKING_LEFT;
			if (AllowJump)
				ani = MARIO_ANI_RACCOON_JUMP_LEFT;
			if (!AllowJump && !OnPlatform)
				ani = MARIO_ANI_RACCOON_DROP_LEFT;
			if (Iskilling && Kill) {
				ani = MARIO_ANI_RACCOON_KILL_LEFT;
			}
			if (vx > -0.1 && nx == 1)
				ani = MARIO_ANI_RACCOON_SLIP_LEFT;
			if (KickShell)
				ani = MARIO_ANI_RACCOON_KICK_LEFT;
			if (IsCatching)
				ani = MARIO_ANI_RACCOON_CATCHING_WALKING_LEFT;
			if (IsBendingOver)
				ani = MARIO_ANI_RACCOON_BEND_OVER_LEFT;
		}
		if (IsLimitRunning && !IsCatching)
			ani = MARIO_ANI_RACCOON_PREPARE_FLY_RIGHT;
		if (IsFlying)
			ani = MARIO_ANI_RACCOON_FLY_RIGHT;
		if (IsSlowDropping)
			if (nx == -1)
				ani = MARIO_ANI_RACCOON_DROP_SLOW_LEFT;
			else
				ani = MARIO_ANI_RACCOON_DROP_SLOW_RIGHT;
		
	}
	else if (level == MARIO_LEVEL_FIRE)
	{
		if (vx == 0)
		{
			if (nx > 0) 
			{
				ani = MARIO_ANI_FIRE_IDLE_RIGHT;
				if (AllowJump)
					ani = MARIO_ANI_FIRE_JUMP_RIGHT;
				if (FireAttack)
					ani = MARIO_ANI_FIRE_ATTACK_RIGHT;
				if (IsBendingOver)
					ani = MARIO_ANI_FIRE_BEND_OVER_RIGHT;
			}
			else {
				ani = MARIO_ANI_FIRE_IDLE_LEFT;
				if (AllowJump)
					ani = MARIO_ANI_FIRE_JUMP_LEFT;
				if (FireAttack)
					ani = MARIO_ANI_FIRE_ATTACK_LEFT;
				if (IsBendingOver)
					ani = MARIO_ANI_FIRE_BEND_OVER_LEFT;
			}
		}
		else if (vx > 0)
		{
			ani = MARIO_ANI_FIRE_WALK_RIGHT;
			if (AllowJump)
				ani = MARIO_ANI_FIRE_JUMP_RIGHT;
			if(vx < 0.1 && nx == -1)
				ani = MARIO_ANI_FIRE_SLIP_RIGHT;
			if (IsLimitRunning)
			{
				ani = MARIO_ANI_FIRE_MAX_SPEED_RUNNING_RIGHT;
				if (AllowJump)
					ani = MARIO_ANI_FIRE_MAX_SPEED_JUMP_RIGHT;
			}
			if (FireAttack)
				ani = MARIO_ANI_FIRE_ATTACK_RIGHT;
			if (IsBendingOver)
				ani = MARIO_ANI_FIRE_BEND_OVER_RIGHT;
		}
		else 
		{
			ani = MARIO_ANI_FIRE_WALK_LEFT;
			if (AllowJump)
				ani = MARIO_ANI_FIRE_JUMP_LEFT;
			if (vx > -0.1 && nx == 1)
				ani = MARIO_ANI_FIRE_SLIP_LEFT;
			if (IsLimitRunning)
			{
				ani = MARIO_ANI_FIRE_MAX_SPEED_RUNNING_LEFT;
				if (AllowJump)
					ani = MARIO_ANI_FIRE_MAX_SPEED_JUMP_LEFT;
			}
			if (FireAttack)
				ani = MARIO_ANI_FIRE_ATTACK_LEFT;
			if (IsBendingOver)
				ani = MARIO_ANI_FIRE_BEND_OVER_LEFT;
		}
	}

	if (level == MARIO_LEVEL_RACCOON && nx == 1)			// tru` di vi tri cai duoi
	{
		if (vx < 0)
			animation_set->at(ani)->Render(x, y, alpha);
		else
			animation_set->at(ani)->Render(x - MARIO_RACCOON_BBOX_TAIL, y, alpha);
	}
	else if (level == MARIO_LEVEL_RACCOON && nx == -1)
	{
		if (vx > 0 || Kill)
			animation_set->at(ani)->Render(x - MARIO_RACCOON_BBOX_TAIL, y, alpha);
		else
			animation_set->at(ani)->Render(x, y, alpha);
	}
	else
		animation_set->at(ani)->Render(x, y, alpha);
	//RenderBoundingBox();
}

void CMario::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case MARIO_STATE_KILL:

		if (!Kill && level == MARIO_LEVEL_RACCOON)
		{
			SkillOn = true;
			Iskilling = true;
			StartKill();
			PrepareCatch = false;
		}
		if (level == MARIO_LEVEL_FIRE)
		{
			if (!firebullet_2->IsBeingFired && firebullet_1->IsBeingFired)		// vien 1 dc ban chua, r thi ban vien 2
			{
				if (nx == 1)
					firebullet_2->attack(this->x, this->y, true);		//right == true
				else
					firebullet_2->attack(this->x, this->y, false);
				StartFireAttack();
			}
			if (!firebullet_1->IsBeingFired)
			{
				if (nx == 1)
					firebullet_1->attack(this->x, this->y, true);		//right == true
				else
					firebullet_1->attack(this->x, this->y, false);
				StartFireAttack();
			}
		}
	case MARIO_STATE_FAST_RUN:
			IsRunning = true;
		break;
	case MARIO_STATE_WALKING_RIGHT:
		if (!HaveInertia)
			XHolding = x;
		HaveInertia = true;
		if (vx < 0.1 && vx != 0)
		{
			vx += MARIO_VY_ASCENDING_SLIP;
			if (vx > 0.1)
				vx = 0.1;
		}
		if (vx == 0 && !IsRunning)
			vx = MARIO_WALKING_SPEED;
		if (IsRunning && vx < MARIO_MAX_SPEED_RUNNING && !IsFlying && !IsLimitRunning)
			vx += MARIO_VY_ASCENDING_RUN;
			nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT: 
		if (!HaveInertia)
			XHolding = x;
		HaveInertia = true;
		if (vx > -0.1 && vx != 0)
		{
			vx -= MARIO_VY_ASCENDING_SLIP;
			if (vx < -0.1)
				vx = -0.1;
		}
		if(vx == 0 && !IsRunning)
			vx = -MARIO_WALKING_SPEED;
		if (IsRunning && vx != -MARIO_MAX_SPEED_RUNNING && !IsFlying && !IsLimitRunning)
			vx -= MARIO_VY_ASCENDING_RUN;
			nx = -1;
		break;
	case MARIO_STATE_JUMP:
		if (OnPlatform && !IsJumping)						// check mario on a platform
		{
			AllowJump = true;
			YHolding = y;
			if(level == MARIO_LEVEL_RACCOON)
				IsSlowDropping = false;
		}
		if (!AllowJump)
		{
			if (level == MARIO_LEVEL_RACCOON)
   				IsSlowDropping = true;
			YHolding = y;
		}
		if (level == MARIO_LEVEL_RACCOON)
			if (IsLimitRunning)
			{
				IsFlying = true;
				Startfly();
			}
		IsJumping = true;
		OnPlatform = false;
		break; 
	case MARIO_STATE_DROP:
		AllowJump = false;
		IsJumping = false;
		break;
	case MARIO_STATE_SKILL_OFF:
		SkillOn = false;
		PrepareCatch = false;
		IsCatching = false;
		IsRunning = false;
		break;
	case MARIO_STATE_STAND:
		if (IsBendingOver)
		{
			if (level == MARIO_LEVEL_BIG || level == MARIO_LEVEL_FIRE)
				this->y -= MARIO_BIG_STAND_Y;
			if (level == MARIO_LEVEL_RACCOON)
				this->y -= MARIO_RACCOON_STAND_Y;
		}
		IsBendingOver = false;	
		break;
	case MARIO_STATE_BEND_OVER:
		if (!IsBendingOver)
		{
				this->y += MARIO_Y_BEND_OVER;
		}
		IsBendingOver = true;
	case MARIO_STATE_IDLE: 
		IsLimitRunning = false;
		if (x - XHolding > MARIO_DISTANCE_INERTIA) // khoan cach bi vang
		{
			if (vx > 0.0f)
			{
				StartSlip();
				vx -= MARIO_INERTIA_DECREASE;
				if (vx < 0)
				{
					vx = 0;
					HaveInertia = false;
				}
					
				break;							//thoat khoi case khong cho nhay? xuong if else duoi(vx =0)
			}
		}
		if (XHolding - x > MARIO_DISTANCE_INERTIA)
		{
			if (vx < 0.0f)
			{
				vx += MARIO_INERTIA_DECREASE;
				if (vx > 0)
				{
					vx = 0;
					HaveInertia = false;
				}
					
				break;
			}
			else
			{
				vx = 0;
				HaveInertia = false;
				break;
			}
		}
		else
		{
			vx = 0;
			HaveInertia = false;
			break;
		}

	
	case MARIO_STATE_DIE:
		vy = -MARIO_DIE_DEFLECT_SPEED;
		break;
	}
}

void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y; 
	switch (level)
	{
	case MARIO_LEVEL_FIRE:
	case MARIO_LEVEL_BIG:
		right = x + MARIO_BIG_BBOX_WIDTH;
		bottom = y + MARIO_BIG_BBOX_HEIGHT;
		if (IsBendingOver)
		{
			right = x + MARIO_BIG_BBOX_BEND_OVER_WIDTH;
			bottom = y + MARIO_BIG_BBOX_BEND_OVER_HEIGHT;
		}
		break;
	case MARIO_LEVEL_RACCOON:
		if (vx > 0)
		{
			right = x  + MARIO_RACCOON_BBOX_WIDTH - MARIO_RACCOON_BBOX_TAIL;
		}
		else
		{
			right = x + MARIO_RACCOON_BBOX_WIDTH - MARIO_RACCOON_BBOX_TAIL;
		}
		bottom = y + MARIO_RACCOON_BBOX_HEIGHT;
		if (IsBendingOver)
		{
			right = x + MARIO_RACCOON_BBOX_BEND_OVER_WIDTH;
			bottom = y + MARIO_RACCOON_BBOX_BEND_OVER_HEIGHT;
		}
		break;
	case MARIO_LEVEL_SMALL:
		right = x + MARIO_SMALL_BBOX_WIDTH;
		bottom = y + MARIO_SMALL_BBOX_HEIGHT;
		break;
	}

}
/*
	Reset Mario status to the beginning state of a scene
*/
void CMario::Reset()
{
	SetState(MARIO_STATE_IDLE);
	SetLevel(MARIO_LEVEL_RACCOON);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}

