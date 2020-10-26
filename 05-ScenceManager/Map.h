#include"Sprites.h"
#include"Textures.h"
#include"Utils.h"
#include <iostream>
#include <fstream>
#include<string>
#include <sstream>
#include"Game.h"

#define ArrTileMap	500
class TileMap
{
	LPCWSTR filePath_data;
	LPCWSTR filePath_texture;

	int id;
	int NumRowOnTilemap, NumColOnTilemap;
	int NumRowOnTexture, NumColOnTextture;
	int TileSetWidth, TileSetHeight;

	int tilemap[ArrTileMap][ArrTileMap];

public:
	TileMap(int ID, LPCWSTR filePath_texture, LPCWSTR filePath_data, int num_row_on_texture, int num_col_on_textture, int num_row_on_tilemap, int num_col_on_tilemap, int tileset_width, int tileset_height);
	~TileMap();
	void LoadFilePath();
	void LoadTextureToSprites();
	void Draw();
	float GetHeightMap();
};