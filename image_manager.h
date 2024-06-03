#pragma once
#include <d2d1.h>
#pragma comment(lib, "d2d1.lib")

#include <wincodec.h>
#pragma comment(lib, "WindowsCodecs.lib")

#include <vector>


struct ImageInfo {
	ID2D1Bitmap* pBitmap;
	float targetWidth, targetHeight;		// �����_�����O�����Ƃ��̑傫��
	float size;
	float x, y;								// �����_�����O������W(����)
};

class ImageManager {
private:

	ID2D1HwndRenderTarget* pRenderTarget;
	std::vector<ImageInfo> images;

	ID2D1Bitmap* LoadBitmapFromFile(const wchar_t*);

public:

	ImageManager(ID2D1HwndRenderTarget*);
	~ImageManager();

	ID2D1Bitmap* TransferFromFile(const wchar_t*);
	void Add(const wchar_t*, float, float, float, float);
	void Draw();

};