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
#include "VenusFireTrap.h"
#include "Items.h"
#include "Tube.h"
#include "Brick.h"
#include "FlyingWood.h"
#include "BoomerangOfBrother.h"
#include "Brothers.h"
#include "MusicNote.h"

CMario::CMario(float x, float y) : CGameObject()
{
	ani = -1;
	LastAni = -1;
	NoCardStartGame = CGame::GetInstance()->GetCard_1();
	if (CGame::GetInstance()->GetCard_1() != -1) {
		NoCardStartGame = 1;
	}
	else if (CGame::GetInstance()->GetCard_2() != -1)
		NoCardStartGame = 2;
	else if (CGame::GetInstance()->GetCard_3() != -1)
		NoCardStartGame = 3;
	else
		NoCardStartGame = 0;

	if (CGame::GetInstance()->Getcurrent_scene() == SCENCE_WORD_MAP_1_1)
		level = MARIO_LEVEL_RACCOON;
	else if (CGame::GetInstance()->Getcurrent_scene() == SCENCE_WORD_MAP_4_1) {
		level = MARIO_LEVEL_RACCOON;
	}
	else
	{
		level = MARIO_LEVEL_SMALL;
		if (CGame::GetInstance()->GetReturnWorld()) {
			this->x = X_RETURN_WORLD_1;
			this->y = Y_RETURN_WORLD_1;
			level = MARIO_LEVEL_RACCOON;
		}
	}
	untouchable = 0;
	SetState(MARIO_STATE_IDLE);
	start_x = x; 
	start_y = y; 
	this->x = x; 
	this->y = y; 
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if(CGame::GetInstance()->GetLevel() != -1)
		CGame::GetInstance()->SetLevel(level);
	if (CGame::GetInstance()->GetReturnWorld() && (CGame::GetInstance()->Getcurrent_scene() == SCENCE_WORD_MAP_1 || CGame::GetInstance()->Getcurrent_scene() == SCENCE_WORD_MAP_4)) {
		this->x = X_RETURN_WORLD_1;
		this->y = Y_RETURN_WORLD_1;
		CGame::GetInstance()->SetReturnWorld(false);
	}
	if (NoCardStartGame == 0 && CGame::GetInstance()->GetCard_1() != -1)
		SetState(MARIO_STATE_WALKING_RIGHT);
	if (NoCardStartGame == 1 && CGame::GetInstance()->GetCard_2() != -1)
		SetState(MARIO_STATE_WALKING_RIGHT);
	if (NoCardStartGame == 2 && CGame::GetInstance()->GetCard_3() != -1)
		SetState(MARIO_STATE_WALKING_RIGHT);
	
	CGameObject::Update(dt);
	if (level != MARIO_LEVEL_MINI)
	{
	if (ani != -1)
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

	// Simple fall down
	if (vx >= (MARIO_MAX_SPEED_RUNNING * dt))
	{
		vx = MARIO_MAX_SPEED_RUNNING * dt;
		IsLimitRunning = true;
	}
	else if (vx <= -(MARIO_MAX_SPEED_RUNNING * dt))
	{
		vx = -(MARIO_MAX_SPEED_RUNNING * dt);
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
			vx = MARIO_WALKING_SPEED * dt;
			vy = 0;
		}
		else if (nx == -1)
		{
			IsLimitRunning = false;
			IsFlying = false;
			vx = -MARIO_WALKING_SPEED * dt;
			vy = 0;
		}
	}
	// state falling
	if (AllowJump)
	{
		vy = -MARIO_JUMP_SPEED_Y * dt;
	}
	else
	{
		if (IsSlowDropping && (y - YHolding < MARIO_DISTANCE_SLOW_DROP) && !OnPlatform)
		{
			vy = MARIO_FALL_SLOWLY_SPEED * dt;
		}
		else
		{
			if (!StartTeleport && state != MARIO_STATE_JUMP)
				vy += MARIO_GRAVITY * dt;
			else
			{
				vy += MARIO_GRAVITY_TELEPORT * dt;
			}

			IsSlowDropping = false;
		}
	}
	if (YHolding - y > MARIO_DISTANCE_JUMP)
	{
		AllowJump = false;
		IsDropping = true;
	}
	if (IsDropping && vy >= (MARIO_DROP_VY_MAX * dt))
		vy = MARIO_DROP_VY_MAX * dt;

	if (isDeflect) {
		if (vx == 0)
		{
			if (nx > 0) {
				vx = MARIO_DEFLECT_VX * dt;
			}
			else
				vx = -MARIO_DEFLECT_VX * dt;
		}
		if (isDeflectLeft)
		{
			vx = -MARIO_DEFLECT_LEFT_RIGHT * dt;
		}
		else if (isDeflectRight)
			vx = MARIO_DEFLECT_LEFT_RIGHT * dt;
		else if (isDeflectByPinkNote)
		{
			vy = -MARIO_DEFLECT_PINK_NOTE * dt;
			vx = 0;
		}
	}
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
	if (Shell != NULL)
		if (!IsCatching && Shell->GetState() == KOOPAS_STATE_SHELL && Shell->IsCatching)
		{
			Shell->SetState(KOOPAS_STATE_ROTATORY);
			Shell->vx = KOOPAS_ROTATORY_SPEED * nx;
			Shell->IsCatching = false;
		}
	if (KickShell && GetTickCount64() - Kick_start > animation_set->at(ani)->GettotalFrameTime())
		KickShell = false;
	if (Kill && GetTickCount64() - Kill_start > animation_set->at(ani)->GettotalFrameTime())
	{
		Kill_start = 0;
		Kill = 0;
		Iskilling = false;
		TailofRaccoon->Attack(this->x, this->y, Iskilling, animation_set->at(ani)->GetcurrentFrame());
		animation_set->at(MARIO_ANI_RACCOON_KILL_RIGHT)->ResetcurrentFrame();		// loi currentFrame co luc k phai la -1
		animation_set->at(MARIO_ANI_RACCOON_KILL_LEFT)->ResetcurrentFrame();
	}
	if (FireAttack && GetTickCount64() - FireAttack_start > animation_set->at(ani)->GettotalFrameTime())
	{
		FireAttack_start = 0;
		FireAttack = 0;
	}
	if (GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}
	if (GetTickCount64() - Fly_start > MARIO_FLY_TIME)	// con thoi gian thi dc bay, nhan 1 cai tinh thoi gian lai
	{
		Fly_start = 0;
		Fly = 0;
		IsFlying = false;
	}
	if (GetTickCount64() - Slip_start > MARIO_SLIP_TIME)
	{
		Slip_start = 0;
		Slip = 0;
	}
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	if (state != MARIO_STATE_DIE && !StartTeleport)
		CalcPotentialCollisions(coObjects, coEvents);
	if (coEvents.size()==0)
	{
		x += dx;
		y += dy;
		mdy += dy;
		if (mdy > 2.0f) {
			AllowJump = false;
			OnPlatform = false;
		}
	}
	else
	{
		mdy = 0;
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;
		float vyLine = vy;
		float vxPre = vx;
		float vyPre = vy;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 
		// block every object first!
		x += min_tx*dx + nx*0.4f;
		y += min_ty*dy + ny*0.4f;

		if (nx!=0 && !OnPlatform)
			vx = 0;
		if (ny != 0)
			vy = 0;
		// check mario on a platform, because Mario is pushed, so need to check IsJunping
		if (ny == -1) 
		{
			if (!AllowJump)
			{
				OnPlatform = true;
				IsDropping = false;
			}
			IsLimitFlying = false;
		}
		else
			OnPlatform = false;

		if (ny == 1)
			AllowJump = false;

		//turn off isDeflect
		if (isDeflect && ny < 0) {
			isDeflect = false;
			isDeflectLeft = false; isDeflectRight = false;
			SetState(MARIO_STATE_DROP);
		}
		
		for (UINT i = 0; i < coEventsResult.size(); i++) 
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			
			if (dynamic_cast<CGoomba*>(e->obj)) // if e->obj is Goomba 
			{
				CGoomba *goomba = dynamic_cast<CGoomba *>(e->obj);
				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					if (goomba->GetState() != GOOMBA_STATE_DIE)
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					if (goomba->TypeGoomba == GOOMBA_NORMAL)
						goomba->SetState(GOOMBA_STATE_DIE);
					else if (goomba->TypeGoomba == PARA_GOOMBA)
					{
						if (goomba->LevelParaGoomba == 0)
						{
							goomba->SetState(GOOMBA_STATE_DIE);
						}
						if (goomba->LevelParaGoomba == 1)
						{
							goomba->SetState(GOOMBA_STATE_WALKING);
							goomba->LevelParaGoomba--;
						}
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
							if (level > MARIO_LEVEL_BIG)
							{
								SetLevel(MARIO_LEVEL_BIG);
								this->x -= MARIO_RACCOON_BBOX_TAIL;
								this->y -= 1;		// khong bi rot xuong Coobj
								StartUntouchable();
							}
							else if (level > MARIO_LEVEL_SMALL)
							{
								level = MARIO_LEVEL_SMALL;
								StartUntouchable();
							}
							else
								SetState(MARIO_STATE_DIE);	
						}
						else {
							//vy = vyPre;
							//y += dy;
						}
					}
					else {
						vx = vxPre;
						x += dx; 
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
					//y -= min_ty * dy + ny * 0.4f;
				}

			}
			else if (dynamic_cast<CKoopas*>(e->obj))
			{
				Shell = dynamic_cast<CKoopas*>(e->obj);
				if (Shell->TypeKoopas == KOOPAS_TYPE_KOOPA_TROOPA_RED || Shell->TypeKoopas == KOOPAS_TYPE_KOOPA_TROOPA_GREEN) {
					if (Shell->GetState() == KOOPAS_STATE_SHELL && !PrepareCatch && e->nx != 0)
					{
						Shell->SetState(KOOPAS_STATE_ROTATORY);
						if (this->nx == 1)
							Shell->vx = KOOPAS_ROTATORY_SPEED;
						if (this->nx == -1)
							Shell->vx = -KOOPAS_ROTATORY_SPEED;
						KickShell = true;
						Kick_start = GetTickCount64();
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
							this->vy = -MARIO_JUMP_DEFLECT_SPEED*2;
						}
						else
						{
							Shell->SetState(KOOPAS_STATE_SHELL);
							this->vy = -MARIO_JUMP_DEFLECT_SPEED*2;
						}
					}
					else
					{
						if (untouchable == 0)
						{
							if (Shell->GetState() != KOOPAS_STATE_DIE)
							{
								if (level > MARIO_LEVEL_BIG)
								{
									SetLevel(MARIO_LEVEL_BIG);
									this->x -= MARIO_RACCOON_BBOX_TAIL;
									this->y -= 1;		// khong bi rot xuong Coobj
									StartUntouchable();
								}
								else if (level > MARIO_LEVEL_SMALL)
								{
									level = MARIO_LEVEL_SMALL;
									StartUntouchable();
								}
								else
									SetState(MARIO_STATE_DIE);
							}
							else {
								vy = vyPre;
								y += dy;
							}
						}
						else {
							vx = vxPre;
							x += dx; 
						}
					}
				}
				else if (Shell->TypeKoopas == KOOPAS_TYPE_KOOPA_PARATROOPA_GREEN) {
					if (e->ny < 0)
					{
						Shell->TypeKoopas = KOOPAS_TYPE_KOOPA_TROOPA_GREEN;
						this->vy = -MARIO_JUMP_DEFLECT_SPEED*2;
					}
					else if (e->nx != 0 && Iskilling)
					{
						Shell->TypeKoopas = KOOPAS_TYPE_KOOPA_TROOPA_GREEN;
						Shell->SetState(KOOPAS_STATE_SHELL);
						Shell->vy -= KOOPAS_DIE_DEFLECT_SPEED;
					}
					else if ((e->nx != 0 && !Iskilling) || (e->ny > 0)) {
						y -= min_ty * dy + ny * 0.4f;	//k bi rot
						if (untouchable == 0)
						{
								if (level > MARIO_LEVEL_BIG)
								{
									SetLevel(MARIO_LEVEL_BIG);
									this->x -= MARIO_RACCOON_BBOX_TAIL;
									this->y -= 1;		// khong bi rot xuong Coobj
									StartUntouchable();
								}
								else if (level > MARIO_LEVEL_SMALL)
								{
									level = MARIO_LEVEL_SMALL;
									StartUntouchable();
								}
								else
									SetState(MARIO_STATE_DIE);
							
						}
						else {
							vx = vxPre;
							x += dx;
						}
					}
				}
				else if (Shell->TypeKoopas == KOOPAS_TYPE_KOOPA_PARATROOPA_RED) {
					if (e->ny < 0)
					{
						Shell->TypeKoopas = KOOPAS_TYPE_KOOPA_TROOPA_RED;
						this->vy = -MARIO_JUMP_DEFLECT_SPEED * 2;
					}
					
				}

			}
			else if (dynamic_cast<Brick*>(e->obj)) {
				Brick* brick = dynamic_cast<Brick*>(e->obj);
				if (e->ny > 0)
				{
					if (brick->StoreItemQBrick)
					{
						if (brick->CountCollBrickMulMoney > 0 && brick->IdItemOfBrick == ITEM_MULTIPLE_MONEY) {
							brick->CountCollBrickMulMoney--;
							brick->QBick->SetState(BRICK_STATE_QUESTION_ON_UP);
							brick->QBick->YCollition = brick->QBick->y;
						}
						else {
							brick->BBox = false;
							brick->SwitchOff = true;
							brick->isDispread = true;
							if (brick->QBick != NULL && brick->QBick->GetState() == BRICK_STATE_QUESTION_ON)
							{
								brick->QBick->SetState(BRICK_STATE_QUESTION_ON_UP);
								brick->QBick->YCollition = brick->QBick->y;
							}
						}
					}
					else
						brick->IsBreaked = true;
						//brick->BBox = false;
				}
			}
			else if (dynamic_cast<QuestionBrick*>(e->obj))
			{
				QuestionBrick* brick = dynamic_cast<QuestionBrick*>(e->obj);
				if (brick->GetState() == BRICK_STATE_QUESTION_ON && e->ny > 0 && brick->vy == 0)
				{
					brick->SetState(BRICK_STATE_QUESTION_ON_UP);
					brick->YCollition = brick->y;
				}
			}
			else if (dynamic_cast<BulletPiranhaPlant*>(e->obj))
			{
				y -= min_ty * dy + ny * 0.4f;
			}
			else if (dynamic_cast<VenusFireTrap*>(e->obj))
			{
				if (untouchable == 0)
				{

					if (level > MARIO_LEVEL_BIG)
					{
						SetLevel(MARIO_LEVEL_BIG);
						this->x -= MARIO_RACCOON_BBOX_TAIL;
						this->y -= 1;		// khong bi rot xuong Coobj
							StartUntouchable();
					}
					else if (level > MARIO_LEVEL_SMALL)
					{
						level = MARIO_LEVEL_SMALL;
						StartUntouchable();
					}
					else
						SetState(MARIO_STATE_DIE);
				}
				else {
					vx = vxPre;
					x += dx;
				}
			}
			else if (dynamic_cast<Brothers*>(e->obj)) {
				Brothers* brother = dynamic_cast<Brothers*>(e->obj);
				if (brother->GetState() != BROTHER_STATE_DIE) {
					brother->SetState(BROTHER_STATE_DIE);
				}
			}
			else if (dynamic_cast<Items*>(e->obj)) 
			{
				Items* item = dynamic_cast<Items*>(e->obj);
				if (e->ny < 0 && item->GetState() != ITEM_SWITCH_STATE_OFF)
				{
					item->Active = true;
					item->SetState(ITEM_SWITCH_STATE_OFF);
				}
			}
			else if (dynamic_cast<FlyingWood*>(e->obj)) {
				FlyingWood* flyingWood = dynamic_cast<FlyingWood*>(e->obj);
				if (e->ny < 0)
				{
					flyingWood->IsCollMario = true;
					this->vy = FLYING_WOOD_VY*dt;
					//AllowJump = true;
					OnPlatform = true;
					//this->y = flyingWood->y - MARIO_BIG_BBOX_HEIGHT + flyingWood->dy;
				}
				if (e->nx < 0)
				{
					//this->x = flyingWood->x - MARIO_BIG_BBOX_WIDTH;
					//vx = flyingWood->vx;
					
				}
			}
			else if (dynamic_cast<MusicNote*>(e->obj)) {
				MusicNote* musicNote = dynamic_cast<MusicNote*>(e->obj);
				//vx = vxPre;
				isDeflect = true;
				if (e->ny < 0 && musicNote->GetState() == MUSIC_NOTE_STATE_IDLE) {
					musicNote->SetState(MUSIC_NOTE_STATE_MOVE_DOWN);	
				}
				else if (e->ny < 0 && musicNote->GetState() == MUSIC_NOTE_STATE_MOVE_UP) {
					AllowJump = true;		// nhay thap, thieu setstate k nhay cao
					SetState(MARIO_STATE_JUMP); // nhay cao, thieu allow jump k nhay lai
					if (!CGame::GetInstance()->IsKeyDown(DIK_S))
						YHolding = this->y + 35.0f;   // tru bot do cao
				}
				if (musicNote->typeNote == MUSIC_NOTE_TYPE_PINK) {
					if (e->ny < 0 && CGame::GetInstance()->IsKeyDown(DIK_S))
					{
						isDeflectByPinkNote = true;
						isDeflect = true;
					}
					else if (nx < 0)
					{
						musicNote->SetState(MUSIC_NOTE_STATE_MOVE_RIGHT_RIGHT);
						isDeflectLeft = true;
					}
					else if (nx > 0)
					{
						musicNote->SetState(MUSIC_NOTE_STATE_MOVE_LEFT_LEFT);
						isDeflectRight = true;
					}
				}

			}
			else if (dynamic_cast<CPortal *>(e->obj))
			{
				vx = vxPre;
				vy = vyPre;
				x += dx; y += dy;
			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	vector<LPGAMEOBJECT> coEventsResultColl;
	coEventsResultColl.clear();
	if (state != MARIO_STATE_DIE)
		CalCollisions(coObjects, coEventsResultColl);
	if (state == MARIO_STATE_DIE && level == MARIO_LEVEL_SMALL && !CheckMarioInScreen())
	{
		CGame::GetInstance()->SwitchScene(SCENCE_START);
		return;
	}
	else if (this->y > (CGame::GetInstance()->GetCamPosY() + CGame::GetInstance()->GetScreenWidth())&& CGame::GetInstance()->Getcurrent_scene() == SCENCE_WORD_MAP_4) {
		CGame::GetInstance()->SwitchScene(SCENCE_START);
		return;
	}
	if (coEventsResultColl.size() != 0)
	{
		for (UINT i = 0; i < coEventsResultColl.size(); i++)
		{
			LPGAMEOBJECT e = coEventsResultColl[i];
			if (dynamic_cast<CPortal*>(e))
			{
				CPortal* p = dynamic_cast<CPortal*>(e);
				if (CGame::GetInstance()->Getcurrent_scene() == SCENCE_START && StartTeleport)
				{
					CGame::GetInstance()->SwitchScene(p->GetSceneId());
					StartTeleport = false;
					return;			// khong return thi coObjects duoi se co 1 vai Obj = NULL
				}
				if (CGame::GetInstance()->Getcurrent_scene() != SCENCE_START)
				{
					if (CGame::GetInstance()->Getcurrent_scene() == SCENCE_WORD_MAP_4_1) {
						CGame::GetInstance()->SwitchScene(p->GetSceneId());
					}
					if (CGame::GetInstance()->Getcurrent_scene() == SCENCE_WORD_MAP_1_1 || CGame::GetInstance()->Getcurrent_scene() == SCENCE_WORD_MAP_4_1) {
						CGame::GetInstance()->SetReturnWorld(true);
					}
					IsWaitingTeleport = true;
					if (StartTeleport ) {
						if(CGame::GetInstance()->Getcurrent_scene() == SCENCE_WORD_MAP_1 || CGame::GetInstance()->Getcurrent_scene() == SCENCE_WORD_MAP_4)
							this->vy = MARIO_START_TELEPORT_VY * dt;
						else if (CGame::GetInstance()->Getcurrent_scene() == SCENCE_WORD_MAP_1_1 || CGame::GetInstance()->Getcurrent_scene() == SCENCE_WORD_MAP_4_1) {
							this->vy = -MARIO_START_TELEPORT_VY * dt;
						}
					}
					if (this->y > p->y && (CGame::GetInstance()->Getcurrent_scene() == SCENCE_WORD_MAP_1 || CGame::GetInstance()->Getcurrent_scene() == SCENCE_WORD_MAP_4)) {
							CGame::GetInstance()->SwitchScene(p->GetSceneId());
					}
					else if (this->y < p->y && (CGame::GetInstance()->Getcurrent_scene() == SCENCE_WORD_MAP_1_1) ) {
						CGame::GetInstance()->SwitchScene(p->GetSceneId());
					}
					TeleUp = false;
					return;
				}
				else {
					if (this->x < p->x)
						vx = 0;
				}
			}
			else if (dynamic_cast<Tube*>(e)) {
				vy = -MARIO_START_TELEPORT_VY * dt;
				TeleUp = true;
			}
			else if (dynamic_cast<FlyingWood*>(e)) {
				FlyingWood* flyingWood = dynamic_cast<FlyingWood*>(e);

				if (this->nx > 0 && this->x < flyingWood->x - MARIO_SMALL_BBOX_WIDTH)
				{
					if (level == MARIO_LEVEL_BIG || level == MARIO_LEVEL_FIRE) {
						this->x = flyingWood->x - MARIO_BIG_BBOX_WIDTH;
						if (vx != 0)
							this->vx = flyingWood->vx;
					}
					else if (level == MARIO_LEVEL_RACCOON) {
						//this->x = flyingWood->x - MARIO_RACCOON_BBOX_WIDTH + MARIO_RACCOON_BBOX_TAIL;//cong them bb duoi
						//if (vx != 0)
						//	this->vx = flyingWood->vx;
					}
					else if (level == MARIO_LEVEL_SMALL){
						this->x = flyingWood->x - MARIO_SMALL_BBOX_WIDTH;

						if (vx != 0)
							this->vx = flyingWood->vx;
					}


				}
			}
			else if (dynamic_cast<BulletPiranhaPlant*>(e)) {
				if (untouchable == 0)
				{
					if (level > MARIO_LEVEL_BIG)
					{
						SetLevel(MARIO_LEVEL_BIG);
						this->x -= MARIO_RACCOON_BBOX_TAIL;
						this->y -= 1;		// khong bi rot xuong Coobj
						StartUntouchable();
					}
					else if (level > MARIO_LEVEL_SMALL)
					{
						level = MARIO_LEVEL_SMALL;
						StartUntouchable();
					}
					else
						SetState(MARIO_STATE_DIE);
				}
			}
			else if (dynamic_cast<BoomerangOfBrother*>(e)) {
				if (untouchable == 0)
				{
					if (level > MARIO_LEVEL_BIG)
					{
						SetLevel(MARIO_LEVEL_BIG);
						this->x -= MARIO_RACCOON_BBOX_TAIL;
						this->y -= 1;		// khong bi rot xuong Coobj
						StartUntouchable();
					}
					else if (level > MARIO_LEVEL_SMALL)
					{
						level = MARIO_LEVEL_SMALL;
						StartUntouchable();
					}
					else
						SetState(MARIO_STATE_DIE);
				}
			}
			/*else {
				IsWaitingTeleport = false;
				StartTeleport = false;
			}*/
		}
	}
	else {
		TeleUp = false;
	} 
}

void CMario::Render()
{
	if (level != MARIO_LEVEL_MINI)
	{
		firebullet_1->Render();
		firebullet_2->Render();
		TailofRaccoon->Render();
	}

	if (untouchable) {
		CountMiliSeconds++;
		if(CountMiliSeconds >= 0)
			alpha = 30;
		if (CountMiliSeconds >= 10)
		{
			alpha = 100;
			if (CountMiliSeconds == 15)
				CountMiliSeconds = 0;
		}
	}
	else alpha = 255;
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
			if (nx > 0) {
				ani = MARIO_ANI_SMALL_IDLE_RIGHT;
				if (AllowJump)
					ani = MARIO_ANI_SMALL_JUMP_RIGHT;
			}
			else {
				ani = MARIO_ANI_SMALL_IDLE_LEFT;
				if (AllowJump)
					ani = MARIO_ANI_SMALL_JUMP_LEFT;
			}
		}
		else if (vx > 0)
		{
			ani = MARIO_ANI_SMALL_WALKING_RIGHT;
			if (AllowJump)
				ani = MARIO_ANI_SMALL_JUMP_RIGHT;
		}
		else {
			ani = MARIO_ANI_SMALL_WALKING_LEFT;
			if (AllowJump)
				ani = MARIO_ANI_SMALL_JUMP_LEFT;
		}
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
				//if(!AllowJump && !OnPlatform)
				if(IsDropping || (isDeflect && vy >= 0))
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
				if (StartTeleport)
					ani = MARIO_ANI_RACCOON_TELEPORT;
				if(TeleUp)
					ani = MARIO_ANI_RACCOON_TELEPORT;
			}
			else 
			{
				ani = MARIO_ANI_RACCOON_IDLE_LEFT;
				if (AllowJump)
					ani = MARIO_ANI_RACCOON_JUMP_LEFT;
				if (IsDropping || (isDeflect && vy < 0))
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
				if (StartTeleport)
					ani = MARIO_ANI_RACCOON_TELEPORT;
				if(TeleUp)
					ani = MARIO_ANI_RACCOON_TELEPORT;
			}
		}
		else if (vx > 0)
		{
			ani = MARIO_ANI_RACCOON_WALKING_RIGHT;
			if (AllowJump)
				ani = MARIO_ANI_RACCOON_JUMP_RIGHT;
			if (IsDropping || (isDeflect && vy >= 0))
				ani = MARIO_ANI_RACCOON_DROP_RIGHT;
			if (Iskilling && Kill) {
				ani = MARIO_ANI_RACCOON_KILL_RIGHT;
			}
			if (vx < 0.1 && nx == -1)
				ani = MARIO_ANI_RACCOON_SLIP_RIGHT;
			if (isDeflectRight)
				ani = MARIO_ANI_RACCOON_DROP_LEFT;
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
			if (IsDropping || (isDeflect && vy < 0))
				ani = MARIO_ANI_RACCOON_DROP_LEFT;
			if (Iskilling && Kill) {
				ani = MARIO_ANI_RACCOON_KILL_LEFT;
			}
			if (vx > -0.1 && nx == 1)
				ani = MARIO_ANI_RACCOON_SLIP_LEFT;
			if (isDeflectLeft)
				ani = MARIO_ANI_RACCOON_DROP_RIGHT;
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
	else if (level == MARIO_LEVEL_MINI) {
		ani = MARIO_MINI_ANI;
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

bool CMario::CheckMarioInScreen()
{
	float XLeftScreen = CGame::GetInstance()->GetCamPosX() - 48;
	float XRightScreen = CGame::GetInstance()->GetCamPosX() + CGame::GetInstance()->GetScreenWidth() + 48;
	float YTopScreen = CGame::GetInstance()->GetCamPosY() - 48;
	float YBotScreen = CGame::GetInstance()->GetCamPosY() + CGame::GetInstance()->GetScreenHeight() + 48;

	if (this->x < XLeftScreen || this->x > XRightScreen)
		return false;
	if (this->y < YTopScreen || this->y > YBotScreen)
		return false;
	return true;
}

void CMario::SetState(int state)
{
	CGameObject::SetState(state);
	if (level == MARIO_LEVEL_MINI)
	{
		switch (state)
		{
		case MARIO_MINI_STATE_UP:
			vy = -MARIO_MINI_SPEED;
			vx = 0;
			break;
		case MARIO_MINI_STATE_DOWN:
			vy = MARIO_MINI_SPEED;
			vx = 0;
			break;
		case MARIO_MINI_STATE_LEFT:
			vy = 0;
			vx = -MARIO_MINI_SPEED;
			break;
		case MARIO_MINI_STATE_RIGHT:
			vy = 0;
			vx = MARIO_MINI_SPEED;
			break;
		case MARIO_MINI_STATE_TELEPORT:
			StartTeleport = true;
			//this->y += 1.0f;
			break;
		}
	}
	else {
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
		if (vx < (MARIO_VY_ASCENDING_SLIP_MAX *dt) && vx != 0)
		{
			vx += MARIO_VY_ASCENDING_SLIP * dt;
			if (vx > (MARIO_VY_ASCENDING_SLIP_MAX * dt))
				vx = (MARIO_VY_ASCENDING_SLIP_MAX * dt);
		}
		if (vx == 0 && !IsRunning)
			vx = MARIO_WALKING_SPEED * dt;
		if (IsRunning && vx < MARIO_MAX_SPEED_RUNNING && !IsFlying && !IsLimitRunning)
			vx += MARIO_VY_ASCENDING_RUN * dt;
			nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT: 
		if (!HaveInertia)
			XHolding = x;
		HaveInertia = true;
		if (vx > -(MARIO_VY_ASCENDING_SLIP_MAX * dt) && vx != 0)
		{
			vx -= MARIO_VY_ASCENDING_SLIP * dt;
			if (vx < -(MARIO_VY_ASCENDING_SLIP_MAX * dt))
				vx = -(MARIO_VY_ASCENDING_SLIP_MAX * dt);
		}
		if(vx == 0 && !IsRunning)
			vx = -MARIO_WALKING_SPEED * dt;
		if (IsRunning && vx != -MARIO_MAX_SPEED_RUNNING && !IsFlying && !IsLimitRunning)
			vx -= MARIO_VY_ASCENDING_RUN * dt;
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
		if (!AllowJump && !IsFlying)
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
	case MARIO_STATE_UP:
		if (IsWaitingTeleport)
			StartTeleport = true;
		break;
	case MARIO_STATE_BEND_OVER:
		if (!IsWaitingTeleport) {
			if (!IsBendingOver)
			{
				this->y += MARIO_Y_BEND_OVER;
			}
			IsBendingOver = true;
		}
		else {
			if(IsWaitingTeleport)
				StartTeleport = true;
		}
	case MARIO_STATE_IDLE: 
		IsLimitRunning = false;
		if (x - XHolding > MARIO_DISTANCE_INERTIA) // khoan cach bi vang
		{
			if (vx > 0.0f)
			{
				StartSlip();
				vx -= MARIO_INERTIA_DECREASE * dt;
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
				vx += MARIO_INERTIA_DECREASE * dt;
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
			if(!isDeflect)
				vx = 0;
			//if (isDeflectLeft)
			//	SetState(MARIO_IS_DEFLECTED_LEFT);
			HaveInertia = false;
			break;
		}

	case MARIO_STATE_DIE:
		vy = -MARIO_DIE_DEFLECT_SPEED * dt;
		int Life = CGame::GetInstance()->GetLife();
		CGame::GetInstance()->SetLife(Life - 1);
		//if (CGame::GetInstance()->GetLife() == 0)
		break;
	}
	}
}

void CMario::SetLevel(int l)
{
	level = l;
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
	case MARIO_LEVEL_MINI:
		right = x + 15;
		bottom = y + 15;
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


