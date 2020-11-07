#include "Koopas.h"
#include "Utils.h"
#include <algorithm>
#include <assert.h>
#include "Game.h"
#include "Mario.h"

CKoopas::CKoopas()
{
	SetState(KOOPAS_STATE_WALKING);
}

void CKoopas::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + KOOPAS_BBOX_WIDTH;

	if (state == KOOPAS_STATE_SHELL || state == KOOPAS_STATE_ROTATORY)
		bottom = y + KOOPAS_BBOX_HEIGHT_SHELL;
	else
		bottom = y + KOOPAS_BBOX_HEIGHT;
}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	vy = 0.01f * dt;
	CGameObject::Update(dt);
	if (vx < 0 && x < 0) {
		x = 0; vx = -vx;
	}

	if (vx > 0 && x > 290) {
		x = 290; vx = -vx;
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

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;
		if (nx != 0)
			vx = 0;
		if (ny != 0)
			vy = 0;
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CKoopas::Render()
{
	int ani = KOOPAS_ANI_WALKING_LEFT;
	if (state == KOOPAS_STATE_SHELL) {
		ani = KOOPAS_ANI_SHELL;
	}
	else if (state == KOOPAS_STATE_ROTATORY)
	{
		if (vx > 0)
			ani = KOOPAS_ANI_ROTATORY_RIGHT;
		else
			ani = KOOPAS_ANI_ROTATORY_LEFT; 
	}
	else if (vx > 0 && state == KOOPAS_STATE_WALKING) ani = KOOPAS_ANI_WALKING_RIGHT;
	else if (vx <= 0 && state == KOOPAS_STATE_WALKING) ani = KOOPAS_ANI_WALKING_LEFT;

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
		break;
	case KOOPAS_STATE_ROTATORY:
		vx = 0.3f;
		break;
	case KOOPAS_STATE_WALKING:
		vx = KOOPAS_WALKING_SPEED;
		break;
	case KOOPAS_STATE_DIE:
		vy = 0.1f;
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
	}
	if (mario->nx == -1)		//left
	{
		this->SetPosition(XMario - KOOPAS_BBOX_WIDTH + MARIO_RACCOON_BBOX_WIDTH / 6, YShell);
		if(mario->vx > 0)
			this->SetPosition(XMario + MARIO_RACCOON_BBOX_WIDTH - MARIO_RACCOON_BBOX_TAIL - MARIO_RACCOON_BBOX_WIDTH / 6, YShell);
	}
}
