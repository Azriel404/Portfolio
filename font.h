#pragma once

#include <d2d1.h>
#include <dwrite.h>
#include <array>
#include <vector>
#include <wchar.h>
#include "brush.h"
#include "command.h"

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

enum Func {

	TIME = 0,
	SCORE,
	WAVE,
	RESOURCE,
	BASE_NAME,
	HISCORE,
	COST

};



class Font {

private:

	ID2D1HwndRenderTarget* pRenderTarget;
	IDWriteFactory* pDWFactory;

	std::vector<IDWriteTextFormat*> FontFormat;

	std::array<WCHAR, 16> nowTime;
	std::array<WCHAR, 32> nowScore;
	std::array<WCHAR, 16> nowWaves;
	std::array<WCHAR, 32> nowResources;
	std::array<WCHAR, 32> hiscore;
	std::array<WCHAR, 16> charaCost;

	Brush* brush;
	Character* character;
	Command* command;

	void ToCenter(IDWriteTextFormat*);

	void DrawTime(int);
	void DrawScore(int);
	void DrawWaves(int);
	void DrawResources(int);
	void DrawBaseName();
	void DrawCost();


public:

	std::vector<IDWriteTextFormat*> OtherFontFormat;

	Font(ID2D1HwndRenderTarget*, Brush*, Character*, Command*);
	~Font();

	void CreateFontFormat(float, bool);

	void DrawALL(int, int, int, int);
	void DrawHiScore(int);

};