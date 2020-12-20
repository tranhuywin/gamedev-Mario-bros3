#include "StatusBar.h"
#include "Utils.h"

void StatusBar::DrawNumber(int x, int y, string stringnumb, int maxsize)
{
	stringnumb = string(maxsize - stringnumb.size(), '0').append(stringnumb);
	for (int i = 0; i < stringnumb.size(); i++)
	{
		if (stringnumb[i] == ' ')
			continue;
		int numb;
		for (int j = 0; j < 10; j++)
		{
			numb = 48 + j;     // 48 la gia tri 0 trong bang ma ascii
			if (stringnumb[i] == numb)
			{
				number = CSprites::GetInstance()->Get(80010 + j);
				break;
			}
		}
		number->Draw(x + (i * 8) + 3, y);
	}
}

void StatusBar::Update(DWORD dt, int camX, int camY)
{
	posX = camX;
	posY = camY;
	score = 0000;
	money = 00;
	word = 1;
	life = 1;
	second++;
	if (second == 60)
	{
		time--;
		second = 1;
	}
}
void StatusBar::DrawPMeter()
{
/*	if (Mario->IsFlying)
		CSprites::GetInstance()->Get(80027)->Draw(posX + 52, posY + 7);
	else*/ if (Mario->IsRunning)
	{
		float marioSpeed = abs(Mario->vx);
		DebugOut(L"marioSpeed%f\n", marioSpeed);
		if (marioSpeed < MARIO_MAX_SPEED_RUNNING / 8)
			CSprites::GetInstance()->Get(80020)->Draw(posX + 52 , posY + 7);
		else if (marioSpeed < MARIO_MAX_SPEED_RUNNING / 7)
			CSprites::GetInstance()->Get(80021)->Draw(posX + 52, posY + 7);
		else if (marioSpeed < MARIO_MAX_SPEED_RUNNING / 6)
			CSprites::GetInstance()->Get(80022)->Draw(posX + 52, posY + 7);
		else if (marioSpeed < MARIO_MAX_SPEED_RUNNING / 5)
			CSprites::GetInstance()->Get(80023)->Draw(posX + 52, posY + 7);
		else if (marioSpeed < MARIO_MAX_SPEED_RUNNING / 4)
			CSprites::GetInstance()->Get(80024)->Draw(posX + 52, posY + 7);
		else if (marioSpeed < MARIO_MAX_SPEED_RUNNING / 3)
			CSprites::GetInstance()->Get(80025)->Draw(posX + 52, posY + 7);
		else if (marioSpeed < MARIO_MAX_SPEED_RUNNING / 2)
			CSprites::GetInstance()->Get(80026)->Draw(posX + 52, posY + 7);
		else if (marioSpeed < MARIO_MAX_SPEED_RUNNING)
			CSprites::GetInstance()->Get(80027)->Draw(posX + 52, posY + 7);
	}

}
void StatusBar::Render()
{
	CSprites::GetInstance()->Get(80000)->Draw(posX, posY);

	DrawNumber(posX + 37, posY + 7, to_string(word), 1);// word 1,2,3
	DrawNumber(posX + 29, posY + 15, to_string(life), 1);//so mang con lai
	DrawNumber(posX + 52, posY + 15, to_string(score), 7);//diem so
	DrawNumber(posX + 130, posY + 7, to_string(money), 2);//money
	DrawNumber(posX + 122, posY + 15, to_string(time), 3);//thoi gian
	DrawPMeter();
}

StatusBar::StatusBar(CMario* mario)
{
	Mario = mario;
	time = 900;
}
