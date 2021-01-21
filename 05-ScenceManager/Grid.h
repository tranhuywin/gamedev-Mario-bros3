﻿#pragma once
#include "GameObject.h"
#include "Game.h"

#define CELL_WIDTH 250
#define CELL_HEIGHT 250

#define MAP_WIDTH	2816
#define MAP_HEIGHT  1000


class CGrid
{
	vector<vector<vector<LPGAMEOBJECT>>> listCells;

public:
	CGrid(int cellWidth = CELL_WIDTH, int cellHeight = CELL_HEIGHT);
	~CGrid();
	void Resize();
	void ClearGrid(int numsRow, int numsCol);
	void ResetGrid(vector<LPGAMEOBJECT> list);
	void renderGrid();
	//void PushGrid(vector<LPGAMEOBJECT> list);
	void GetGrid(vector<LPGAMEOBJECT>& list);
	//void PushGridStart(LPGAMEOBJECT obj, int row, int col);
	void MakeObjOutOfCam(vector<LPGAMEOBJECT>& list);
};
