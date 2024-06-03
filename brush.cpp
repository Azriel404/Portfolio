#include "brush.h"


/*=====================
		private
=====================*/

void Brush::Create() {

	if (brushes.empty()) {		// brushesが空であるか(ブラシが作成されていない状態)を確認する

		/*==== Red ====*/
		ID2D1SolidColorBrush* pRed;
		D2D1::ColorF red = D2D1::ColorF(1.0f, 0.0f, 0.0f);
		pRenderTarget->CreateSolidColorBrush(red, &pRed);
		brushes.push_back(pRed);


		/*==== Green ====*/
		ID2D1SolidColorBrush* pGreen;
		D2D1::ColorF green = D2D1::ColorF(0.0f, 1.0f, 0.0f);
		pRenderTarget->CreateSolidColorBrush(green, &pGreen);
		brushes.push_back(pGreen);


		/*==== Blue ====*/
		ID2D1SolidColorBrush* pBlue;
		D2D1::ColorF blue = D2D1::ColorF(0.0f, 0.0f, 1.0f);
		pRenderTarget->CreateSolidColorBrush(blue, &pBlue);
		brushes.push_back(pBlue);


		/*==== Dodgerblue ====*/
		ID2D1SolidColorBrush* pDBlue;
		D2D1::ColorF dblue = D2D1::ColorF(30.0f / 255.0f, 144.0f / 255.0f, 255.0f / 255.0f);
		pRenderTarget->CreateSolidColorBrush(dblue, &pDBlue);
		brushes.push_back(pDBlue);


		/*==== Yellow ====*/
		ID2D1SolidColorBrush* pYellow;
		D2D1::ColorF yellow = D2D1::ColorF(1.0f, 1.0f, 0.0f);
		pRenderTarget->CreateSolidColorBrush(yellow, &pYellow);
		brushes.push_back(pYellow);


		/*==== Black ====*/
		ID2D1SolidColorBrush* pBlack;
		D2D1::ColorF black = D2D1::ColorF(0.0f, 0.0f, 0.0f);
		pRenderTarget->CreateSolidColorBrush(black, &pBlack);
		brushes.push_back(pBlack);


		/*==== White ====*/
		ID2D1SolidColorBrush* pWhite;
		D2D1::ColorF white = D2D1::ColorF(1.0f, 1.0f, 1.0f);
		pRenderTarget->CreateSolidColorBrush(white, &pWhite);
		brushes.push_back(pWhite);


		/*==== Gray ====*/
		ID2D1SolidColorBrush* pGray;
		D2D1::ColorF gray = D2D1::ColorF(0.5f, 0.5f, 0.5f);
		pRenderTarget->CreateSolidColorBrush(gray, &pGray);
		brushes.push_back(pGray);


		/*====Purple====*/
		ID2D1SolidColorBrush* pPurple;
		D2D1::ColorF purple = D2D1::ColorF(1.0f, 0.0f, 1.0f);
		pRenderTarget->CreateSolidColorBrush(purple, &pPurple);
		brushes.push_back(pPurple);

	}

}



/*====================
		public
====================*/

/*==== コンストラクタ ====*/
Brush::Brush(ID2D1HwndRenderTarget* pRenderTarget) :
	pRenderTarget(pRenderTarget)
{

	Create();

}


/*==== デストラクタ ====*/
Brush::~Brush() {

	for (auto& b : brushes) {

		b->Release();
		b = nullptr;

	}

}


/*==== ブラシの色を返すゲッター(引数が色で指定されている場合) ====*/
ID2D1SolidColorBrush* Brush::GetBrushColor(Color c) {

	/*==== 指定された色が存在するかを判定 ====*/
	if (static_cast<int>(c) >= 0 && static_cast<int>(c) < brushes.size()) {

		return brushes[static_cast<int>(c)];

	}

	return brushes[Gray];		// もしなかったら、デフォルトカラーとしてグレーを返す

}


/*==== ブラシの色を返すゲッター(引数が要素番号で指定されている場合) ====*/
ID2D1SolidColorBrush* Brush::GetBrushColor(int i) {

	/*==== 指定された要素番号が存在するかを判定 ====*/
	if (static_cast<int>(i) >= 0 && static_cast<int>(i) < brushes.size()) {

		return brushes[static_cast<int>(i)];

	}

	return brushes[Gray];		// もしなかったら、デフォルトカラーとしてグレーを返す

}


/*==== ブラシの数を返すゲッター ====*/
int Brush::GetBrushNum() {

	return brushes.size();		// brushesの要素数を返す

}
