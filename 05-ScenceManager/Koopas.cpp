#include "Koopas.h"
#include "Utils.h"
#include <algorithm>
#include <assert.h>
#include "Game.h"
#include "Mario.h"
#include "Line.h"
#include "Ground.h"
#include "Tube.h"
#include "QuestionBrick.h"
#include "Brick.h"

CKoopas::CKoopas(int TypeKoopas)
{
	SetState(KOOPAS_STATE_WALKING);
	this->TypeKoopas = TypeKoopas;
}

void CKoopas::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + KOOPAS_BBOX_WIDTH;

	if (state == KOOPAS_STATE_WALKING)
		bottom = y + KOOPAS_BBOX_HEIGHT;
	else
		bottom = y + KOOPAS_BBOX_HEIGHT_SHELL;
}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	if(!IsCatching)
		vy += KOOPAS_GRAVITY/2 * dt;
	CGameObject::Update(dt);
	if (TypeKoopas == KOOPAS_TYPE_KOOPA_TROOPA_RED || TypeKoopas == KOOPAS_TYPE_KOOPA_TROOPA_GREEN) {
		if (state == KOOPAS_STATE_PREPARE_WAKE_UP)
		{
			if (ShakingLeft)
				x -= 3.0f;
			else
				x += 3.0f;
			ShakingLeft = !ShakingLeft;
		}
		if (GetTickCount() - Sleep_start > KOOPAS_TIME_SLEEP && state == KOOPAS_STATE_SHELL)
		{
			Sleep_start = 0;
			Sleep = 0;
			state = KOOPAS_STATE_PREPARE_WAKE_UP;
			StartPrepareWakeUp();
		}
		if (GetTickCount() - PrepareWakeUp_start > KOOPAS_TIME_WAKE_UP && state == KOOPAS_STATE_PREPARE_WAKE_UP)
		{
			PrepareWakeUp_start = 0;
			PrepareWakeUp = 0;
			y -= 12.0f;
			state = KOOPAS_STATE_WALKING;
			vx = KOOPAS_WALKING_SPEED;
		}
	}
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	if(state != KOOPAS_STATE_DIE)
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
		/*if (nx != 0)
			vx = 0;*/
		if (ny != 0)
			vy = 0;
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<Line*>(e->obj))
			{
				Line* line = dynamic_cast<Line*>(e->obj);
				if (TypeKoopas == KOOPAS_TYPE_KOOPA_TROOPA_RED) {
					float XLeftLine = line->GetX_Left();
					if (vx < 0 && x < XLeftLine && state == KOOPAS_STATE_WALKING) {
						x = XLeftLine; vx = -vx;
					}
					float XRightLine = line->GetX_Right();
					if (vx > 0 && x > XRightLine - KOOPAS_BBOX_WIDTH && state == KOOPAS_STATE_WALKING) {
						x = XRightLine - KOOPAS_BBOX_WIDTH; vx = -vx;
					}
					if (e->ny > 0)
					{
						if (e->ny > 0)		// o duoi len
						{
							vy = vyLine;
							y += dy;
						}
					}
				}
				else if (TypeKoopas == KOOPAS_TYPE_KOOPA_PARATROOPA_GREEN) {
					vy = -KOOPAS_PARATROOPA_WALKING_SPEED * dt;
				}
			}else if (dynamic_cast<Brick*>(e->obj))
			{
				Brick* brick = dynamic_cast<Brick*>(e->obj);
				if (TypeKoopas == KOOPAS_TYPE_KOOPA_TROOPA_RED) {
					float XLeftBrick = brick->x;
					if (vx < 0 && x < XLeftBrick - KOOPAS_BBOX_WIDTH / 2 && state == KOOPAS_STATE_WALKING) {
						x = XLeftBrick; vx = -vx;
					}
					float XRightBrick = brick->x + BRICK_BBOX_WIDTH;
					if (vx > 0 && x > XRightBrick - KOOPAS_BBOX_WIDTH / 3 && state == KOOPAS_STATE_WALKING) {
						x = XRightBrick - KOOPAS_BBOX_WIDTH; vx = -vx;
					}
					if (e->ny > 0)
					{
						if (e->ny > 0)		// o duoi len
						{
							vy = vyLine;
							y += dy;
						}
					}
				}
				else if (TypeKoopas == KOOPAS_TYPE_KOOPA_PARATROOPA_GREEN) {
					vy = -KOOPAS_PARATROOPA_WALKING_SPEED * dt;
				}
			}
			else if (dynamic_cast<Ground*>(e->obj)) {
				if (TypeKoopas == KOOPAS_TYPE_KOOPA_PARATROOPA_GREEN)
					vy = -KOOPAS_PARATROOPA_WALKING_SPEED * dt;
			}
			if (!dynamic_cast<CMario*>(e->obj)) {
				if (e->nx != 0)
				{
					vx = -vx;
				}
				if (dynamic_cast<QuestionBrick*>(e->obj))
				{
					QuestionBrick* questionBrick = dynamic_cast<QuestionBrick*>(e->obj);
					if (questionBrick->GetState() == BRICK_STATE_QUESTION_ON)
						questionBrick->SetState(BRICK_STATE_QUESTION_OFF);
				}
			}
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CKoopas::Render()
{
	int ani = KOOPAS_ANI_WALKING_LEFT_RED;
	if(this->TypeKoopas == KOOPAS_TYPE_KOOPA_TROOPA_RED)
	{
		if (state == KOOPAS_STATE_SHELL) {
			ani = KOOPAS_ANI_SHELL_RED;
			if (TailAttack)
				ani = KOOPAS_ANI_SHELL_TAIL_ATTACK_RED;
		}
		else if (state == KOOPAS_STATE_ROTATORY)
		{
			if (vx > 0)
			{
				ani = KOOPAS_ANI_ROTATORY_RIGHT_RED;
				if (TailAttack)
					ani = KOOPAS_ANI_SHELL_TAIL_ATTACK_ROTATORY_RED;
			}
			else
			{
				ani = KOOPAS_ANI_ROTATORY_LEFT_RED;
				if (TailAttack)
					ani = KOOPAS_ANI_SHELL_TAIL_ATTACK_ROTATORY_RED;
			}
		}
		else if (state == KOOPAS_STATE_PREPARE_WAKE_UP) {
			ani = KOOPAS_ANI_SHELL_TAIL_ATTACK_PREPARE_WAKE_UP_RED;
		}
		else if (vx > 0 && state == KOOPAS_STATE_WALKING) ani = KOOPAS_ANI_WALKING_RIGHT_RED;
		else if (vx <= 0 && state == KOOPAS_STATE_WALKING) ani = KOOPAS_ANI_WALKING_LEFT_RED;

	}
	else if (this->TypeKoopas == KOOPAS_TYPE_KOOPA_TROOPA_GREEN)
	{
		if (state == KOOPAS_STATE_SHELL) {
			ani = KOOPAS_ANI_SHELL_GREEN;
			if (TailAttack)
				ani = KOOPAS_ANI_SHELL_TAIL_ATTACK_GREEN;
		}
		else if (state == KOOPAS_STATE_ROTATORY)
		{
			if (vx > 0)
			{
				ani = KOOPAS_ANI_ROTATORY_RIGHT_GREEN;
				if (TailAttack)
					ani = KOOPAS_ANI_SHELL_TAIL_ATTACK_ROTATORY_GREEN;
			}
			else
			{
				ani = KOOPAS_ANI_ROTATORY_LEFT_GREEN;
				if (TailAttack)
					ani = KOOPAS_ANI_SHELL_TAIL_ATTACK_ROTATORY_GREEN;
			}
		}
		else if (state == KOOPAS_STATE_PREPARE_WAKE_UP) {
			ani = KOOPAS_ANI_SHELL_TAIL_ATTACK_PREPARE_WAKE_UP_GREEN;
		}
		else if (vx > 0 && state == KOOPAS_STATE_WALKING) ani = KOOPAS_ANI_WALKING_RIGHT_GREEN;
		else if (vx <= 0 && state == KOOPAS_STATE_WALKING) ani = KOOPAS_ANI_WALKING_LEFT_GREEN;

	}
	else if (this->TypeKoopas == KOOPAS_TYPE_KOOPA_PARATROOPA_GREEN)
	{
		if (vx > 0)
		{
			if (vy < 0) // nhay
			{
				ani = KOOPA_PARATROOPA_GREEN_ANI_JUMP_RIGHT;
			}
			else
				ani = KOOPA_PARATROOPA_GREEN_ANI_WING_RIGHT;
		}
		else {
			if (vy < 0) // nhay
			{
				ani = KOOPA_PARATROOPA_GREEN_ANI_JUMP_LEFT;
			}
			else
				ani = KOOPA_PARATROOPA_GREEN_ANI_WING_LEFT;
		}
	}
	if (state != KOOPAS_STATE_DIE)
		animation_set->at(ani)->Render(x, y);
	//RenderBoundingBox();
}

void CKoopas::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPAS_STATE_SHELL:
		vx = 0;
		vy = 0;
		StartSleep();
		break;
	case KOOPAS_STATE_ROTATORY:
		vx = KOOPAS_ROTATORY_SPEED;
		break;
	case KOOPAS_STATE_WALKING:
		vx = -KOOPAS_WALKING_SPEED / 3;
		if (this->TypeKoopas == KOOPAS_TYPE_KOOPA_PARATROOPA_GREEN)
			vx = -KOOPAS_PARATROOPA_WALKING_SPEED;
		break;
	case KOOPAS_STATE_DIE:
		vy = KOOPAS_DIE;
		break;
	}
}

void CKoopas::BeCatch(LPGAMEOBJECT mario, float YShell)
{
	float XMario, YMario;
	mario->GetPosition(XMario, YMario);
	if (mario->nx == 1)		// right
	{
		this->SetPosition(XMario + MARIO_RACCOON_BBOX_WIDTH - MARIO_RACCOON_BBOX_TAIL - MARIO_RACCOON_BBOX_WIDTH / 6, YShell);
		if(mario->vx < 0)
			this->SetPosition(XMario - KOOPAS_BBOX_WIDTH + MARIO_RACCOON_BBOX_WIDTH / 6, YShell);
		IsCatching = true;
	}
	if (mario->nx == -1)		//left
	{
		this->SetPosition(XMario - KOOPAS_BBOX_WIDTH + MARIO_RACCOON_BBOX_WIDTH / 6, YShell);
		if(mario->vx > 0)
			this->SetPosition(XMario + MARIO_RACCOON_BBOX_WIDTH - MARIO_RACCOON_BBOX_TAIL - MARIO_RACCOON_BBOX_WIDTH / 6, YShell);
		IsCatching = true;
	}
}
