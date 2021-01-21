#pragma once

#include <unordered_map>

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>


#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include "Scence.h"
//#include "PlayScence.h"

using namespace std;

#define KEYBOARD_BUFFER_SIZE 1024
#define SCENCE_START			0
#define SCENCE_WORD_MAP_1		1
#define SCENCE_WORD_MAP_1_1		2
#define SCENCE_WORD_MAP_4		3
#define SCENCE_WORD_MAP_4_1		4
//#define SCREEN_WIDTH 1000.0f
class CGame
{
	static CGame * __instance;
	HWND hWnd;									// Window handle

	LPDIRECT3D9 d3d = NULL;						// Direct3D handle
	LPDIRECT3DDEVICE9 d3ddv = NULL;				// Direct3D device object

	LPDIRECT3DSURFACE9 backBuffer = NULL;		
	LPD3DXSPRITE spriteHandler = NULL;			// Sprite helper library to help us draw 2D image on the screen 

	LPDIRECTINPUT8       di;		// The DirectInput object         
	LPDIRECTINPUTDEVICE8 didv;		// The keyboard device 

	BYTE  keyStates[256];			// DirectInput keyboard state buffer 
	DIDEVICEOBJECTDATA keyEvents[KEYBOARD_BUFFER_SIZE];		// Buffered keyboard data

	LPKEYEVENTHANDLER keyHandler;

	float cam_x = 0.0f;
	float cam_y = 0.0f;

	int screen_width;
	int screen_height; 
	int Score = 0;
	int Money = 0;
	int Life = 4;
	int Time = 900;
	int Level = -1;
	int Card_1 = -1, Card_2 =-1, Card_3 = -1;
	bool ReturnWorld;
	unordered_map<int, LPSCENE> scenes;
	int current_scene; 

	void _ParseSection_SETTINGS(string line);
	void _ParseSection_SCENES(string line);

public:
	void InitKeyboard();
	void SetKeyHandler(LPKEYEVENTHANDLER handler) { keyHandler = handler; }
	void Init(HWND hWnd);
	void Draw(float x, float y, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, int alpha = 255);
	int GetScore() { return Score; };
	void SetScore(int Score) { this->Score = Score; }
	int GetLife() { return Life; };
	void SetLife(int Life) { this->Life = Life; }
	int GetTime() { return Time; };
	void SetTime(int Life) { this->Time = Time; }
	int GetMoney() { return Money; }
	void SetMoney(int money) { this->Money = money; }
	int GetCard_1() { return Card_1; };
	int GetCard_2() { return Card_2; };
	int GetCard_3() { return Card_3; };
	bool GetReturnWorld() { return ReturnWorld; };
	void SetReturnWorld(bool ReturnWorld) { this->ReturnWorld = ReturnWorld; };
	void SetCard_1(int Card_1) { this->Card_1 = Card_1; };
	void SetCard_2(int Card_2) { this->Card_2 = Card_2; };
	void SetCard_3(int Card_3) { this->Card_3 = Card_3; };
	int GetLevel() { return Level; };
	void SetLevel(int level) { this->Level = level; };
	int Getcurrent_scene() { return current_scene; }
	int IsKeyDown(int KeyCode);
	int IsKeyUp(int KeyCode);
	void ProcessKeyboard();

	void Load(LPCWSTR gameFile);
	LPSCENE GetCurrentScene() { return scenes[current_scene]; }
	void SwitchScene(int scene_id);
	//void SaveCurrentScene(){return }
	int GetScreenWidth() { return screen_width; }
	int GetScreenHeight() { return screen_height; }
	
	static void SweptAABB(
		float ml,			// move left 
		float mt,			// move top
		float mr,			// move right 
		float mb,			// move bottom
		float dx,			// 
		float dy,			// 
		float sl,			// static left
		float st, 
		float sr, 
		float sb,
		float &t, 
		float &nx, 
		float &ny);

	LPDIRECT3DDEVICE9 GetDirect3DDevice() { return this->d3ddv; }
	LPDIRECT3DSURFACE9 GetBackBuffer() { return backBuffer; }
	LPD3DXSPRITE GetSpriteHandler() { return this->spriteHandler; }

	void SetCamPos(float x, float y) { cam_x = x; cam_y = y; }
	float GetCamPosX() { return cam_x; };
	float GetCamPosY() { return cam_y; };
	static CGame * GetInstance();

	~CGame();
};


