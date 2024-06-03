#include "brush.h"


/*=====================
		private
=====================*/

void Brush::Create() {

	if (brushes.empty()) {		// brushes����ł��邩(�u���V���쐬����Ă��Ȃ����)���m�F����

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

/*==== �R���X�g���N�^ ====*/
Brush::Brush(ID2D1HwndRenderTarget* pRenderTarget) :
	pRenderTarget(pRenderTarget)
{

	Create();

}


/*==== �f�X�g���N�^ ====*/
Brush::~Brush() {

	for (auto& b : brushes) {

		b->Release();
		b = nullptr;

	}

}


/*==== �u���V�̐F��Ԃ��Q�b�^�[(�������F�Ŏw�肳��Ă���ꍇ) ====*/
ID2D1SolidColorBrush* Brush::GetBrushColor(Color c) {

	/*==== �w�肳�ꂽ�F�����݂��邩�𔻒� ====*/
	if (static_cast<int>(c) >= 0 && static_cast<int>(c) < brushes.size()) {

		return brushes[static_cast<int>(c)];

	}

	return brushes[Gray];		// �����Ȃ�������A�f�t�H���g�J���[�Ƃ��ăO���[��Ԃ�

}


/*==== �u���V�̐F��Ԃ��Q�b�^�[(�������v�f�ԍ��Ŏw�肳��Ă���ꍇ) ====*/
ID2D1SolidColorBrush* Brush::GetBrushColor(int i) {

	/*==== �w�肳�ꂽ�v�f�ԍ������݂��邩�𔻒� ====*/
	if (static_cast<int>(i) >= 0 && static_cast<int>(i) < brushes.size()) {

		return brushes[static_cast<int>(i)];

	}

	return brushes[Gray];		// �����Ȃ�������A�f�t�H���g�J���[�Ƃ��ăO���[��Ԃ�

}


/*==== �u���V�̐���Ԃ��Q�b�^�[ ====*/
int Brush::GetBrushNum() {

	return brushes.size();		// brushes�̗v�f����Ԃ�

}
