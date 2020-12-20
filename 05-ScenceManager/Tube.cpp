
#include "Tube.h"

void Tube::Render()
{
	//RenderBoundingBox();
	if(NoTube != -1)
		animation_set->at(NoTube)->Render(x, y);
}

void Tube::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + TUBE_BBOX_WIDTH;
	switch ((int)x)
	{
	case(X_TUBE_1):
		b = y + TUBE_BBOX_HIGHT_1;
		NoTube = 0;
		break;
	case(X_TUBE_2):
	{
		b = y + TUBE_BBOX_HIGHT_2;
		NoTube = 1;
	}
		break;
	case(X_TUBE_3):
		b = y + TUBE_BBOX_HIGHT_3;
		NoTube = 0;
		break;
	case(X_TUBE_4):
		b = y + TUBE_BBOX_HIGHT_4;
		break;
	case(X_TUBE_5):
		b = y + TUBE_BBOX_HIGHT_5;
		break;
	case(X_TUBE_6):
		b = y + TUBE_BBOX_HIGHT_6;
		break;
	}
}
