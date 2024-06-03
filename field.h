#pragma once

#include <array>
#include <d2d1.h>

#pragma comment(lib, "d2d1.lib")


struct Area {

	D2D1_RECT_F areaRect;
	float crWidth;
	float crHeight;

};


struct Grid {

	float cellWidth;
	float cellHeight;

};

struct CanDrop {

	D2D1_ELLIPSE area;
	bool frag;

};


class Field {

private:

	/*=================
		private変数
	=================*/

	Area			area;		// ゲームエリア情報 
	D2D1_RECT_F		myArea;		// 自キャラを設置できるエリア情報
	Grid			grid;		// グリッド線情報


	float	edge;		// ウィンドウの右端

	int		row;		// 自陣の行数
	int		column;		// 自陣の列数


	ID2D1HwndRenderTarget* pRenderTarget;		// レンダーターゲット

	std::array<std::array<D2D1_POINT_2F, 3>, 5> centerPos;	// 自陣の各セルの中心座標を保持する配列
	std::array<std::array<CanDrop, 3>, 5> canDropArea;	// 自キャラを設置できるエリア


	/*=================
		private関数
	=================*/

	/*===== 初期化関数 ====*/
	HRESULT InitArea();					// ゲームエリアの初期化関数
	HRESULT InitGrids(int, int);		// グリッド線の初期化関数
	HRESULT InitCenterPositions();		// セルの中心座標の初期化関数
	HRESULT InitCanDropArea();


	/*==== 描画関数 ====*/
	void DrawArea(ID2D1SolidColorBrush*);				// ゲームエリアの描画関数
	void DrawGrids(ID2D1SolidColorBrush*);				// グリッド線の描画関数
	void DrawCenterPositions(ID2D1SolidColorBrush*);	// セルの中心点の描画関数



public:

	Field(ID2D1HwndRenderTarget*);		// コンストラクタ
	~Field();							// デストラクタ


	/*==== 描画関数 ====*/
	void DrawField(ID2D1SolidColorBrush*, ID2D1SolidColorBrush*, ID2D1SolidColorBrush*);		// ゲームエリア、グリッド線、セルの中心点を描画する関数
	void DrawCanDropArea(ID2D1SolidColorBrush*, ID2D1SolidColorBrush*);

	/*==== アクセッサ ====*/
	D2D1_POINT_2F GetCenterPosition(int, int);		// セルの中心点を返すゲッター
	int GetRow();									// 行数を返すゲッター
	D2D1_RECT_F GetGameArea();						// ゲームエリアを返すゲッター
	std::array<std::array<CanDrop, 3>, 5>& GetCanDropArea();

};