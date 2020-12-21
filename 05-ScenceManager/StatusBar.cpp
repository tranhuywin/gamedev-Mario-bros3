#include "StatusBar.h"
#include "Utils.h"

void StatusBar::DrawNumber(int x, int y, string stringnumb, int maxsize)
{
	stringnumb = string(maxsize - stringnumb.size(), '0').append(stringnumb);
	for (int i = 0; i < stringnumb.size(); i++)
	{
		int numb; // 48 la gia tri 0 trong bang ma ascii
		for (int j = 0; j < 10; j++)
		{
			numb = 48 + j;
			if (stringnumb[i] == numb)
			{
				number = CSprites::GetInstance()->Get(SpriteNumber0 + j);
				break;
			}
		}
		number->Draw(x + (i * 8), y);
	}
}

void StatusBar::Update(DWORD dt, int camX, int camY)
{
	posX = camX;
	posY = camY;
	TotalScore = CGame::GetInstance()->GetScore();
	TotalMoney = 00;
	WordMap = 1;
	Life = CGame::GetInstance()->GetLife();
	Second++;
	if (Second == 60)
	{
		Time--;
		Second = 1;
	}
}
void StatusBar::DrawPMeter()
{
	float PosXPMeter = posX + X_DISTANCE_PMETER, PosYPMeter = posY + Y_DISTANCE_PMETER;
	if (Mario->IsFlying)
		CSprites::GetInstance()->Get(SpritePowerState + 7)->Draw(PosXPMeter, PosYPMeter);
	else if (Mario->IsRunning)
	{
		float marioSpeed = abs(Mario->vx);
		//DebugOut(L"marioSpeed%f\n", marioSpeed);
		if (marioSpeed < SPEED_X_TIRER_1)
			CSprites::GetInstance()->Get(SpritePowerState)->Draw(PosXPMeter, PosYPMeter);
		else if (marioSpeed < SPEED_X_TIRER_2)
			CSprites::GetInstance()->Get(SpritePowerState + 1)->Draw(PosXPMeter, PosYPMeter);
		else if (marioSpeed < SPEED_X_TIRER_3)
			CSprites::GetInstance()->Get(SpritePowerState + 2)->Draw(PosXPMeter, PosYPMeter);
		else if (marioSpeed < SPEED_X_TIRER_4)
			CSprites::GetInstance()->Get(SpritePowerState + 3)->Draw(PosXPMeter, PosYPMeter);
		else if (marioSpeed < SPEED_X_TIRER_5)
			CSprites::GetInstance()->Get(SpritePowerState + 4)->Draw(PosXPMeter, PosYPMeter);
		else if (marioSpeed < SPEED_X_TIRER_6)
			CSprites::GetInstance()->Get(SpritePowerState + 5)->Draw(PosXPMeter, PosYPMeter);
		else if (marioSpeed < SPEED_X_TIRER_7)
			CSprites::GetInstance()->Get(SpritePowerState + 6)->Draw(PosXPMeter, PosYPMeter);
		else// if (marioSpeed < SPEED_X_TIRER_8)
			CSprites::GetInstance()->Get(SpritePowerState + 7)->Draw(PosXPMeter, PosYPMeter);
	}
}
void StatusBar::Render()
{
	//BackGound
	CSprites::GetInstance()->Get(SpriteStatusBar + 2)->Draw(posX - 5, posY - 3);
	// status
	CSprites::GetInstance()->Get(SpriteStatusBar)->Draw(posX, posY);
	//card
	CSprites::GetInstance()->Get(SpriteCardBar)->Draw(posX + X_CARD, posY);

	DrawNumber(posX + X_WORD, posY + Y_WORD, to_string(WordMap), 1);
	DrawNumber(posX + X_LIFE, posY + Y_LIFE, to_string(Life), 1);
	DrawNumber(posX + X_SCORE, posY + Y_SCORE, to_string(TotalScore), 7);
	DrawNumber(posX + X_MONEY, posY + Y_MONEY, to_string(TotalMoney), 2);
	DrawNumber(posX + X_TIME, posY + Y_TIME, to_string(Time), 3);
	DrawPMeter();
}

StatusBar::StatusBar(CMario* mario, int SpriteStatusBar, int SpriteCardBar, int SpriteNumber0, int SpritePowerState)
{
	Mario = mario;
	Time = CGame::GetInstance()->GetTime();
	this->SpriteStatusBar = SpriteStatusBar;
	this->SpriteCardBar = SpriteCardBar;
	this->SpriteNumber0 = SpriteNumber0;
	this->SpritePowerState = SpritePowerState;
}

void StatusBar::PlusScore(int Score)
{
	this->TotalScore += Score;
}
