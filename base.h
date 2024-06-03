#pragma once

#include <d2d1.h>
#pragma comment(lib, "d2d1.lib")
#include "brush.h"

class Base {
private:

	int baseHP;
	int nowHP;

	D2D1_RECT_F baseHP_bar;
	D2D1_RECT_F nowHP_bar;

	ID2D1HwndRenderTarget* pRT;
	Brush* brush;

public:

	Base(ID2D1HwndRenderTarget*, Brush*);
	~Base();

	void DecrementHP();
	void ShowHP();
	bool DefeatCheck();

	void DebugSetHP();

};

