#include "Effect.h"

void Effect::Update(ULONGLONG dt)
{
	Vy -= SPEED_VY * dt;
	PosY += Vy;
	if (Vy < -SPEED_VY_MAX)
 		StopDraw = false;
}

void Effect::Render()
{
	if(StopDraw)
		CSprites::GetInstance()->Get(AniEffect)->Draw(PosX, PosY);
}

Effect::Effect(float PosX, float PosY, int Ani)
{
	this->PosX = PosX;
	this->PosY = PosY;
	this->AniEffect = Ani;
}
