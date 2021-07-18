#include "ObjCheckFalling.h"
#include "Utils.h"
ObjCheckFalling::ObjCheckFalling()
{
}
void ObjCheckFalling::Update(ULONGLONG dt, vector<LPGAMEOBJECT>* listMapObj)
{
	vy += 0.005f * dt;
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	CGameObject::Update(dt, listMapObj);
	coEvents.clear();
	CalcPotentialCollisions(listMapObj, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
		//isFalling = true;
		mdy += dy;
		if (mdy > 5.0f)
		{
			isFalling = true;
		}
	}
	else
	{
		mdy = 0;
		float min_tx, min_ty, nx = 0, ny = 0;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.1f;

		if (ny != 0)
		{
			vy = 0;
			isFalling = false;
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}

void ObjCheckFalling::Render()
{
	RenderBoundingBox();
}

void ObjCheckFalling::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	b = t + 26;
	r = x + 16;
}