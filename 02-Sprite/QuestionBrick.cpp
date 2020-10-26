#include "Game.h"
#include "QuestionBrick.h"

CQuestionBrick::CQuestionBrick(float x, float y) :CGameObject(x, y) {}

void CQuestionBrick::Update(DWORD dt)
{
	int BackBufferWidth = CGame::GetInstance()->GetBackBufferWidth();
}

void CQuestionBrick::Render()
{
	LPANIMATION ani;

	//[RED FLAG][TODO]: Student needs to think about how to associate this animation/asset to Mario!!
	ani = CAnimations::GetInstance()->Get(510);

	ani->Render(x, y);
}