#pragma once

#include <d2d1.h>
#include "game_system.h"

#pragma comment(lib, "d2d1.lib")



class Mouse {

private:

	D2D1_POINT_2F mousePos;
	float mouseXpos;
	float mouseYpos;

	GameSystem* game;

	Brush* brush;
	Field* field;
	Command* command;
	Character* character;


public:

	Mouse(GameSystem*);		// コンストラクタ
	~Mouse();				// デストラクタ


	/*==== マウス操作関数 ====*/
	void OnMouseMove(float, float);		// マウスを動かすと座標を更新する
	void MouseClick_L();				// 右クリック
	void MouseClick_R();				// 左クリック


	/*==== アクセッサ ====*/
	D2D1_POINT_2F GetMousePos();
	float GetMouseXPos();
	float GetMouseYpos();


};

