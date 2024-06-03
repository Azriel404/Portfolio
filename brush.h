#pragma once

#include <d2d1.h>
#include <vector>
#include "color.h"

class Brush {

private:

	/*=================
		private変数
	=================*/

	ID2D1HwndRenderTarget* pRenderTarget;		// レンダーターゲット
	std::vector<ID2D1SolidColorBrush*>	brushes;			// ブラシを配列として保持


	/*=================
		private関数
	=================*/

	void Create();		// ブラシの生成


public:

	Brush(ID2D1HwndRenderTarget*);		// コンストラクタ
	~Brush();							// デストラクタ


	/*==== アクセッサ ====*/
	ID2D1SolidColorBrush* GetBrushColor(Color);		// ブラシの色を変数として受け取り、その色のブラシのポインタを返すゲッター
	ID2D1SolidColorBrush* GetBrushColor(int);		// int型の変数を受け取り、その要素番号にあるブラシのポインタを返すゲッター
	int GetBrushNum();								// ブラシの数を返すゲッター

};