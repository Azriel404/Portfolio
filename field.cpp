#include "field.h"

/*=====================
		private
=====================*/

/*==== ゲームエリアの初期化関数 ====*/
HRESULT Field::InitArea() {

	D2D1_SIZE_F clientRectSize = pRenderTarget->GetSize();
	edge = pRenderTarget->GetSize().width;

	area.crWidth = static_cast<float>(clientRectSize.width * 0.8f);
	area.crHeight = static_cast<float>(clientRectSize.height * 0.6f);


	float left = (clientRectSize.width - area.crWidth) / 2.0f;
	float top = (clientRectSize.height - area.crHeight) / 4.0f;
	float right = left + (area.crWidth * (2.0f / 5.0f));
	float bottom = top + area.crHeight;

	area.areaRect = D2D1::RectF(left, top, left + area.crWidth, top + area.crHeight);
	myArea = D2D1::RectF(left, top, right, bottom);


	return S_OK;

}


/*==== グリッド線の初期化関数 ====*/
HRESULT Field::InitGrids(int row, int column) {

	this->row = row;
	this->column = column;

	grid.cellWidth = (myArea.right - myArea.left) / column;
	grid.cellHeight = (myArea.bottom - myArea.top) / row;


	return S_OK;

}


/*==== 自キャラのセルの中心点の初期化関数 ====*/
HRESULT Field::InitCenterPositions() {

	float Xpos = myArea.left + (grid.cellWidth / 2.0f);
	float Ypos = myArea.top + (grid.cellHeight / 2.0f);

	for (int i = 0; i < row; i++) {

		for (int j = 0; j < column; j++) {

			centerPos[i][j] = D2D1::Point2F(Xpos + (grid.cellWidth * j), Ypos + (grid.cellHeight * i));

		}

	}

	return S_OK;

}


HRESULT Field::InitCanDropArea() {

	for (int i = 0; i < row; i++) {

		for (int j = 0; j < column; j++) {

			canDropArea[i][j].area = D2D1::Ellipse(D2D1::Point2(centerPos[i][j].x, centerPos[i][j].y - 15.0f), 30.0f, 30.0f);
			canDropArea[i][j].frag = true;		// キャラクターを設置できるか

		}

	}

	return S_OK;

}



/*==== ゲームエリアの描画関数 ====*/
void Field::DrawArea(ID2D1SolidColorBrush* color) {

	pRenderTarget->FillRectangle(myArea, color);

}


/*==== グリッド線の描画関数 ====*/
void Field::DrawGrids(ID2D1SolidColorBrush* color) {


	/*==== 横線 ====*/
	for (int i = 0; i < (row + 1); i++) {

		pRenderTarget->DrawLine(
			D2D1::Point2F(myArea.left, myArea.top + grid.cellHeight * i),
			D2D1::Point2F(edge, myArea.top + grid.cellHeight * i),
			color,
			3.0f
		);

	}


	/*==== 縦線 ====*/
	for (int i = 0; i < (column + 1); i++) {

		pRenderTarget->DrawLine(
			D2D1::Point2F(myArea.left + grid.cellWidth * i, myArea.top),
			D2D1::Point2F(myArea.left + grid.cellWidth * i, myArea.bottom),
			color,
			3.0f
		);

	}

}


/*==== 自キャラを設置するセルの中心点の描画関数 ====*/
void Field::DrawCenterPositions(ID2D1SolidColorBrush* color) {

	D2D1_POINT_2F point;

	for (int i = 0; i < row; i++) {

		for (int j = 0; j < column; j++) {

			point = GetCenterPosition(i, j);

			pRenderTarget->FillEllipse(
				D2D1::Ellipse(point, 2.0f, 2.0f),
				color
			);

		}

	}

}


/*====================
		public
====================*/

/*==== コンストラクタ ====*/
Field::Field(ID2D1HwndRenderTarget* pRenderTarget) :
	pRenderTarget(pRenderTarget)
{

	HRESULT hr;

	hr = InitArea();
	hr = InitGrids(5, 3);
	hr = InitCenterPositions();
	hr = InitCanDropArea();

}


/*==== デストラクタ ====*/
Field::~Field() {


}


/*==== 描画関数 ====*/
void Field::DrawField(ID2D1SolidColorBrush* areaColor, ID2D1SolidColorBrush* gridColor, ID2D1SolidColorBrush* centerPosColor) {

	DrawArea(areaColor);
	DrawGrids(gridColor);
	DrawCenterPositions(centerPosColor);

}


void Field::DrawCanDropArea(ID2D1SolidColorBrush* ellipseColor, ID2D1SolidColorBrush* edgeColor) {

	for (int i = 0; i < row; i++) {

		for (int j = 0; j < column; j++) {

			if (canDropArea[i][j].frag) {

				pRenderTarget->FillEllipse(canDropArea[i][j].area, ellipseColor);
				pRenderTarget->DrawEllipse(canDropArea[i][j].area, edgeColor, 1.0f, NULL);

			}

		}

	}
}


/*==== アクセッサ ====*/
/*-- セルの中心点を返す --*/
D2D1_POINT_2F Field::GetCenterPosition(int row, int column) {

	// centerPos のサイズが row および column の範囲内であることを確認
	if (row >= 0 && row < centerPos.size() && column >= 0 && column < centerPos[0].size()) {
		return centerPos[row][column];
	}
	else {
		OutputDebugStringW(L"Invalid row or column index");
	}

}


/*-- 行数を返す --*/
int Field::GetRow() {

	return row;

}


D2D1_RECT_F Field::GetGameArea() {

	return area.areaRect;

}


std::array<std::array<CanDrop, 3>, 5>& Field::GetCanDropArea() {

	return canDropArea;

}
