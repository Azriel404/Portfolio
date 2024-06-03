#include "ui_manager.h"

/*
	UIManager
*/
UIManager::UIManager(ID2D1HwndRenderTarget* pRenderTarget, ImageManager* image) :
	pRenderTarget(pRenderTarget), image(image)
{

	start = image->TransferFromFile(L"src/images/start.png");
	result = image->TransferFromFile(L"src/images/result.png");
	continueWnd = image->TransferFromFile(L"src/images/continue.png");

}


UIManager::~UIManager() {

}


void UIManager::ShowStartScreen() {

	pRenderTarget->BeginDraw();
	pRenderTarget->Clear();
	pRenderTarget->DrawBitmap(start, D2D1::RectF(0.0f, 0.0f, start->GetSize().width, start->GetSize().height));
	pRenderTarget->EndDraw();

}


void UIManager::ShowResultScreen(Font* font, int score) {

	pRenderTarget->BeginDraw();
	pRenderTarget->Clear();
	pRenderTarget->DrawBitmap(result, D2D1::RectF(0.0f, 0.0f, result->GetSize().width, result->GetSize().height));
	font->DrawHiScore(score);
	pRenderTarget->EndDraw();

}


void UIManager::ShowContinueWindow() {

	D2D1_SIZE_F size = continueWnd->GetSize();
	float sizeWidth = size.width;
	float sizeHeight = size.height;


	float winWidth = pRenderTarget->GetSize().width;
	float winHeight = pRenderTarget->GetSize().height;

	pRenderTarget->BeginDraw();
	pRenderTarget->DrawBitmap(
		continueWnd,
		D2D1::RectF(
			winWidth / 2.0f - sizeWidth / 2.0f,
			winHeight / 2.0f - sizeHeight / 2.0f,
			winWidth / 2.0f + sizeWidth / 2.0f,
			winHeight / 2.0f + sizeHeight / 2.0f
		)
	);
	pRenderTarget->EndDraw();

}