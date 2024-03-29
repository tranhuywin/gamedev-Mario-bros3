#include "Items.h"
#include "Utils.h"
#include "QuestionBrick.h"
#include "Brick.h"
#include "Ground.h"
#include "BulletPiranhaPlant.h"
#include "WoodenBrick.h"
void Items::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (BBox) {
		if (IdItem == ITEM_SWITCH)
		{
			left = x;
			top = y + 1;
		}
		else {
			left = x + ITEM_BBOX_L;
			top = y + ITEM_BBOX_T;
		}
		
		if (!Active)
		{
			right = x + ITEM_BBOX_R;
			bottom = y + ITEM_BBOX_B;
			if (IdItem == ITEM_MONEY_IDLE)
			{
				left = x;
			}
			else if (IdItem == ITEM_MULTIPLE_MONEY) {
				right = x + ITEM_BBOX_MONEY_IDLE;
				bottom = y + ITEM_BBOX_MONEY_IDLE + 1;
			}
		}
		else {
			left = x + ITEM_BBOX_L;
			top = y + ITEM_BBOX_T;
			right = x + ITEM_BBOX_R_ACTIVE;
			bottom = y + ITEM_BBOX_B_ACTIVE;
			if (IdItem == ITEM_SWITCH)
			{
				right = x + ITEM_BBOX_SWITCH;
				bottom = y + ITEM_BBOX_SWITCH;
				if (state == ITEM_SWITCH_STATE_OFF)
					bottom = y + ITEM_BBOX_SWITCH_B;
			}
			else if (IdItem == ITEM_MONEY_ROTATOR || IdItem == ITEM_CARD) {
				right = x + ITEM_BBOX_MONEY_IDLE - ITEM_BBOX_L;
				bottom = y + ITEM_BBOX_MONEY_IDLE;
			}
			else if (IdItem == ITEM_MUSHROOM_GREEN || IdItem == ITEM_MUSHROOM_RED) {
				left = x;
				top = y;
				right = x + ITEM_BBOX_MUSHROOM;
				bottom = y + ITEM_BBOX_MUSHROOM;
			}
		}
	}
}

void Items::Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects)
{
	/*if ((CGame::GetInstance()->GetLevel() == MARIO_LEVEL_RACCOON) && IdItem == ITEM_ANI_TREE_LEAF)
	{
		IdItem = ITEM_ANI_MUSHROOM_RED;
	}*/
	if (!SetPosStart)
	{
		this->X_Start = this->x;
		this->Y_Start = this->y;
		SetPosStart = true;
	}
	if (effect != NULL)
		effect->Update(dt);

	if (BrickBreak != NULL)
		if (IdItem == ITEM_MONEY_IDLE && BrickBreak->IsBreaked)	//brick da bi be
		{
			Active = false;
			BBox = false;
		}else if (BrickBreak->SwitchOff)
		{
			if(IdItem != ITEM_MULTIPLE_MONEY)
				Active = true;
		}

	//Active == true thi se co chuyen gi xay ra 
	{
		if (Active)
		{
			if (IdItem == ITEM_TREE_LEAF)
			{
				vy += ITEM_GRAVITY * dt;
				if (vy > 2 * ITEM_GRAVITY * dt)
					vy = 2 * ITEM_GRAVITY * dt;
				if ((this->x - this->X_Start) > ITEM_TREE_LEAF_X_DISTANCE_X_START)
					vx -= ITEM_TREE_LEAF_VX * dt;
				else
					vx += ITEM_TREE_LEAF_VX * dt;
			}
			else if (IdItem == ITEM_MONEY || IdItem == ITEM_MULTIPLE_MONEY) {
				vy += ITEM_GRAVITY * dt;
				if (y - Y_Start > 0 && vy != 0)
				{
					int CurentScore = CGame::GetInstance()->GetScore();
					CGame::GetInstance()->SetScore(CurentScore + 100);
					int CurentMoney = CGame::GetInstance()->GetMoney();
					CGame::GetInstance()->SetMoney(CurentMoney + 1);
					AniEffect = SpriteEffectStart + EFFECT_100;
					effect = new Effect(this->x, this->y - BRICK_BBOX_HEIGHT, AniEffect);
					vx = 0; vy = 0;
					CountColl--;
					if (CountColl <= 0)
						BBox = false;
					if (IdItem == ITEM_MULTIPLE_MONEY) {
						this->x = X_Start;
						this->y = Y_Start;
					}
					Active = false;
				}
			}
			else if (IdItem == ITEM_SWITCH)
			{
				if (Y_Start - y > ITEM_SWITCH_YSTART_DISTANCE_Y)
					vy = 0;
				if (state == ITEM_SWITCH_STATE_OFF) {
					for (unsigned int i = 0; i < coObjects->size(); i++) {
						if (dynamic_cast<Brick*>(coObjects->at(i)))
						{
							Brick* brick = dynamic_cast<Brick*>(coObjects->at(i));
							brick->SwitchOff = true;
						}
					}
				}
			}
			else if (IdItem == ITEM_CARD) {
				if (EndScence) {
					AniEffect = SpriteEffectStart + EFFECT_END_SCENCE;
					float xdraw = CGame::GetInstance()->GetCamPosX() + CGame::GetInstance()->GetScreenWidth() / 3;
					float ydraw = CGame::GetInstance()->GetCamPosY();
					effect = new Effect(xdraw, ydraw, AniEffect);
				}

			}
		}
		else {
			vy = 0; vx = 0;
		}
	}
	//
	// Canh bao truoc khi va cham cho mushroom
	{
		CGameObject::Update(dt);
		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResultPro;
		coEvents.clear();
		if (Active && (IdItem == ITEM_MUSHROOM_GREEN || IdItem == ITEM_MUSHROOM_RED))
			CalcPotentialCollisions(coObjects, coEvents);
		if (coEvents.size() == 0)
		{
			x += dx;
			y += dy;
		}
		else {
			float min_tx, min_ty, nx = 0, ny;
			float rdx = 0, rdy = 0;
			float vxPre = vx, vyPre = vy;
			FilterCollision(coEvents, coEventsResultPro, min_tx, min_ty, nx, ny, rdx, rdy);
			x += min_tx * dx + nx * 0.3f;
			y += min_ty * dy + ny * 0.3f;

			if (nx != 0 && ny != 0 && (IdItem == ITEM_MUSHROOM_GREEN || IdItem == ITEM_MUSHROOM_RED))
			{
				vx = -vx;
				this->nx = -this->nx;
			}
			if (ny != 0)
				vy = 0;
			for (UINT i = 0; i < coEventsResultPro.size(); i++)
			{
				LPCOLLISIONEVENT e = coEventsResultPro[i];
				if (dynamic_cast<CMario*>(e->obj)) {
					CMario* mario = dynamic_cast<CMario*>(e->obj);
					if (nx != 0) {
						vx = vxPre;
						x += dx;
					}
					if (IdItem == ITEM_MUSHROOM_RED || IdItem == ITEM_MUSHROOM_GREEN) {
						
						AniEffect = SpriteEffectStart + EFFECT_1000;
						effect = new Effect(this->x, this->y, AniEffect);
						int CurrentScore = CGame::GetInstance()->GetScore();
						CGame::GetInstance()->SetScore(CurrentScore + 1000);
						if (mario->GetLevel() == MARIO_LEVEL_SMALL) {
							mario->y -= MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT;
							mario->SetLevel(MARIO_LEVEL_BIG);
						}
						this->Active = false;
						BBox = false;
					}
				}
				else if (dynamic_cast<Brick*>(e->obj)) {
					if (nx != 0)
					{
						vx = vxPre;
						x += dx;
					}
				}
				else if (dynamic_cast<BulletPiranhaPlant*>(e->obj)) {
					vx = vxPre;
					vy = vyPre;
					x += dx; y += dy;
				}
			}
		}
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
	//
	//canh bao trong khi va cham cho tat ca items
	vector<LPGAMEOBJECT> coEventsResult;
	coEventsResult.clear();
	CalCollisions(coObjects, coEventsResult);
	unsigned int sizeCo = coEventsResult.size();

	if (sizeCo == 0 && (IdItem == ITEM_MUSHROOM_GREEN || IdItem == ITEM_MUSHROOM_RED))
	{
		if(nx == 1)
			vx = ITEM_MUSHROOM_VX * dt;
		else
			vx = -ITEM_MUSHROOM_VX * dt;
		vy = ITEM_MUSHROOM_GRANVITY * dt;
	}
	if (sizeCo != 0)
	{
		for (unsigned int i = 0; i < sizeCo; i++)
		{
			LPGAMEOBJECT e = coEventsResult[i];
			if (dynamic_cast<CMario*>(e))
			{
				CMario* mario = dynamic_cast<CMario*>(e);
				if (IdItem == ITEM_TREE_LEAF)
				{
					if (mario->GetLevel() == MARIO_LEVEL_RACCOON && vy > 0)
					{
						AniEffect = SpriteEffectStart + EFFECT_1000;
						effect = new Effect(this->x, this->y - BRICK_BBOX_HEIGHT, AniEffect);
						int CurentScore = CGame::GetInstance()->GetScore();
						CGame::GetInstance()->SetScore(CurentScore + 1000);
						int CurentMoney = CGame::GetInstance()->GetMoney();
						CGame::GetInstance()->SetMoney(CurentMoney + 1);
						x = -100; y = -100; vy = 0; vx = 0;
						Active = false;
					}
					if(mario->GetLevel() == MARIO_LEVEL_RACCOON && sizeCo == 1)
					{
						//x = -100; y = -100; vy = 0; vx = 0;
						BBox = false;
						Active = false;
					}
					if (mario->GetLevel() == MARIO_LEVEL_BIG)
						mario->SetLevel(MARIO_LEVEL_RACCOON);
				}
				else if (IdItem == ITEM_MONEY_IDLE)
					{
						//if (mario->Iskilling)
						//{
						//	//CollTail = true;
						//	//DebugOut(L"Kill\n");
						//	//MarioGetMoney = true;
						//}
						if(Active)
						{
							int CurentScore = CGame::GetInstance()->GetScore();
							CGame::GetInstance()->SetScore(CurentScore + 100);
							int CurentMoney = CGame::GetInstance()->GetMoney();
							CGame::GetInstance()->SetMoney(CurentMoney + 1);
							x = -100; y = -100; vy = 0; vx = 0;
							BBox = false;
							Active = false;
							BrickBreak->IsBreaked = true;
							/*if (BrickBreak != NULL)
							{
								BrickBreak->x = -100;
								BrickBreak->y = -100;
							}*/
						}
					}
				else if (IdItem == ITEM_MONEY_ROTATOR ) {
					Active = false;
					/*int CurentScore = CGame::GetInstance()->GetScore();
					CGame::GetInstance()->SetScore(CurentScore + 100);
					int CurentMoney = CGame::GetInstance()->GetMoney();
					CGame::GetInstance()->SetMoney(CurentMoney + 1);*/
					BBox = false;
				}
				else if (IdItem == ITEM_MULTIPLE_MONEY) {
					vy = -ITEM_DEFLECT_SPEED * dt;
					Active = true;
				}
				else if (IdItem == ITEM_SWITCH) {
					if (mario->vy > 0)
					{
						this->SetState(ITEM_SWITCH_STATE_OFF);
					}
				}
				else if (IdItem == ITEM_CARD) {
					int Frame = animation_set->at(ITEM_ANI_CARD)->GetcurrentFrame();
					
					if (CGame::GetInstance()->GetCard_1() == -1) {
						CGame::GetInstance()->SetCard_1(Frame + 1);
					}
					else if (CGame::GetInstance()->GetCard_2() == -1) {
						CGame::GetInstance()->SetCard_2(Frame + 1);
					}
					else if (CGame::GetInstance()->GetCard_3() == -1) {
						CGame::GetInstance()->SetCard_3(Frame + 1);
					}
					//Active = false;
					EndScence = true;
					vy = -0.03f * dt;
					BBox = false;
				}
				else if ((IdItem == ITEM_MUSHROOM_GREEN || IdItem == ITEM_MUSHROOM_RED) && vx != 0) {
					if (IdItem == ITEM_MUSHROOM_GREEN) {
						AniEffect = SpriteEffectStart + EFFECT_1_UP;
						effect = new Effect(this->x, this->y, AniEffect);
						int CurentLife = CGame::GetInstance()->GetLife();
						CGame::GetInstance()->SetLife(CurentLife + 1);
					}
					else {
						AniEffect = SpriteEffectStart + EFFECT_1000;
						effect = new Effect(this->x, this->y, AniEffect);
						int CurrentScore = CGame::GetInstance()->GetScore();
						CGame::GetInstance()->SetScore(CurrentScore + 1000);
						if (mario->GetLevel() == MARIO_LEVEL_SMALL) {
							mario->y -= MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT;
							mario->SetLevel(MARIO_LEVEL_BIG);
						}

					}
					this->Active = false;
					BBox = false;
				}
			}
			else if (dynamic_cast<QuestionBrick*>(e))
			{
				//brick->StoreItemQBrick = true;
				QuestionBrick* QBrick = dynamic_cast<QuestionBrick*>(e);
				if (QBrick->GetState() == BRICK_STATE_QUESTION_ON_UP)
				{
					if (IdItem == ITEM_TREE_LEAF || IdItem == ITEM_MONEY || IdItem == ITEM_MULTIPLE_MONEY)
						vy = -ITEM_DEFLECT_SPEED * dt;
					Active = true;
				}
				if (QBrick->GetState() == BRICK_STATE_QUESTION_OFF)
				{
					if (IdItem == ITEM_SWITCH)
						vy = -ITEM_SWITCH_VY * dt;
					if ((IdItem == ITEM_MUSHROOM_GREEN || IdItem == ITEM_MUSHROOM_RED) && Active)
					{
						vx = 0; vy = -ITEM_MUSHROOM_VY_UP * dt;
					}
					if(IdItem != ITEM_MULTIPLE_MONEY)
						Active = true;
				}
			}
			else if (dynamic_cast<WoodenBrick*>(e)) 
			{
				WoodenBrick* brick = dynamic_cast<WoodenBrick*>(e);
				if (brick->haveItem && brick->GetState() != BRICK_STATE_IDLE)
				{
					if (IdItem == ITEM_TREE_LEAF)
						vy = -ITEM_DEFLECT_SPEED * dt;
					Active = true;
					brick->haveItem = false;
				}
			}
			else if (dynamic_cast<Brick*>(e) && BrickBreak == NULL) {
				Brick* brick = dynamic_cast<Brick*>(e);
				brick->IdItemOfBrick = IdItem;		
				BrickBreak = brick;
			}

		}
	}
}
void Items::Render()
{
	if (effect != NULL)
		effect->Render();
	int ani = -1;
	if (this->Active)
		if (IdItem == ITEM_TREE_LEAF)
			ani = ITEM_ANI_TREE_LEAF;
		else if (IdItem == ITEM_MONEY || IdItem == ITEM_MONEY_ROTATOR || IdItem == ITEM_MULTIPLE_MONEY)
			ani = ITEM_ANI_MONEY;
		else if (IdItem == ITEM_MONEY_IDLE)
			ani = ITEM_ANI_MONEY_IDLE;
		else if (IdItem == ITEM_MUSHROOM_GREEN)
			ani = ITEM_ANI_MUSHROOM_GREEN;
		else if (IdItem == ITEM_MUSHROOM_RED)
			ani = ITEM_ANI_MUSHROOM_RED;
		else if (IdItem == ITEM_CARD)
			ani = ITEM_ANI_CARD;
		else if (IdItem == ITEM_SWITCH)
		{
			if (state != ITEM_SWITCH_STATE_OFF)
				ani = ITEM_ANI_SWITCH_ON;
			else {
					ani = ITEM_ANI_SWITCH_OFF;
			}
			//RenderBoundingBox();
		}
	if(ani != -1 && BBox)
		animation_set->at(ani)->Render(x, y);

}

Items::Items(int IdItem, int SpriteEffectStart)
{
	this->IdItem = IdItem;
	if (IdItem == ITEM_MONEY_ROTATOR)
		Active = true;
	else if(IdItem == ITEM_CARD)
		Active = true;
	else if (IdItem == ITEM_MUSHROOM_GREEN)
	{
		srand((int)time(NULL));
		int rd = rand() % (2);
		if (rd == 1)
			nx = 1;
		else
			nx = -1;
	}
	this->SpriteEffectStart = SpriteEffectStart;
}

void Items::SetState(int state)
{
	switch (state)
	{
	case ITEM_STATE_LISTEN:
		vx = 0;
		vy = 0;
		break;
	case ITEM_SWITCH_STATE_OFF:
		if(this->state != ITEM_SWITCH_STATE_OFF)
		y += ITEM_BBOX_SWITCH - ITEM_BBOX_SWITCH_B;
		break;
	}
	CGameObject::SetState(state);
}
