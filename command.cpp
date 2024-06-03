#include "command.h"


/*
	private
*/

HRESULT Command::InitCommands(D2D1_RECT_F areaRect) {

	comArea = D2D1::RectF(areaRect.left, areaRect.bottom + 30.0f, areaRect.right, pRenderTarget->GetSize().height);		// コマンドが利用するエリアの算出

	int comNum = commands.size();		// コマンドの数
	float margin = 10.0f;				// コマンドとコマンドのマージン
	float comWidth = ((comArea.right - comArea.left) - (margin * (comNum + 1))) / comNum;		// 利用できるエリアから、マージンを差し引いた値をコマンドの総数で割ることで、一つのコマンドの横幅が求まる
	float comHeight = comArea.bottom - comArea.top;
	float next = margin + comWidth;

	float left;
	for (int i = 0; i < comNum; i++) {

		left = comArea.left + margin + next * i;
		commands[i].rect = D2D1::RectF(left, (comArea.top + comHeight / 5.0f), left + comWidth, comArea.top + comHeight);
		commands[i].id = i;

		/*==== 画像関連 ====*/
		commands[i].image.pBitmap = images[i];
		commands[i].image.size = 50.0f;
		float a = (commands[i].rect.right - commands[i].rect.left) / 2.0f - commands[i].image.size / 2.0f;
		commands[i].image.x = commands[i].rect.left + a;
		commands[i].image.y = commands[i].rect.top + 15.0f;

	}

	return S_OK;

}


/*
	public
*/

Command::Command(ID2D1HwndRenderTarget* pRenderTarget, D2D1_RECT_F areaRect, std::array<ID2D1Bitmap*, 5>& heroImage) :
	pRenderTarget(pRenderTarget)
{

	isSelected = 999;
	images = heroImage;
	InitCommands(areaRect);

}


Command::~Command() {

}



void Command::Draw(ID2D1SolidColorBrush* color, ID2D1SolidColorBrush* edgeColor) {

	for (int i = 0; i < commands.size(); i++) {

		if (commands[i].id == isSelected) {

			pRenderTarget->FillRectangle(commands[i].rect, color);
			pRenderTarget->DrawRectangle(commands[i].rect, edgeColor, 5.0f, NULL);

		}
		else {

			pRenderTarget->FillRectangle(commands[i].rect, color);

		}

		pRenderTarget->DrawBitmap(
			commands[i].image.pBitmap,
			D2D1::RectF(
				commands[i].image.x,
				commands[i].image.y,
				commands[i].image.x + commands[i].image.size,
				commands[i].image.y + commands[i].image.size
			)
		);

	}

}


void Command::DetectCommand(D2D1_POINT_2F point) {

	for (int i = 0; i < commands.size(); i++) {

		if (point.x >= commands[i].rect.left && point.x <= commands[i].rect.right) {

			if (point.y >= commands[i].rect.top && point.y <= commands[i].rect.bottom) {

				if (commands[i].id != isSelected) {

					isSelected = i;

				}

				else {

					isSelected = 999;

				}

			}

		}

	}

}



D2D1_RECT_F Command::GetCommandArea() {

	return comArea;

}


std::array<ComInfo, 5>& Command::GetCommands() {

	return commands;

}


int Command::GetIsSelected() {

	return isSelected;

}


void Command::ResetIsSelected() {

	isSelected = 999;

}