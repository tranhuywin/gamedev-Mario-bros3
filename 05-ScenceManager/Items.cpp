#include "Items.h"
#include "Utils.h"
#include "QuestionBrick.h"
void Items::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + ITEM_BBOX_L;
	top = y + ITEM_BBOX_T;
	if (!Active)
	{
		right = x + ITEM_BBOX_R;
		bottom = y + ITEM_BBOX_B;
	}
	else {
		right = x + ITEM_BBOX_R_ACTIVE;
		bottom = y + ITEM_BBOX_B_ACTIVE;
	}
}

void Items::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!SetPosStart)
	{
		this->X_Start = this->x;
		this->Y_Start = this->y;
		SetPosStart = true;
	}
	CGameObject::Update(dt);
	x += dx;
	y += dy;
	if (Active)
	{
		vy += ITEM_GRAVITY * dt;

		if (IdItem == ITEM_TREE_LEAF)
		{
			vy = 2 * ITEM_GRAVITY * dt;
			if ((this->x - this->X_Start) > ITEM_TREE_LEAF_X_DISTANCE_X_START)
				vx -= ITEM_TREE_LEAF_VX * dt;
			else
				vx += ITEM_TREE_LEAF_VX * dt;
		}
		if (IdItem == ITEM_MONEY) {
			if (y - Y_Start > 0)
			{
				x = -100; y = -100; vy = 0; vx = 0;
				Active = false;
			}
		}
	}
	else {
		vx = vy = 0;
	}
	vector<LPGAMEOBJECT> coEventsResult;
	coEventsResult.clear();
	
	CalCollisions(coObjects, coEventsResult);
	int sizeCo = coEventsResult.size();

	if (sizeCo != 0)
	{
		for (UINT i = 0; i < sizeCo; i++)
		{
			LPGAMEOBJECT e = coEventsResult[i];

			if (dynamic_cast<CMario*>(e)) // if e->obj is Goomba 
			{
				CMario* mario = dynamic_cast<CMario*>(e);
				if (IdItem == ITEM_TREE_LEAF)
				{
					if (mario->GetLevel() == MARIO_LEVEL_RACCOON && vy > 0)
					{
						x = -100; y = -100; vy = 0; vx = 0;
						Active = false;
					}
					if (mario->GetLevel() == MARIO_LEVEL_BIG)
						mario->SetLevel(MARIO_LEVEL_RACCOON);
				}
			}
			if (dynamic_cast<QuestionBrick*>(e))
			{
				QuestionBrick* QBrick = dynamic_cast<QuestionBrick*>(e);
				if (QBrick->GetState() == BRICK_STATE_QUESTION_ON_UP)
				{
					vy = -ITEM_DEFLECT_SPEED * dt;
					Active = true;
				}
			}
		}
	}
}

void Items::Render()
{
	//RenderBoundingBox();
	if(this->Active)
	if(IdItem == ITEM_TREE_LEAF)
		animation_set->at(ITEM_ANI_TREE_LEAF)->Render(x, y);
	else if(IdItem == ITEM_MONEY)
		animation_set->at(ITEM_ANI_MONEY)->Render(x, y);
}

Items::Items(int IdItem)
{
	this->IdItem = IdItem;
	if (this->IdItem == ITEM_TREE_LEAF)
	{

	}
	else if (this->IdItem == ITEM_MONEY)
	{

	}
}

void Items::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case ITEM_STATE_LISTEN:
		vx = 0;
		vy = 0;
		break;
	}
}
