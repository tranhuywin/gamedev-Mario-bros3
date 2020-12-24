#include "SenceWorldMap.h"
#include "Game.h"
#include <iostream>
#include <fstream>
#include "Brick.h"
#include "Line.h"
#include "Tube.h"

#define SCENE_SECTION_UNKNOWN			-1
#define SCENE_SECTION_TEXTURES			2
#define SCENE_SECTION_SPRITES			3
#define SCENE_SECTION_ANIMATIONS		4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS			6
#define SCENE_SECTION_TITLE_MAP			7
#define SCENE_SECTION_EFFECT			8
#define SCENE_SECTION_STATUS_BAR		9

#define OBJECT_TYPE_MARIO				0
#define OBJECT_TYPE_BRICK				1
#define OBJECT_TYPE_GOOMBA				2
#define OBJECT_TYPE_KOOPAS				3
#define OBJECT_TYPE_FIRE_BULLET			4
#define OBJECT_TYPE_GROUND				5
#define OBJECT_TYPE_LINE				6
#define OBJECT_TYPE_TUBE				7
#define OBJECT_TYPE_QUESTION_BRICK		8
#define OBJECT_TYPE_WOODEN_BRICK		9
#define OBJECT_TYPE_FIRE_PIRANHA_PLANT	10
#define OBJECT_TYPE_ITEM				11

#define OBJECT_TYPE_PORTAL				50

#define MAX_SCENE_LINE 1024

using namespace std;
SenceWorldMap::SenceWorldMap(int id, LPCWSTR filePath) : CScene(id, filePath)
{
	key_handler = new  CMapSceneKeyHandler(this);
}

void SenceWorldMap::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}
void SenceWorldMap::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}
void SenceWorldMap::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}
void SenceWorldMap::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations* animations = CAnimations::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());

		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);
}
void SenceWorldMap::_ParseSection_TITLE_MAP(string line)
{
	vector<string> tokens = split(line);
	if (tokens.size() < 7) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	wstring filePath_texture = ToWSTR(tokens[1]);
	wstring filePath_data = ToWSTR(tokens[2]);
	int num_row_on_texture = atoi(tokens[3].c_str());
	int num_col_on_textture = atoi(tokens[4].c_str());
	int tileset_width = atoi(tokens[5].c_str());
	int tileset_height = atoi(tokens[6].c_str());
	this->tileMap = new TileMap(ID, filePath_texture.c_str(), filePath_data.c_str(), num_row_on_texture, num_col_on_textture, tileset_width, tileset_height);
}
void SenceWorldMap::_ParseSection_STATUS_BAR(string line)
{
	vector<string> tokens = split(line);
	if (tokens.size() < 4) return; // skip invalid lines

	int SpriteStatusBar = atoi(tokens[0].c_str());	// 80000	80001	80010	80020
	int SpriteCardBar = atoi(tokens[1].c_str());
	int SpriteNumber0 = atoi(tokens[2].c_str());
	int SpritePowerState = atoi(tokens[3].c_str());
	statusBar = new StatusBar(player, SpriteStatusBar, SpriteCardBar, SpriteNumber0, SpritePowerState);
}
void SenceWorldMap::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());

	int ani_set_id = atoi(tokens[3].c_str());

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	CGameObject* obj = NULL;
	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
	{
		if (player != NULL)
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new CMario(x, y);
		player = (CMario*)obj;

		if (CGame::GetInstance()->Getcurrent_scene() == 0)
			player->SetLevel(MARIO_LEVEL_MINI);
		DebugOut(L"[INFO] Player object created!\n");
	} break;
	case OBJECT_TYPE_GROUND:
	{
		float Width = atoi(tokens[4].c_str());
		obj = new Ground(Width);
	}break;
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}
	if (obj != NULL)
	{
		// General object setup
		obj->SetPosition(x, y);
		//obj->SetXYStartLive(x, y);
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

		obj->SetAnimationSet(ani_set);
		objects.push_back(obj);
	}
}
void SenceWorldMap::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
		if (line == "[SPRITES]") {
			section = SCENE_SECTION_SPRITES; continue;
		}
		if (line == "[ANIMATIONS]") {
			section = SCENE_SECTION_ANIMATIONS; continue;
		}
		if (line == "[ANIMATION_SETS]") {
			section = SCENE_SECTION_ANIMATION_SETS; continue;
		}
		if (line == "[OBJECTS]") {
			section = SCENE_SECTION_OBJECTS; continue;
		}
		if (line == "[TITLEMAP]") {
			section = SCENE_SECTION_TITLE_MAP; continue;
		}
		if (line == "[EFFECT]") {
			section = SCENE_SECTION_EFFECT; continue;
		}
		if (line == "[STATUS_BAR]") {
			section = SCENE_SECTION_STATUS_BAR; continue;
		}
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		case SCENE_SECTION_TITLE_MAP: _ParseSection_TITLE_MAP(line); break;
		//case SCENE_SECTION_EFFECT: _ParseSection_EFFECT(line); break;
		case SCENE_SECTION_STATUS_BAR: _ParseSection_STATUS_BAR(line); break;
		}
	}

	f.close();

	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}
bool CheckObjInScreenWorld(LPGAMEOBJECT Obj) {
	float XLeftScreen = CGame::GetInstance()->GetCamPosX() - 48;
	float XRightScreen = CGame::GetInstance()->GetCamPosX() + CGame::GetInstance()->GetScreenWidth() + 48;
	float YTopScreen = CGame::GetInstance()->GetCamPosY() - 48;
	float YBotScreen = CGame::GetInstance()->GetCamPosY() + CGame::GetInstance()->GetScreenHeight() + 48;

	if (Obj->x < XLeftScreen || Obj->x > XRightScreen)
		return false;
	if (Obj->y < YTopScreen || Obj->y > YBotScreen)
		return false;
	return true;
}
void SenceWorldMap::Update(DWORD dt)
{
	vector<LPGAMEOBJECT> coObjects;
	vector<LPGAMEOBJECT> coObjectsItem;

	//Push nhung Obj trong man hinh vao coObjects de xet va cham 
	for (size_t i = 1; i < objects.size(); i++)
	{
		LPGAMEOBJECT e = objects[i];
		if (dynamic_cast<Ground*>(e) || dynamic_cast<Line*>(e) || dynamic_cast<Tube*>(e) || dynamic_cast<Effect*>(e))
			coObjects.push_back(objects[i]);
		else if (CheckObjInScreenWorld(objects[i]))
			coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		//if (CheckObjInScreen(objects[i]))
		coObjectsItem.push_back(objects[i]);
	}
	for (size_t i = 0; i < objectsItem.size(); i++)
	{
		//if (CheckObjInScreen(objects[i]))
		objectsItem[i]->Update(dt, &coObjectsItem);
	}
	// Update Obj is Active in screen
	for (size_t i = 0; i < objects.size(); i++)
	{
		LPGAMEOBJECT e = objects[i];
		if (dynamic_cast<Brick*>(e) || dynamic_cast<CMario*>(e))
		{
			objects[i]->Update(dt, &coObjects);
		}
		else if (CheckObjInScreenWorld(objects[i]))
			objects[i]->Update(dt, &coObjects);
	}
	//player->Update(dt, &coObjects);

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;

	if (player->x < 0)
		player->SetPosition(0, player->y);
	if (player->x > tileMap->GetWidthMap() - MARIO_BIG_BBOX_WIDTH * 2)
		player->SetPosition(tileMap->GetWidthMap() - MARIO_BIG_BBOX_WIDTH * 2, player->y);

	// Update camera to follow mario
	UpdateCammera();
	// Update Status bar
	float XStatusBar = CGame::GetInstance()->GetCamPosX() + 5;
	float YStatusBar = CGame::GetInstance()->GetCamPosY() + CGame::GetInstance()->GetScreenHeight() - 30;
	statusBar->Update(dt, XStatusBar, YStatusBar);
}

void SenceWorldMap::Render()
{
	tileMap->Draw();
	if (CGame::GetInstance()->Getcurrent_scene() == SCENCE_START)
	{
		CSprites::GetInstance()->Get(80003)->Draw(-50.0f, 0);
		CSprites::GetInstance()->Get(80003)->Draw(224.0f, 0);
	}
	for (int i = 1; i < objects.size(); i++)
	{
		LPGAMEOBJECT e = objects[i];
		if (dynamic_cast<Brick*>(e)) {
			objects[i]->Render();
		}
		else if (CheckObjInScreenWorld(objects[i]))
		{
			objects[i]->Render();
		}
	}
	for (size_t i = 0; i < objectsItem.size(); i++)
	{
		objectsItem[i]->Render();
	}
	objects[0]->Render();
	//player->Render();
	//CMario::GetInstance()->Render();
	//CMario::Render();
	statusBar->Render();
}
void SenceWorldMap::UpdateCammera()
{
	CGame* game = CGame::GetInstance();
	float cx, cy;
	int CurSecene = game->Getcurrent_scene();

	player->GetPosition(cx, cy);
	cx -= game->GetScreenWidth() / 2;
	if (cx > tileMap->GetWidthMap() - game->GetScreenWidth() - 16)
		cx = tileMap->GetWidthMap() - game->GetScreenWidth() - 16;
	if (player->IsFlying && cy < (tileMap->GetHeightMap() - game->GetScreenHeight() / 2))
	{
		cy -= game->GetScreenHeight() / 2;
	}
	else if (player->IsLimitFlying && cy < (tileMap->GetHeightMap() - game->GetScreenHeight() - 16))
		cy -= game->GetScreenHeight() / 2;
	else
	{
		if (CurSecene == 1)
			cy = tileMap->GetHeightMap() / 2 + 16 + game->GetScreenHeight() / 4;
		else if (CurSecene == 4)
			cy = tileMap->GetHeightMap() / 2 + 16 + game->GetScreenHeight() / 2;
		if (player->y < 192)
			cy -= game->GetScreenHeight();
	}
	if (cx < 16)
		cx = 16;
	if (cy < 16)
		cy = 16;
	if (CurSecene == 0)
	{
		cx = -50.0f;
	}
	else if (CurSecene == 4)
	{
		cx = 95.0f;
		cy = 8.0f;
	}
	CGame::GetInstance()->SetCamPos(cx, cy);
}
void SenceWorldMap::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];
	objects.clear();
	for (int i = 0; i < objectsItem.size(); i++)
		delete objectsItem[i];
	objectsItem.clear();
	//if(CGame::GetInstance()->Getcurrent_scene() == 4)

	player = NULL;

	delete tileMap;
	tileMap = NULL;
	delete statusBar;
	statusBar = NULL;
	//delete ItemSwitch;
	//ItemSwitch = NULL;
	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CMapSceneKeyHandler::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();
	CMario* mario = ((SenceWorldMap*)scence)->GetPlayer();

	// disable control key when Mario die 
	if (mario->GetState() == MARIO_STATE_DIE) return;
	if (game->IsKeyDown(DIK_DOWN))
	{
		if (mario->GetLevel() != MARIO_LEVEL_MINI)
			mario->SetState(MARIO_STATE_BEND_OVER);
		else
			mario->SetState(MARIO_MINI_STATE_DOWN);
	}
	else if (game->IsKeyDown(DIK_RIGHT))
	{
		if (mario->GetLevel() != MARIO_LEVEL_MINI)
			mario->SetState(MARIO_STATE_WALKING_RIGHT);
		else
			mario->SetState(MARIO_MINI_STATE_RIGHT);
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		if (mario->GetLevel() != MARIO_LEVEL_MINI)
			mario->SetState(MARIO_STATE_WALKING_LEFT);
		else
			mario->SetState(MARIO_MINI_STATE_LEFT);
	}
	else if (game->IsKeyDown(DIK_UP))
	{
		if (mario->GetLevel() == MARIO_LEVEL_MINI)
			mario->SetState(MARIO_MINI_STATE_UP);
	}
	else
		mario->SetState(MARIO_STATE_IDLE);
}
void CMapSceneKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	CMario* mario = ((SenceWorldMap*)scence)->GetPlayer();

	switch (KeyCode)
	{
	case DIK_A:
		mario->Reset();
		break;
	case DIK_SPACE:
		mario->SetState(MARIO_STATE_JUMP);
		break;
	case DIK_Q:
	{
		if (mario->GetLevel() != MARIO_LEVEL_MINI)
			mario->SetState(MARIO_STATE_KILL);
		else {
			mario->SetState(MARIO_MINI_STATE_TELEPORT);
		}
	}
	break;
	case DIK_1:
		mario->SetLevel(MARIO_LEVEL_SMALL);
		break;
	case DIK_2:
		mario->SetPosition(mario->x, mario->y - MARIO_BIG_BBOX_HEIGHT / 2);
		mario->SetLevel(MARIO_LEVEL_BIG);
		break;
	case DIK_3:
		mario->SetPosition(mario->x, mario->y - MARIO_BIG_BBOX_HEIGHT / 3);
		mario->SetLevel(MARIO_LEVEL_RACCOON);
		break;
	case DIK_4:
		mario->SetPosition(mario->x, mario->y - MARIO_BIG_BBOX_HEIGHT / 2);
		mario->SetLevel(MARIO_LEVEL_FIRE);
		break;
	case DIK_0:
		mario->SetPosition(2268, 70);
		break;

	}
}

void CMapSceneKeyHandler::OnKeyUp(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	CMario* mario = ((SenceWorldMap*)scence)->GetPlayer();
	switch (KeyCode)
	{
	case DIK_SPACE:
		mario->SetState(MARIO_STATE_DROP);
		break;
	case DIK_DOWN:
		mario->SetState(MARIO_STATE_STAND);
	case DIK_Q:
		mario->SetState(MARIO_STATE_SKILL_OFF);
		break;
	}
}