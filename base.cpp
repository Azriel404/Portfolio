#include "base.h"

Base::Base(ID2D1HwndRenderTarget* pRT, Brush* brush) :
	pRT(pRT), brush(brush)
{

	D2D1_POINT_2F setPosition = D2D1::Point2F(20.0f, 25.0f);
	float size = 300.0f;

	// ベースの体力
	baseHP = 50;
	baseHP_bar = D2D1::RectF(setPosition.x, setPosition.y, setPosition.x + size, setPosition.y + 20.0f);


	// 現在の体力
	nowHP = baseHP;
	nowHP_bar = baseHP_bar;

}


Base::~Base() {

}


void Base::DecrementHP() {

	// 体力が1以上あるか？
	if (!DefeatCheck()) {

		nowHP -= 1;		// ダメージをどれくらい受けるか

		float barWidth = baseHP_bar.right - baseHP_bar.left;		// バーの横幅
		float damage = barWidth / static_cast<float>(baseHP);		// 1ダメージ当たりのバーの減少量

		if (nowHP_bar.right >= 0.0f) {

			nowHP_bar.right -= damage;

		}

	}

}


void Base::ShowHP() {

	pRT->FillRectangle(baseHP_bar, brush->GetBrushColor(Black));
	pRT->FillRectangle(nowHP_bar, brush->GetBrushColor(Red));

}


bool Base::DefeatCheck() {

	// 体力がなくなったら...
	if (nowHP <= 0) {
		nowHP = 0;
		return true;
	}

	// 体力があったら...
	else {
		return false;
	}

}


// デバッグ用の関数。基地の体力を1にする
void Base::DebugSetHP() {

	nowHP = 1;

	float barWidth = baseHP_bar.right - baseHP_bar.left;		// バーの横幅
	float damage = barWidth / static_cast<float>(baseHP);		// 1ダメージ当たりのバーの減少量

	nowHP_bar.right = nowHP_bar.left + damage;

}