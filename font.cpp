#include "font.h"


Font::Font(ID2D1HwndRenderTarget* pRenderTarget, Brush* brush, Character* character, Command* command) :
	pRenderTarget(pRenderTarget),
	brush(brush), character(character), command(command)
{
	/*==== �t�@�N�g���̐��� ====*/
	DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(&pDWFactory)
	);


	IDWriteTextFormat* t = nullptr;


	// ���Ԃ̕����t�H���g�t�H�[�}�b�g
	pDWFactory->CreateTextFormat(
		L"Impact",
		nullptr,
		DWRITE_FONT_WEIGHT_BOLD,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		40.0f,
		L"en-US",
		&t
	);

	ToCenter(t);
	FontFormat.push_back(t);


	// �X�R�A�̕����t�H���g�t�H�[�}�b�g
	pDWFactory->CreateTextFormat(
		L"Impact",
		nullptr,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		25.0f,
		L"en-US",
		&t
	);

	ToCenter(t);
	FontFormat.push_back(t);

	// ���݂̃E�F�[�u���̕����t�H���g�t�H�[�}�b�g
	pDWFactory->CreateTextFormat(
		L"Impact",
		nullptr,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		20.0f,
		L"en-US",
		&t
	);

	ToCenter(t);
	FontFormat.push_back(t);


	// �����̕����t�H���g�t�H�[�}�b�g
	pDWFactory->CreateTextFormat(
		L"Impact",
		nullptr,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		25.0f,
		L"en-US",
		&t
	);

	ToCenter(t);
	FontFormat.push_back(t);


	// ���_�̖��O�̕����t�H���g�t�H�[�}�b�g
	pDWFactory->CreateTextFormat(
		L"Impact",
		nullptr,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		15.0f,
		L"en-US",
		&t
	);

	ToCenter(t);
	FontFormat.push_back(t);


	// ���U���g�̃X�R�A�̕����t�H���g�t�H�[�}�b�g
	pDWFactory->CreateTextFormat(
		L"Impact",
		nullptr,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		60.0f,
		L"en-US",
		&t
	);

	ToCenter(t);
	FontFormat.push_back(t);


	// �L�����̃R�X�g�̕����t�H���g�t�H�[�}�b�g
	pDWFactory->CreateTextFormat(
		L"Impact",
		nullptr,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		20.0f,
		L"en-US",
		&t
	);

	ToCenter(t);
	FontFormat.push_back(t);


	t = nullptr;

}


Font::~Font() {

	pDWFactory->Release();

	for (auto& f : FontFormat) {
		f->Release();
	}

}


void Font::CreateFontFormat(float fontSize, bool isCenter = false) {

	IDWriteTextFormat* t = nullptr;

	// �t�H���g�t�H�[�}�b�g�̍쐬
	pDWFactory->CreateTextFormat(
		L"Impact",
		nullptr,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		fontSize,
		L"en-US",
		&t
	);

	// �㉺���E�������������邩
	if (isCenter) {

		ToCenter(t);

	}

	OtherFontFormat.push_back(t);

}





void Font::DrawALL(int time, int score, int waves, int resources) {

	DrawTime(time);
	DrawScore(score);
	DrawWaves(waves);
	DrawResources(resources);
	DrawBaseName();
	DrawCost();

}


void Font::ToCenter(IDWriteTextFormat* format) {

	// �e�L�X�g�̉������̒�������
	format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);

	// �e�L�X�g�̏c�����̒�������
	format->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

}


void Font::DrawTime(int time) {

	int minutes = time / 60;
	int seconds = time % 60;

	if (minutes >= 99) {
		minutes = 99;
	}

	if (seconds >= 59) {
		seconds = 59;
	}

	swprintf_s(nowTime.data(), nowTime.size(), L"%02d : %02d", minutes, seconds);


	float width = pRenderTarget->GetSize().width;
	float size = 140;
	D2D1_RECT_F TimeRect = D2D1::RectF(((width / 2.0f) - (size / 2.0f)), 10.0f, ((width / 2.0f) + (size / 2.0f)), 50.0f);		// �ݒu�ʒu


	// �g�̕`��
	pRenderTarget->FillRectangle(TimeRect, brush->GetBrushColor(White));
	pRenderTarget->DrawRectangle(TimeRect, brush->GetBrushColor(Black), 3.0f);


	// �e�L�X�g�̕`��
	pRenderTarget->DrawTextW(
		nowTime.data(),
		wcslen(nowTime.data()),
		FontFormat[TIME],
		TimeRect,
		brush->GetBrushColor(Black)
	);

}


void Font::DrawScore(int score) {

	if (score >= 99999999) {
		score = 99999999;
	}

	swprintf_s(nowScore.data(), nowScore.size(), L"SCORE : %08d", score);

	float width = pRenderTarget->GetSize().width;
	float size = 150;
	D2D1_RECT_F ScoreRect = D2D1::RectF(width - size - 50.0f, 10.0f, width - 10.0f, 50.0f);


	// �g�̕`��
	pRenderTarget->FillRectangle(ScoreRect, brush->GetBrushColor(White));
	pRenderTarget->DrawRectangle(ScoreRect, brush->GetBrushColor(Black), 3.0f);


	// �e�L�X�g�̕`��
	pRenderTarget->DrawTextW(
		nowScore.data(),
		wcslen(nowScore.data()),
		FontFormat[SCORE],
		ScoreRect,
		brush->GetBrushColor(Black)
	);

}


void Font::DrawWaves(int waves) {

	if (waves >= 99) {
		waves = 99;
	}

	swprintf_s(nowWaves.data(), nowWaves.size(), L"Wave : %02d", waves);

	float height = pRenderTarget->GetSize().height;
	float size = 85.0f;
	float Ypos = height - 30.0f - 10.0f;		// 30.0f�̓t�H���g�T�C�Y+�}�[�W�� 10.0f�̓}�[�W��
	D2D1_RECT_F WaveRect = D2D1::RectF(10.0f, Ypos, 10.0f + size, height - 10.0f);


	// �g�̕`��
	pRenderTarget->FillRectangle(WaveRect, brush->GetBrushColor(White));
	pRenderTarget->DrawRectangle(WaveRect, brush->GetBrushColor(Black), 3.0f);


	// �e�L�X�g�̕`��
	pRenderTarget->DrawTextW(
		nowWaves.data(),
		wcslen(nowWaves.data()),
		FontFormat[WAVE],
		WaveRect,
		brush->GetBrushColor(Black)
	);


}



void Font::DrawResources(int resources) {

	if (resources >= 99999999) {
		resources = 99999999;
	}

	swprintf_s(nowResources.data(), nowResources.size(), L"RESOURCE : %08d", resources);

	float width = pRenderTarget->GetSize().width;
	float height = pRenderTarget->GetSize().height;
	float size = 250.0f;
	float Xpos = (width / 2.0f) - (size / 2.0f);
	float Ypos = height - 150.0f - 10.0f;

	D2D1_RECT_F ResourceRect = D2D1::RectF(Xpos, Ypos, Xpos + size, height - 130.0f);


	// �g�̕`��
	pRenderTarget->FillRectangle(ResourceRect, brush->GetBrushColor(White));
	pRenderTarget->DrawRectangle(ResourceRect, brush->GetBrushColor(Black), 3.0f);


	// �e�L�X�g�̕`��
	pRenderTarget->DrawTextW(
		nowResources.data(),
		wcslen(nowResources.data()),
		FontFormat[RESOURCE],
		ResourceRect,
		brush->GetBrushColor(Black)
	);


}


void Font::DrawBaseName() {

	float size = 60.0f;
	D2D1_RECT_F BaseName = D2D1::RectF(20.0f, 5.0f, 20.0f + size, 25.0f);

	// �g�̕`��
	pRenderTarget->FillRectangle(BaseName, brush->GetBrushColor(White));
	pRenderTarget->DrawRectangle(BaseName, brush->GetBrushColor(Black));


	// �e�L�X�g�̕`��
	pRenderTarget->DrawTextW(
		L"MY BASE",
		wcslen(L"MY BASE"),
		FontFormat[BASE_NAME],
		BaseName,
		brush->GetBrushColor(Black)
	);

}


void Font::DrawCost() {

	auto& cost = character->GetCost();
	auto& com = command->GetCommands();
	float sizeW, sizeH, Xpos, Ypos;
	D2D1_RECT_F costRect;

	for (int i = 0; i < com.size(); i++) {

		swprintf_s(charaCost.data(), charaCost.size(), L"cost : %d", cost[i]);

		sizeW = com[i].rect.right - com[i].rect.left;
		sizeH = 30.0f;
		Xpos = com[i].rect.left;
		Ypos = com[i].rect.bottom - sizeH - 15.0f;

		costRect = D2D1::RectF(Xpos, Ypos, Xpos + sizeW, Ypos + sizeH);

		pRenderTarget->DrawTextW(
			charaCost.data(),
			wcslen(charaCost.data()),
			FontFormat[COST],
			costRect,
			brush->GetBrushColor(Black)
		);

	}


}



void Font::DrawHiScore(int score = 0) {

	swprintf_s(hiscore.data(), hiscore.size(), L"Your  Score  is  \"%08d\" !!!", score);

	float width = pRenderTarget->GetSize().width;
	float height = pRenderTarget->GetSize().height;
	float sizeW = 800.0f;
	float sizeH = 100.0f;
	float Xpos = (width / 2.0f) - (sizeW / 2.0f);
	float Ypos = (height / 2.0f) - (sizeH / 2.0f);

	D2D1_RECT_F ScoreRect = D2D1::RectF(Xpos, Ypos, Xpos + sizeW, Ypos + sizeH);


	// �g�̕`��
	pRenderTarget->FillRectangle(ScoreRect, brush->GetBrushColor(White));
	pRenderTarget->DrawRectangle(ScoreRect, brush->GetBrushColor(Black), 3.0f);


	// �e�L�X�g�̕`��
	pRenderTarget->DrawTextW(
		hiscore.data(),
		wcslen(hiscore.data()),
		FontFormat[HISCORE],
		ScoreRect,
		brush->GetBrushColor(Black)
	);


}