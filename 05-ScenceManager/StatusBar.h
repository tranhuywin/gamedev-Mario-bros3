#pragma once
#include "Sprites.h"
#include "Mario.h"
#include "GameObject.h"
#include <string>
#define SPEED_X_TIRER_1 0.1125f
#define SPEED_X_TIRER_2 0.125f
#define SPEED_X_TIRER_3 0.1375f
#define SPEED_X_TIRER_4 0.15f
#define SPEED_X_TIRER_5 0.1625f
#define SPEED_X_TIRER_6 0.175f
#define SPEED_X_TIRER_7 0.1875f
#define SPEED_X_TIRER_8 0.2f

#define CARD_NONE				-1
#define CARD_MUSHROOM			1
#define CARD_FLOWER				2
#define CARD_STAR				3

#define X_DISTANCE_PMETER		52
#define Y_DISTANCE_PMETER		7
#define X_WORD					37
#define Y_WORD					7
#define X_SCORE					52
#define Y_SCORE					15
#define X_LIFE					37
#define Y_LIFE					15
#define X_TIME					124
#define Y_TIME					15
#define X_MONEY					132
#define Y_MONEY					7
#define X_CARD					193
#define X_CARD_1					0
#define X_CARD_2					24
#define X_CARD_3					48
class StatusBar
{
	LPSPRITE number;
	float posX;
	float posY;
	int TotalScore;
	int TotalMoney;
	int WordMap;
	int Life;
	int Time;
	int Second;
	int SpriteStatusBar, SpriteCardBar, SpriteNumber0, SpritePowerState, SpriteCard;
	void DrawNumber(float x, float y, string stringnumb, int maxsize);
	void DrawPMeter();
	void DrawCard();
public:
	void Update(DWORD dt, float camX, float camY);
	void Render();
	CMario* Mario;
	StatusBar(CMario* mario, int SpriteStatusBar, int SpriteCardBar, int SpriteNumber0, int SpritePowerState, int SpriteCard);
	void PlusScore(int Score);
	//~StatusBar();
};

