#include "Map.h"

TileMap::TileMap(int ID, LPCWSTR filePath_texture, LPCWSTR filePath_data, int NumRowOnTexture, int NumColOnTextture, int NumRowOnTilemap, int NumColOnTilemap, int TileSetWidth, int TileSetHeight)
{
	id = ID;

	this->filePath_texture = filePath_texture;
	this->filePath_data = filePath_data;

	this->NumRowOnTexture = NumRowOnTexture;
	this->NumColOnTextture = NumColOnTextture;
	this->NumRowOnTilemap = NumRowOnTilemap;
	this->NumColOnTilemap = NumColOnTilemap;
	this->TileSetWidth = TileSetWidth;
	this->TileSetHeight = TileSetHeight;

	LoadFilePath();
	LoadTextureToSprites();
}

TileMap::~TileMap()
{
}
void TileMap::LoadFilePath()
{
	ifstream fs(filePath_data, ios::in);

	if (fs.fail())
	{
		fs.close();
		return;
	}

	for (int i = 0; i < NumRowOnTilemap; i++)
	{
		for (int j = 0; j < NumColOnTilemap; j++)
			fs >> tilemap[i][j];
	}

	fs.close();
}

void TileMap::LoadTextureToSprites()
{
	//CTextures* texture = CTextures::GetInstance();
	CTextures::GetInstance()->Add(id, filePath_texture, D3DCOLOR_XRGB(255, 0, 255));

	LPDIRECT3DTEXTURE9 texTileMap = CTextures::GetInstance()->Get(id);

	int SpritesID = 0;

	for (UINT i = 0; i < NumRowOnTexture; i++)
	{
		for (UINT j = 0; j < NumColOnTextture; j++)
		{
			int idSpriteTexture = id + SpritesID;
			CSprites::GetInstance()->Add(idSpriteTexture, TileSetWidth * j, TileSetHeight * i, TileSetWidth * (j + 1), TileSetHeight * (i + 1), texTileMap);
			SpritesID++;
		}
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
			float x = TileSetWidth * (currentColumn - FirstCol) + CGame::GetInstance()->GetCamPosX() - (int)(CGame::GetInstance()->GetCamPosX()) % TileSetWidth;
			float y = TileSetHeight * (currentRow - FirstRow) + CGame::GetInstance()->GetCamPosY() - (int)(CGame::GetInstance()->GetCamPosY()) % TileSetHeight;
			CSprites::GetInstance()->Get(tilemap[currentRow][currentColumn] + id)->Draw(x, y);
		}
	}
}
float TileMap::GetHeightMap()
{
	return NumRowOnTilemap * TileSetHeight;
}

