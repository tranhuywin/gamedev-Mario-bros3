#include <iostream>
#include <fstream>

#include "PlayScence.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "Ground.h"
#include "FireBullet.h"
#include "Line.h"
#include "Tube.h"
#include "WoodenBrick.h"
#include "Brick.h"
#include "VenusFireTrap.h"
#include "Items.h";
#include "BulletPiranhaPlant.h"
#include "Tree.h"
#include "FlyingWood.h"
#include "Brothers.h"
#include "MusicNote.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath):	CScene(id, filePath)
{
	key_handler = new CPlayScenceKeyHandler(this);
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/

#define SCENE_SECTION_UNKNOWN			-1
#define SCENE_SECTION_TEXTURES			2
#define SCENE_SECTION_SPRITES			3
#define SCENE_SECTION_ANIMATIONS		4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS			6
#define SCENE_SECTION_TITLE_MAP			7
#define SCENE_SECTION_EFFECT			8
#define SCENE_SECTION_STATUS_BAR		9
#define SCENE_SECTION_GRID				10
#define SCENE_SECTION_CAMERA			11

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
#define OBJECT_TYPE_TREE				12
#define OBJECT_TYPE_CARD				13
#define OBJECT_TYPE_FLYING_WOOD			14
#define OBJECT_TYPE_BROTHER				15
#define OBJECT_TYPE_MUSIC_NOTE			16

#define OBJECT_TYPE_PORTAL				50

#define MAX_SCENE_LINE 1024

#define CAMERA_ON_PLATFORM 64.0f
#define CAMERA_MOVE_VX		0.055f
#define CAMERA_X_START_MAP	48.0f
#define CAMERA_Y_START_MAP 32.0f
#define CAMERA_X_WORLD_1_1	95.0f
#define CAMERA_Y_WORLD_1_1	8.0f
#define CAMERA_Y_WORLD_4_1  6.0f

#define SCREEN_BORDER		0.0f
#define STATUS_BAR_MARGIN_LEFT	5.0f
#define STATUS_BAR_MARGIN_TOP	35.0f
#define SCREEN_BORDER_RIGHT 16.0f
void CPlayScene::_ParseSection_TEXTURES(string line)
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
void CPlayScene::_ParseSection_SPRITES(string line)
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
void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i+1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}
void CPlayScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations *animations = CAnimations::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());
		
		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);
}
void CPlayScene::_ParseSection_TITLE_MAP(string line)
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
void CPlayScene::_ParseSection_EFFECT(string line) {
	vector<string> tokens = split(line);
	if (tokens.size() < 1) return;
	SpriteEffectStart = atoi(tokens[0].c_str());
}
void CPlayScene::_ParseSection_STATUS_BAR(string line) {
	vector<string> tokens = split(line);
	if (tokens.size() < 4) return; // skip invalid lines

	int SpriteStatusBar = atoi(tokens[0].c_str());	// 80000	80001	80010	80020	80030
	int SpriteCardBar = atoi(tokens[1].c_str());
	int SpriteNumber0 = atoi(tokens[2].c_str());
	int SpritePowerState = atoi(tokens[3].c_str());
	int SpriteCard = atoi(tokens[4].c_str());
	statusBar = new StatusBar(player, SpriteStatusBar, SpriteCardBar, SpriteNumber0, SpritePowerState, SpriteCard);
}
void CPlayScene::_ParseSection_GRID(string line) {
	vector<string> tokens = split(line);
	wstring filePath = ToWSTR(tokens[0]);
	grid = new Grid(filePath.c_str(), listAllObject);
}
void CPlayScene::_ParseSection_CAMERA(string line) {
	vector<string> tokens = split(line);
	if (tokens.size() < 3) return; // skip invalid lines

	int Move = atoi(tokens[0].c_str());
	if (Move == 1)
		isCammeraMove = true;
	else
		isCammeraMove = false;
	possitonCammeraInTheSky = atoi(tokens[1].c_str());
	hightCammeraStatusBar = atoi(tokens[2].c_str());
}
/*
	Parse a line in section [OBJECTS] 
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());

	int ani_set_id = atoi(tokens[3].c_str());

	CAnimationSets * animation_sets = CAnimationSets::GetInstance();

	CGameObject *obj = NULL;	
	CGameObject* objItem = NULL;
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

			if (CGame::GetInstance()->Getcurrent_scene() == SCENCE_START)
				player->SetLevel(MARIO_LEVEL_MINI);
			DebugOut(L"[INFO] Player object created!\n");
		} break;
		case OBJECT_TYPE_BRICK: obj = new Brick(ItemSwitch); break;
		case OBJECT_TYPE_FIRE_BULLET: obj = new FireBullet(); break;
		case OBJECT_TYPE_QUESTION_BRICK: obj = new QuestionBrick(); break;
		case OBJECT_TYPE_WOODEN_BRICK: 
		{
			if (tokens.size() == 5)
			{
				int typeItem = atoi(tokens[4].c_str());
				obj = new WoodenBrick(typeItem);
			}
			else
				obj = new WoodenBrick(); 
		}break;
		case OBJECT_TYPE_FLYING_WOOD: obj = new FlyingWood(); break;
		case OBJECT_TYPE_MUSIC_NOTE: 
		{
			int typeNote = atoi(tokens[4].c_str());
			obj = new MusicNote(typeNote); break;
		}
		case OBJECT_TYPE_BROTHER: 
		{
			int IDType = atoi(tokens[4].c_str());
			int AniWeapon = atoi(tokens[5].c_str());
			BoomerangOfBrother* Boomerang1 = new BoomerangOfBrother(AniWeapon);
			//listAllObject.push_back(Boomerang1);
			listWeaponEnemy.push_back(Boomerang1);
			objects.push_back(Boomerang1);
			BoomerangOfBrother* Boomerang2 = new BoomerangOfBrother(AniWeapon);
			//listAllObject.push_back(Boomerang2);
			listWeaponEnemy.push_back(Boomerang2);
			objects.push_back(Boomerang2);
			obj = new Brothers(IDType, Boomerang1, Boomerang2, player);
			
		}break;
		case OBJECT_TYPE_TREE:
		{
			int IdObj = atoi(tokens[4].c_str());
			obj = new Tree(IdObj);
		} break;
		
		case OBJECT_TYPE_TUBE: 
		{
			float Height = atoi(tokens[4].c_str());
			int IDAni = atoi(tokens[5].c_str());
			obj = new Tube(Height, IDAni);
		} break;
		case OBJECT_TYPE_LINE: 
		{
			float Width = atoi(tokens[4].c_str());
			obj = new Line(Width);
		}break;
		case OBJECT_TYPE_GROUND: 
		{
			float Width = atoi(tokens[4].c_str());
			obj = new Ground(Width);
		}break;
		case OBJECT_TYPE_KOOPAS: 
		{
			int TypeKoopas = atoi(tokens[4].c_str());
			obj = new CKoopas(TypeKoopas);
		}break;
		case OBJECT_TYPE_ITEM:
		{
			int IdItem = atoi(tokens[4].c_str());
			if (IdItem != ITEM_SWITCH)
			{
				objItem = new Items(IdItem, SpriteEffectStart);
			}
			else
			{
				obj = new Items(IdItem, SpriteEffectStart);
			}
			
		}break;
		case OBJECT_TYPE_GOOMBA:
		{
			int TypeGoomba = atoi(tokens[4].c_str());
			obj = new CGoomba(TypeGoomba);
		}break;
		case OBJECT_TYPE_FIRE_PIRANHA_PLANT:
		{
			int aniPlant = atoi(tokens[4].c_str());
			int TypeVenusFireTrap = atoi(tokens[5].c_str());
			BulletPiranhaPlant* objBullet = new BulletPiranhaPlant();
			LPANIMATION_SET aniPlant_set = animation_sets->Get(aniPlant);
			objBullet->SetAnimationSet(aniPlant_set);

			listWeaponEnemy.push_back(objBullet);

			//objects.push_back(objBullet);
			obj = new VenusFireTrap(player, objBullet, TypeVenusFireTrap);
			//listAllObject.push_back(objBullet);
		}break;
		case OBJECT_TYPE_PORTAL:
		{
			float r = atof(tokens[4].c_str());
			float b = atof(tokens[5].c_str());
			int scene_id = atoi(tokens[6].c_str());
			int playerMove = atof(tokens[7].c_str());
			obj = new CPortal(x, y, r, b, scene_id, playerMove);
		}break;
		default:
			DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
			return;
		}
	if (obj != NULL)
	{
		obj->SetPosition(x, y);
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

		obj->SetAnimationSet(ani_set);
		objects.push_back(obj);
		if(!dynamic_cast<CMario*>(obj))
			listAllObject.push_back(obj);
	}
	if (objItem != NULL)
	{
		objItem->SetPosition(x, y);
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

		objItem->SetAnimationSet(ani_set);
		objectsItem.push_back(objItem);
		listAllObject.push_back(objItem);
	}
		
}

void CPlayScene::Load()
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
			section = SCENE_SECTION_SPRITES; continue; }
		if (line == "[ANIMATIONS]") { 
			section = SCENE_SECTION_ANIMATIONS; continue; }
		if (line == "[ANIMATION_SETS]") { 
			section = SCENE_SECTION_ANIMATION_SETS; continue; }
		if (line == "[OBJECTS]") { 
			section = SCENE_SECTION_OBJECTS; continue; }
		if (line == "[TITLEMAP]") {
			section = SCENE_SECTION_TITLE_MAP; continue;
		}
		if (line == "[EFFECT]") {
			section = SCENE_SECTION_EFFECT; continue;
		}
		if (line == "[STATUS_BAR]") {
			section = SCENE_SECTION_STATUS_BAR; continue;
		}
		if (line == "[GRID]") {
			section = SCENE_SECTION_GRID; continue;
		}
		if (line == "[CAMERA]") {
			section = SCENE_SECTION_CAMERA; continue;
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
			case SCENE_SECTION_EFFECT: _ParseSection_EFFECT(line); break;
			case SCENE_SECTION_STATUS_BAR: _ParseSection_STATUS_BAR(line); break;
			case SCENE_SECTION_GRID: _ParseSection_GRID(line); break;
			case SCENE_SECTION_CAMERA: _ParseSection_CAMERA(line); break;
		}
	}

	f.close();

	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));
	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}
bool CheckObjInScreen(LPGAMEOBJECT Obj) {
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
void CPlayScene::Update(DWORD dt)
{
	vector<LPGAMEOBJECT> coObjects;
	vector<LPGAMEOBJECT> coObjectsItem;
	vector<LPGAMEOBJECT> ObjectsInGrid;
	if (CGame::GetInstance()->Getcurrent_scene() != SCENCE_START)
	{
		grid->GetGrid(ObjectsInGrid);
		for (int i = 0; i < ObjectsInGrid.size(); i++)
		{
			if (!dynamic_cast<Items*>(ObjectsInGrid[i]))
				coObjects.push_back(ObjectsInGrid[i]);
		}
		
		for (int i = 0; i < listWeaponEnemy.size(); i++) {
			coObjects.push_back(listWeaponEnemy[i]);
		}


		for (int i = 0; i < ObjectsInGrid.size(); i++)
		{
			if (!dynamic_cast<Items*>(ObjectsInGrid[i]))
				ObjectsInGrid[i]->Update(dt, &coObjects);
			else
			{
				coObjects.push_back(player);
				ObjectsInGrid[i]->Update(dt, &coObjects);
			}
		}
		player->Update(dt, &coObjects);
	}
	else {
		for (size_t i = 1; i < objects.size(); i++)
		{
			coObjects.push_back(objects[i]);
		}
		player->Update(dt, &coObjects);

	}

	
	if (player == NULL) return; 

	if (player->x < 0)
		player->SetPosition(0, player->y);
	if (player->x > tileMap->GetWidthMap() - MARIO_BIG_BBOX_WIDTH)
		player->SetPosition(tileMap->GetWidthMap()- MARIO_BIG_BBOX_WIDTH, player->y);

	// Update camera 
	UpdateCammera(dt);
	// Update Status bar
	float XStatusBar = CGame::GetInstance()->GetCamPosX() + STATUS_BAR_MARGIN_LEFT;
	float YStatusBar = CGame::GetInstance()->GetCamPosY() + CGame::GetInstance()->GetScreenHeight() - STATUS_BAR_MARGIN_TOP;
	statusBar->Update(dt, XStatusBar, YStatusBar);
}

void CPlayScene::Render()
{
	vector<LPGAMEOBJECT> ObjectsInGrid;
	vector<LPGAMEOBJECT> Objects;
	tileMap->Draw();

	if (CGame::GetInstance()->Getcurrent_scene() != SCENCE_START) {
		grid->GetGrid(ObjectsInGrid);
		
		for (int i = 0; i < ObjectsInGrid.size(); i++)
		{
			ObjectsInGrid[i]->Render();
		}
	}
	else {
		for (int i = 0; i < objects.size(); i++)
		{
			objects[i]->Render();
		}
	}

	player->Render();
			
	if (dynamic_cast<Tube*>(objects[0])) {
		objects[0]->Render();	// cong che portal
	}
	if (dynamic_cast<Tube*>(objects[1])) {
		objects[1]->Render();	// cong che portal
	}
	statusBar->Render();
}


void CPlayScene::Unload()
{
	CamX = 0; CamY = 0;
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];
	objects.clear();
	for (int i = 0; i < objectsItem.size(); i++)
		delete objectsItem[i];
	objectsItem.clear();
	/*for (int i = 0; i < listAllObject.size(); i++)
		delete listAllObject[i];*/
	listAllObject.clear();
	listWeaponEnemy.clear();
	
	//delete grid;
	grid = NULL;
	player = NULL;
	delete tileMap;
	tileMap = NULL;
	delete statusBar;
	statusBar = NULL;
	delete ItemSwitch;
	ItemSwitch = NULL;
	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CPlayScene::UpdateCammera(DWORD dt)
{
	CGame* game = CGame::GetInstance();
	//float CamX, CamY;
	int CurSecene = game->Getcurrent_scene();

	if (!isCammeraMove)
	{
		player->GetPosition(CamX, CamY);
		CamX -= game->GetScreenWidth() / 2;
		
		if (player->IsFlying && CamY < (tileMap->GetHeightMap() - game->GetScreenHeight() / 2))			// camY theo bay
			CamY -= game->GetScreenHeight() / 2;
		else if (player->IsLimitFlying && CamY < (tileMap->GetHeightMap() - game->GetScreenHeight()))  // camY theo rot
			CamY -= game->GetScreenHeight() / 2;
		else																							// camY theo di bo
		{
			CamY = tileMap->GetHeightMap() -( game->GetScreenHeight() - hightCammeraStatusBar);
			if (player->y < possitonCammeraInTheSky)
				CamY -= game->GetScreenHeight();
		}
		// gioi han camX
		if (CamX < 0.0f)
			CamX = 0.0f;
		if (CamX > tileMap->GetWidthMap() - game->GetScreenWidth())
			CamX = tileMap->GetWidthMap() - game->GetScreenWidth();
		// gioi han camY
		if (CamY < 0.0f)
			CamY = 0.0f;
		//cam of scence start 
		if (CurSecene == SCENCE_START)
		{
			CamX = -CAMERA_X_START_MAP;
			CamY = -CAMERA_Y_START_MAP;
		}
	}
	else
	{
		CamY = tileMap->GetHeightMap() - (game->GetScreenHeight() - hightCammeraStatusBar);
		if ((player->y - game->GetScreenHeight() / 2) <= CamY)			// camY theo bay
			CamY = (player->y - game->GetScreenHeight() / 2);

		CamX += CAMERA_MOVE_VX * dt;
		if (CamX > tileMap->GetWidthMap() - game->GetScreenWidth() - SCREEN_BORDER_RIGHT)
			CamX = tileMap->GetWidthMap() - game->GetScreenWidth() - SCREEN_BORDER_RIGHT;
		if (player->x < CamX)
			player->SetPosition(CamX, player->y);
		if (player->x > CamX + game->GetScreenWidth() - SCREEN_BORDER_RIGHT)
			player->SetPosition(CamX + game->GetScreenWidth() - SCREEN_BORDER_RIGHT, player->y);
		// gioi han camY
		if (CamY < 0.0f)
			CamY = 0.0f;
	}
	CGame::GetInstance()->SetCamPos(CamX, CamY);
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	CMario *mario = ((CPlayScene*)scence)->GetPlayer();

	switch (KeyCode)
	{
	//case DIK_A: 
	//	mario->Reset();
	//	break;
	case DIK_S:
		mario->SetState(MARIO_STATE_JUMP);
		break;
	case DIK_A:
		{
			if(mario->GetLevel() != MARIO_LEVEL_MINI)
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
		mario->SetPosition(mario->x, mario->y - MARIO_BIG_BBOX_HEIGHT/2);
		mario->SetLevel(MARIO_LEVEL_BIG);
		break;
	case DIK_3:
		mario->SetPosition(mario->x, mario->y - MARIO_BIG_BBOX_HEIGHT/2);
		mario->SetLevel(MARIO_LEVEL_RACCOON);
		break;
	case DIK_4:
		mario->SetPosition(mario->x, mario->y - MARIO_BIG_BBOX_HEIGHT/2);
		mario->SetLevel(MARIO_LEVEL_FIRE);
		break;
	case DIK_0:
		mario->SetPosition(2268, 70);
		break;
	case DIK_9:
		mario->SetPosition(2010, 74);
		((CPlayScene*)scence)->CamX = 1800;
		break;
	case DIK_8:
		mario->SetPosition(988, 130);
		((CPlayScene*)scence)->CamX = 810;
		break;
	case DIK_7:
		mario->SetPosition(653, 94);
		((CPlayScene*)scence)->CamX = 550;
		break;
	}
}

void CPlayScenceKeyHandler::OnKeyUp(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	CMario* mario = ((CPlayScene*)scence)->GetPlayer();
	switch (KeyCode)
	{
	case DIK_S:
		mario->SetState(MARIO_STATE_DROP);
		break;
	case DIK_DOWN:
		mario->SetState(MARIO_STATE_STAND);
	case DIK_A:
		mario->SetState(MARIO_STATE_SKILL_OFF);
		break;
	}
}

void CPlayScenceKeyHandler::KeyState(BYTE *states)
{
	CGame *game = CGame::GetInstance();
	CMario *mario = ((CPlayScene*)scence)->GetPlayer();
	
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
		else {
			mario->SetState(MARIO_STATE_UP);
		}
	}
	else
		mario->SetState(MARIO_STATE_IDLE);
}