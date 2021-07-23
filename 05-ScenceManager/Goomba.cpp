#include "Goomba.h"
#include "Utils.h"
#include "Items.h"
#include "Line.h"
CGoomba::CGoomba(int TypeGoomba)
{
	this->TypeGoomba = TypeGoomba;
	if (TypeGoomba != BROWN_PARA_GOOMBA)
		SetState(GOOMBA_STATE_WALKING);
	else
	{
		SetState(BROWN_PARAGOOMBA_STATE_FLYING);
		vy = 0.075f;
		MGoomba_1 = new MicroGoomba();
		MGoomba_2 = new MicroGoomba();
		MGoomba_3 = new MicroGoomba();
		MGoomba_4 = new MicroGoomba();
	}
	vx = -0.05f;
}

CGoomba::CGoomba(int TypeGoomba, CMario* mario)
{
	this->TypeGoomba = TypeGoomba;
	if (TypeGoomba != BROWN_PARA_GOOMBA)
		SetState(GOOMBA_STATE_WALKING);
	else
	{
		SetState(BROWN_PARAGOOMBA_STATE_FLYING);
		vy = 0.075f;
		MGoomba_1 = new MicroGoomba();
		MGoomba_2 = new MicroGoomba();
		MGoomba_3 = new MicroGoomba();
		MGoomba_4 = new MicroGoomba();

		MGoomba_1->mario = mario;
		MGoomba_2->mario = mario;
		MGoomba_3->mario = mario;
		MGoomba_4->mario = mario;
	}
	vx = -0.05f;
	this->mario = mario;
}

void CGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	if (TypeGoomba == GOOMBA_NORMAL)
	{
		right = x + GOOMBA_BBOX_WIDTH;
		bottom = y + GOOMBA_BBOX_HEIGHT;
	}
	else if (TypeGoomba == RED_PARA_GOOMBA)
	{
		if (LevelParaGoomba == PARAGOOMBA_LEVEL)
		{
			right = x + GOOMBA_BBOX_WIDTH;
			bottom = y + GOOMBA_BBOX_HEIGHT;
		}
		else if (LevelParaGoomba == PARAGOOMBA_WINGED_LEVEL)
		{
			right = x + PARAGOOMBA_WINGED_BBOX_WIDTH;
			bottom = y + PARAGOOMBA_WINGED_BBOX_HEIGHT;
		}
		else {
			right = x + GOOMBA_BBOX_WIDTH;
			bottom = y + GOOMBA_BBOX_HEIGHT_DIE;
		}
	}
	else if (TypeGoomba == BROWN_PARA_GOOMBA)
	{
		right = x + PARAGOOMBA_WINGED_BBOX_WIDTH;
		bottom = y + PARAGOOMBA_WINGED_BBOX_HEIGHT;
	}
	if (state == GOOMBA_STATE_DIE)
		bottom = y + GOOMBA_BBOX_HEIGHT_DIE;
}

void CGoomba::Update(ULONGLONG dt, vector<LPGAMEOBJECT> *coObjects)
{
	if (effect != NULL) {
		effect->Update(dt);
	}
	if (firstUpdate) {
		yInit = this->y; xInit = this->x;
		firstUpdate = false;
	}

	if (state != GOOMBA_STATE_DIE && TypeGoomba != BROWN_PARA_GOOMBA)
		vy += GOOMBA_GRAVITY * dt;
	else if(TypeGoomba != BROWN_PARA_GOOMBA)
	{
		vx = 0; vy = 0;
	}
	if (TypeGoomba == BROWN_PARA_GOOMBA) {
		MGoomba_1->Update(dt, coObjects);
		MGoomba_2->Update(dt, coObjects);
		MGoomba_3->Update(dt, coObjects);
		MGoomba_4->Update(dt, coObjects);
		coObjects->push_back(MGoomba_1);
		coObjects->push_back(MGoomba_2);
		coObjects->push_back(MGoomba_3);
		coObjects->push_back(MGoomba_4);
	}
	CGameObject::Update(dt);
	if(state == GOOMBA_STATE_DIE)
		if (Die && GetTickCount64() - Die_start > animation_set->at(ani)->GettotalFrameTime())
		{
			Die_start = 0;
			Die = 0;
			Died = true;
		}
	
	if (TypeGoomba == RED_PARA_GOOMBA) {
		if (LevelParaGoomba == PARAGOOMBA_WINGED_LEVEL)
			if (vy > -PARAGOOMBA_VY_WING_BIG && vy < PARAGOOMBA_VY_WING_BIG)
			{
				ani = RED_PARAGOOMBA_WINGED_BIG_ANI;
			}
			else ani = RED_PARAGOOMBA_WINGED_ANI;

		if (JumpCount == PARAGOOMBA_JUMP_COUNT)
		{
			SetState(PARAGOOMBA_WINGED_STATE_JUMP_HEIGHT);
			JumpCount = 0;
		}
	}
	else if (TypeGoomba == BROWN_PARA_GOOMBA)
	{
		if (this->y < (yInit - BROWN_PARAGOOMBA_DISTANCE_Y_FLY))
			vy = BROWN_PARAGOOMBA_VY_FLYING * dt;
		else if (this->y >= (yInit + BROWN_PARAGOOMBA_DISTANCE_Y_FLY) && vy > 0)
		{
			vy = -BROWN_PARAGOOMBA_VY_FLYING * dt;

			//micro goomba start falling
			{
				if (MGoomba_1->GetState() == MICRO_GOOMBA_STATE_WAIT_FALLING)
				{
					MGoomba_1->setPositionFalling(this->x + X_FALLING, this->y + Y_FALLING);
					MGoomba_1->SetState(MICRO_GOOMBA_STATE_FALLING);
				}
				else if (MGoomba_2->GetState() == MICRO_GOOMBA_STATE_WAIT_FALLING)
				{
					MGoomba_2->setPositionFalling(this->x + X_FALLING, this->y + Y_FALLING);
					MGoomba_2->SetState(MICRO_GOOMBA_STATE_FALLING);
				}
				else if (MGoomba_3->GetState() == MICRO_GOOMBA_STATE_WAIT_FALLING)
				{
					MGoomba_3->setPositionFalling(this->x + X_FALLING, this->y + Y_FALLING);
					MGoomba_3->SetState(MICRO_GOOMBA_STATE_FALLING);
				}
				else if (MGoomba_4->GetState() == MICRO_GOOMBA_STATE_WAIT_FALLING)
				{
					MGoomba_4->setPositionFalling(this->x + X_FALLING, this->y + Y_FALLING);
					MGoomba_4->SetState(MICRO_GOOMBA_STATE_FALLING);
				}
			}
		}

		if (this->x < (xInit - BROWN_PARAGOOMBA_DISTANCE_X_FLY))
			vx = GOOMBA_WALKING_SPEED * dt;
		else if (this->x > xInit + BROWN_PARAGOOMBA_DISTANCE_X_FLY)
			vx = -GOOMBA_WALKING_SPEED * dt;
		
	}
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	if (!Died)
		CalcPotentialCollisions(coObjects, coEvents);
	else {
		x = 0; y = 0; vx = 0; vy = 0;
	}

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
		if (nx != 0)
			vx = -vx;
		if (ny != 0)
		{
			vy = 0;
			if (TypeGoomba == RED_PARA_GOOMBA)
				if (LevelParaGoomba == PARAGOOMBA_WINGED_LEVEL) {
					vy = -PARAGOOMBA_JUMP_DEFLECT_SPEED * dt;
					JumpCount++;
				}
		}

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<Line*>(e->obj))
			{
				Line* line = dynamic_cast<Line*>(e->obj);
				if (e->ny > 0)		// o duoi len
				{
					vy = vyLine;
					y += dy;
				}
			}
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CGoomba::Render()
{
	float YRender = y;
	if (effect != NULL) {
		effect->Render();
	}
	if (TypeGoomba == GOOMBA_NORMAL)
	{
		ani = GOOMBA_ANI_WALKING;
		if (state == GOOMBA_STATE_DIE) {
			ani = GOOMBA_ANI_DIE;
		}
	}
	else if (TypeGoomba == RED_PARA_GOOMBA)
	{
		if (LevelParaGoomba == PARAGOOMBA_WINGED_LEVEL)
		{
			if (ani == RED_PARAGOOMBA_WINGED_BIG_ANI)
				YRender -= PARAGOOMBA_WINGED_BIG_BBOX_HEIGHT - PARAGOOMBA_WINGED_BBOX_HEIGHT;
		}
		else {
			ani = RED_PARAGOOMBA_WALKING_ANI;
			if (state == GOOMBA_STATE_DIE)
				ani = RED_PARAGOOMBA_DIE_ANI;
		}
	}
	else if (TypeGoomba == BROWN_PARA_GOOMBA)
	{
		ani = BROWN_PARAGOOMBA_WINGED_ANI;
		MGoomba_1->Render();
		MGoomba_2->Render();
		MGoomba_3->Render();
		MGoomba_4->Render();
	}
	if (!Died)
		animation_set->at(ani)->Render(x, YRender);
	//RenderBoundingBox();
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		case GOOMBA_STATE_DIE:
			StartDie();
			vx = 0;
			y += GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE - 1.0f;
			if (effect == NULL)
			{
				effect = new Effect(this->x, this->y - GOOMBA_BBOX_HEIGHT, CGame::GetInstance()->GetSpriteEffect() + EFFECT_100);
				int CurrentScore = CGame::GetInstance()->GetScore();
				CGame::GetInstance()->SetScore(CurrentScore + 100);
			}
			break;
		case GOOMBA_STATE_WALKING: 
			vx = -GOOMBA_WALKING_SPEED * dt;
			Died = false;
			break;
		case PARAGOOMBA_WINGED_STATE_JUMP:
			vx = GOOMBA_WALKING_SPEED * dt;
			LevelParaGoomba = PARAGOOMBA_WINGED_LEVEL;
			break;
		case PARAGOOMBA_WINGED_STATE_JUMP_HEIGHT:
			vy = -PARAGOOMBA_JUMP_DEFLECT_HEIGHT_SPEED * dt;
			break;
		case BROWN_PARAGOOMBA_STATE_FLYING:
			break;
	}
}
