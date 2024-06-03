#pragma once

#include <Windows.h>
#include "mouse.h"
#include <d2d1.h>
#include "game_system.h"
#include "ui_manager.h"
#include "audio_manager.h"

#pragma comment (lib, "d2d1.lib")

enum GameState {
	Start,
	InGame,
	Result
};

class Window {

private:

	/*=================
		private変数
	=================*/

	HWND		hwnd;			// ウィンドハンドル
	HINSTANCE	hInstance;		// インスタンスハンドル
	bool		windowed;		// 全画面表示するか(true:ウィンドウ表示  false:フルスクリーン表示)
	LPCTSTR		wcn;			// ウィンドウクラスの名前


	float		width;			// ウィンドウサイズ(幅)	
	float		height;			// ウィンドウサイズ(高さ)


	ID2D1HwndRenderTarget* pRenderTarget;		// レンダーターゲットへのポインタ
	ID2D1Factory* pD2DFactory;		// Direct2D Factoryへのポインタ

	GameSystem* game;
	ImageManager* image;
	UIManager* ui;
	AudioManager* audio;

	bool isDefeat;


	static Mouse* mouse;
	static int time;
	static GameState currentGameState;
	static bool isPouse;

	/*=================
		private関数
	=================*/

	/*==== 初期化関数 ====*/
	HRESULT InitWindow(int);	// メインウィンドウの初期化関数
	HRESULT Init2D();			// Direct2Dの初期化関数


	/*==== その他の処理関数 ====*/
	static LRESULT CALLBACK WinProc(HWND, UINT, WPARAM, LPARAM);		// コールバック関数
	int MessageLoop();			// メッセージループ関数


public:

	Window(HINSTANCE, int);		// コンストラクタ
	~Window();					// デストラクタ

	/*==== アクセッサ ====*/
	HWND GetHWND();
	ID2D1HwndRenderTarget* GetRenderTarget();

};