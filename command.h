#pragma once

#include <array>

#include <d2d1.h>
#pragma comment(lib, "d2d1.lib")
#include "color.h"

#include <string>
#include "image_manager.h"
#include "character.h"

typedef struct CommandInfo {

	D2D1_RECT_F rect;
	ID2D1SolidColorBrush* color;
	int id;

	ImageInfo image;

} ComInfo;



class Command {

private:

	D2D1_RECT_F comArea;

	ID2D1HwndRenderTarget* pRenderTarget;

	std::array<ComInfo, 5> commands;
	std::array<ID2D1Bitmap*, 5> images;

	float comHeight;

	int isSelected;

public:

	Command(ID2D1HwndRenderTarget*, D2D1_RECT_F, std::array<ID2D1Bitmap*, 5>&);
	~Command();

	HRESULT InitCommands(D2D1_RECT_F);
	void Draw(ID2D1SolidColorBrush*, ID2D1SolidColorBrush*);
	void DetectCommand(D2D1_POINT_2F);


	D2D1_RECT_F GetCommandArea();
	std::array<ComInfo, 5>& GetCommands();
	int GetIsSelected();
	void ResetIsSelected();

};

