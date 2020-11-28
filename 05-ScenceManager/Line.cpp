#include "Line.h"
void Line::Render()
{
}

void Line::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	b = y + LINE_BBOX_HEIGHT;
	X_Left = l;
	switch ((int)x)
	{
	case X_LINE_1:
		r = x + LINE_BBOX_WIDTH_1;
		break;
	case X_LINE_2:
		r = x + LINE_BBOX_WIDTH_2;
		break;
	case X_LINE_3:
		r = x + LINE_BBOX_WIDTH_3;
		break;
	case X_LINE_4:
		r = x + LINE_BBOX_WIDTH_4;
		break;
	case X_LINE_5:
		r = x + LINE_BBOX_WIDTH_5;
		break;
	case X_LINE_6:
		r = x + LINE_BBOX_WIDTH_6;
		break;
	case X_LINE_7:
		r = x + LINE_BBOX_WIDTH_7;
		break;
	case X_LINE_8:
		r = x + LINE_BBOX_WIDTH_8;
		break;
	case X_LINE_9:
		r = x + LINE_BBOX_WIDTH_9;
		break;
	case X_LINE_10:
		r = x + LINE_BBOX_WIDTH_10;
		break;
	case X_LINE_11:
		r = x + LINE_BBOX_WIDTH_11;
		break;
	case X_LINE_12:
		r = x + LINE_BBOX_WIDTH_12;
		break;
	}
	X_Right = r;
}