#include "Map.h"

TileMap::TileMap(int ID, LPCWSTR filePath_texture, LPCWSTR filePath_data, int NumRowOnTexture, int NumColOnTextture, int TileSetWidth, int TileSetHeight)
{
	id = ID;
	this->filePath_texture = filePath_texture;
	this->filePath_data = filePath_data;

	this->NumRowOnTexture = NumRowOnTexture;
	this->NumColOnTextture = NumColOnTextture;
	this->TileSetWidth = TileSetWidth;
	this->TileSetHeight = TileSetHeight;

	SetNumColOnTilemap();
	LoadFilePath();
	LoadTextureToSprites();
}

TileMap::~TileMap()
{
}
void TileMap::LoadFilePath()
{
	ifstream fs;
	fs.open(filePath_data, ios::in);

	if (fs.fail())
	{
		fs.close();
		return;
	}

	int RowRead = 0, ColRead = 0;
	while (!fs.eof())
	{
		if (ColRead >= NumColOnTilemap)
		{
			ColRead = 0;
			RowRead++;
			NumRowOnTilemap = RowRead;
		}
		fs >> tilemap[RowRead][ColRead];
		ColRead++;
	}
	fs.close();
}

void TileMap::LoadTextureToSprites()
{
	CTextures::GetInstance()->Add(id, filePath_texture, D3DCOLOR_XRGB(255, 0, 255));
	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(id);
	int spriteIDstart = 0;
	for(int i = 0; i < NumRowOnTexture; i++)
		for (int j = 0; j < NumColOnTextture; j++)
		{
			int IDsprite = spriteIDstart + id;
			CSprites::GetInstance()->Add(IDsprite, TileSetWidth * j, TileSetWidth * i, TileSetWidth*(j + 1), TileSetWidth*(i + 1), tex);
			spriteIDstart++;
		}
}
void TileMap::Draw()
{	
	int FirstCol = (int)CGame::GetInstance()->GetCamPosX() / TileSetWidth;
	int LastCol = FirstCol + (CGame::GetInstance()->GetScreenWidth() / TileSetWidth) + 1;

	int FirstRow = (int)CGame::GetInstance()->GetCamPosY() / TileSetHeight;
	int LastRow = FirstRow + (CGame::GetInstance()->GetScreenHeight() / TileSetHeight) + 1;

	for (int currentRow = FirstRow; currentRow <= LastRow; currentRow++)
	{
		for (int currentColumn = FirstCol; currentColumn <= LastCol; currentColumn++)
		{
			float x = TileSetWidth * (currentColumn - FirstCol) + CGame::GetInstance()->GetCamPosX() -(int)(CGame::GetInstance()->GetCamPosX()) % TileSetWidth;
			float y = TileSetHeight * (currentRow - FirstRow) + CGame::GetInstance()->GetCamPosY() - (int)(CGame::GetInstance()->GetCamPosY()) % TileSetHeight;
			CSprites::GetInstance()->Get(tilemap[currentRow][currentColumn] + id)->Draw(x, y);
		}
	}
}
void TileMap::SetNumColOnTilemap()
{
	ifstream fs;
	fs.open(filePath_data, ios::in);

	if (fs.fail())
	{
		fs.close();
		NumColOnTilemap = 0;
		return;
	}
	string Col;
	getline(fs, Col);
	NumColOnTilemap = Col.size() / 2;	// chia khoan trang khoang trang
	fs.close();
}
float TileMap::GetHeightMap()
{
	return NumRowOnTilemap * TileSetHeight;
}

