#pragma once

#include <d2d1.h>
#pragma comment(lib, "d2d1.lib")

#include "image_manager.h"
#include "font.h"


class UIManager {
private:

	ID2D1HwndRenderTarget* pRenderTarget;
	ImageManager* image;

	ID2D1Bitmap* start;
	ID2D1Bitmap* result;
	ID2D1Bitmap* continueWnd;


public:

	UIManager(ID2D1HwndRenderTarget*, ImageManager*);
	~UIManager();

	void ShowStartScreen();
	void ShowResultScreen(Font*, int);
	void ShowContinueWindow();


};
