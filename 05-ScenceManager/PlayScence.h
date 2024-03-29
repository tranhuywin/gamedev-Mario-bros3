#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scence.h"
#include "GameObject.h"
#include "QuestionBrick.h"
#include "Mario.h"
#include "Goomba.h"
#include "Koopas.h"
#include "Items.h"
#include "Map.h"
#include "StatusBar.h"
#include "Grid.h"

class CPlayScene: public CScene
{
protected: 
	int SpriteEffectStart;
	bool isCammeraMove = false;
	int possitonCammeraInTheSky = 0, hightCammeraStatusBar = 0;
	TileMap *tileMap;
	CMario* player;
	//Grid
	Grid* grid;
	vector <LPGAMEOBJECT> listAllObject;
	StatusBar* statusBar;
	LPGAMEOBJECT ItemSwitch;
	vector<LPGAMEOBJECT> objectsItem;
	vector<LPGAMEOBJECT> objects;
	vector <LPGAMEOBJECT> listWeaponEnemy;
	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_TITLE_MAP(string line);
	void _ParseSection_EFFECT(string line);
	void _ParseSection_STATUS_BAR(string line);
	void _ParseSection_GRID(string line);
	void _ParseSection_CAMERA(string line);
public: 
	float CamX = 0, CamY = 0;
	CPlayScene(int id, LPCWSTR filePath);
	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	void UpdateCammera(ULONGLONG dt);
	CMario * GetPlayer() { return player; } 
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{
public: 
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CPlayScenceKeyHandler(CScene *s) :CScenceKeyHandler(s) {};
};

