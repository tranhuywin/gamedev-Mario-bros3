#pragma once
#include "GameObject.h"
#include "Game.h"

#define CELL_WIDTH 195
#define CELL_HEIGHT 150

#define MAP_WIDTH	3000
#define MAP_HEIGHT  1000

class Grid
{
	vector<vector<vector<LPGAMEOBJECT>>> listCells;
	vector<vector<vector<int>>> listIdObjects;

public:
	~Grid();
	Grid(LPCWSTR filepatch, vector<LPGAMEOBJECT> list);
	void Resize();
	void ClearGrid(int numsRow, int numsCol);
	void AddObjectToGrid(int id, vector<LPGAMEOBJECT> object);
	void GetGrid(vector<LPGAMEOBJECT>& list);
	//void MakeObjOutOfCam(vector<LPGAMEOBJECT>& list);
};
