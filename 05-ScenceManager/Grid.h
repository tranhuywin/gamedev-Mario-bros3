#pragma once

#include <vector>
#include "GameObject.h"
class Grid
{
	vector<LPGAMEOBJECT> objects;
	float XGrid, YGrid;
	float Height, Width;
public:
	Grid(float xGrid, float yGrid, float Height, float Width);
	~Grid();

	void Add(LPGAMEOBJECT object);
	LPGAMEOBJECT Move(LPGAMEOBJECT object); // x, y là toạ độ mới của unit, vì vậy lúc này x, y của unit lưu vị trí cũ
	void Get(D3DXVECTOR3 camPosition, vector<LPGAMEOBJECT>*  listUnits); // lấy tất cả các Unit* nằm trong vùng viewport để Update và Render

	void Out();
};

