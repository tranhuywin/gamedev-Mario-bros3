#pragma once

#include "Scence.h"
#include "Mario.h"
#include "Portal.h"
#include "Ground.h"
#include "GameObject.h"
#include "Map.h"
#include "StatusBar.h"


class SenceWorldMap : public CScene
{
public:
	CMario* player;
	TileMap* tileMap;
	StatusBar* statusBar;

	//int word_id;
	//CPortal* current_portal;
	vector<LPGAMEOBJECT> objects;
	//vector<LPGAMEOBJECT> mapPortals;
	vector<LPGAMEOBJECT> objectsItem;
	SenceWorldMap(int id, LPCWSTR filePath);
	CMario* GetPlayer() { return player; }

	void Load();
	void Update(DWORD dt);
	void Render();
	void Unload();
	void UpdateCammera();
	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_TITLE_MAP(string line);
	void _ParseSection_STATUS_BAR(string line);
};

class CMapSceneKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CMapSceneKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};

