#include "Grid.h"
#include "Utils.h"
#include <iostream>
#include <ostream>
#include <fstream>
#include <vector>

Grid::Grid(LPCWSTR filepatch, vector<LPGAMEOBJECT> list) {
	ifstream fs;
	fs.open(filepatch, ios::in);

	listIdObjects.resize(200 );
	for (int i = 0; i <  200; i++)
		listIdObjects[i].resize(200);

	if (fs.fail())
	{
		fs.close();
		return;
	}
	else {
		char str[1024];
		DebugOut(L"[INFO]: Loading Grid\n");
		while (fs.getline(str, 1024))
		{
			string line(str);
			vector<string> tokens = split(line);
			int IDObject = atoi(tokens[0].c_str());
			int ColStart = atoi(tokens[1].c_str());
			int RowStart = atoi(tokens[2].c_str());
			int ColEnd = atoi(tokens[3].c_str());
			int RowEnd = atoi(tokens[4].c_str());
			
			for (int i = RowStart; i <= RowEnd; i++) {
				for (int j = ColStart; j <= ColEnd; j++) {
					listIdObjects[i][j].push_back(IDObject);
				}
			}
			Resize();
			for (int i = RowStart; i < RowEnd; i++) {
				for (int j = ColStart; j < ColEnd; j++) {
					listCells[i][j].push_back(list[IDObject]);
				}
			}

			//Resize();
			////ClearGrid((int)MAP_HEIGHT / CELL_HEIGHT, (int)MAP_WIDTH / CELL_WIDTH);
			//for (int i = 0; i < list.size(); i++)
			//{
			//	for (int row = RowStart; row < RowEnd; row++)
			//		for (int col = ColStart; col < ColEnd; col++)
			//			listCells[row][col].push_back(list[i]);
			//}
		}
		DebugOut(L"[INFO]: Load Grid xong\n");
	}
	
	fs.close();
	Resize();
	ClearGrid((int)MAP_HEIGHT / CELL_HEIGHT, (int)MAP_WIDTH / CELL_WIDTH);

	for (int i = 0; i < list.size(); i++)
	{
		float l, t, r, b;
		list[i]->GetBoundingBox(l, t, r, b);
		int Top = int(t / CELL_HEIGHT);
		int Left = int(l / CELL_WIDTH);
		int Right = ceil(r / CELL_WIDTH);
		int Bottom = ceil(b / CELL_HEIGHT);
		/*if (!list[i]->isdone)*/
		{
			for (int row = Top; row < Bottom; row++)
				for (int col = Left; col < Right; col++)
				listCells[row][col].push_back(list[i]);
		}
	}
}

void Grid::Resize()
{
	int totalCol = MAP_WIDTH / CELL_WIDTH;
	int totalRow = MAP_HEIGHT / CELL_HEIGHT;

	listCells.resize(totalRow + 2);
	for (int i = 0; i < totalRow + 2; i++)
		listCells[i].resize(totalCol + 2);

	for (int i = 0; i < totalRow + 2; i++)
		for (int j = 0; j < totalCol + 2; j++)
			listCells[i][j].clear();

}

void Grid::ClearGrid(int numRow, int numCol)
{
	for (int i = 0; i < numRow; i++)
		for (int j = 0; j < numCol; j++)
			listCells[i][j].clear();
}

void Grid::AddObjectToGrid(int id, vector<LPGAMEOBJECT> object)
{
	int totalCol = MAP_WIDTH / CELL_WIDTH;
	int totalRow = MAP_HEIGHT / CELL_HEIGHT;
	Resize();

	for (int i = 0; i < totalRow; i++) {
		for (int j = 0; j < totalCol; j++) {
			for (int k = 0; k < listIdObjects[i][j].size(); k++)
			{
				if (listIdObjects[i][j][k] == id)
				{
					listCells[i][j].push_back(object[id]);
				}
			}
			
		}
	}
}

void Grid::GetGrid(vector<LPGAMEOBJECT>& list)
{
	int firstCol = (int)(CGame::GetInstance()->GetCamPosX() / CELL_WIDTH);
	int lastCol = (int)((CGame::GetInstance()->GetCamPosX() + CGame::GetInstance()->GetScreenWidth()) / CELL_WIDTH) + 1;
	int firstRow = (int)(CGame::GetInstance()->GetCamPosY() / CELL_HEIGHT);
	int lastRow = (int)((CGame::GetInstance()->GetCamPosY() + CGame::GetInstance()->GetScreenHeight()) / CELL_HEIGHT) + 1;

	for (int i = firstRow; i < lastRow; i++)
	{
		for (int j = firstCol; j < lastCol; j++)
		{
			for (int k = 0; k < listCells[i][j].size(); k++)
			{
				list.push_back(listCells[i][j][k]);
			}
		}
	}
}


