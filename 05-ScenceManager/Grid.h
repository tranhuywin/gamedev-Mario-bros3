#pragma once
#include "GameObject.h"
#include "Game.h"

#define CELL_WIDTH 180
#define CELL_HEIGHT 150

#define MAP_WIDTH	3000
#define MAP_HEIGHT  1000

class Grid
{
	vector<vector<vector<LPGAMEOBJECT>>> listCells;
	vector<vector<vector<int>>> listIdObjects;

public:
	//Grid(int cellWidth = CELL_WIDTH, int cellHeight = CELL_HEIGHT);
	//Grid();
	~Grid();
	Grid(LPCWSTR filepatch);
	void Resize();
	void ClearGrid(int numsRow, int numsCol);
	void ResetGrid(vector<LPGAMEOBJECT> list);

	void AddObjectToGrid(int id, vector<LPGAMEOBJECT> object);

	//void PushGrid(vector<LPGAMEOBJECT> list);
	void GetGrid(vector<LPGAMEOBJECT>& list);
	//void PushGridStart(LPGAMEOBJECT obj, int row, int col);
	void MakeObjOutOfCam(vector<LPGAMEOBJECT>& list);
};
