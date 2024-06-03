#include "base.h"

Base::Base(ID2D1HwndRenderTarget* pRT, Brush* brush) :
	pRT(pRT), brush(brush)
{

	D2D1_POINT_2F setPosition = D2D1::Point2F(20.0f, 25.0f);
	float size = 300.0f;

	// �x�[�X�̗̑�
	baseHP = 50;
	baseHP_bar = D2D1::RectF(setPosition.x, setPosition.y, setPosition.x + size, setPosition.y + 20.0f);


	// ���݂̗̑�
	nowHP = baseHP;
	nowHP_bar = baseHP_bar;

}


Base::~Base() {

}


void Base::DecrementHP() {

	// �̗͂�1�ȏ゠�邩�H
	if (!DefeatCheck()) {

		nowHP -= 1;		// �_���[�W���ǂꂭ�炢�󂯂邩

		float barWidth = baseHP_bar.right - baseHP_bar.left;		// �o�[�̉���
		float damage = barWidth / static_cast<float>(baseHP);		// 1�_���[�W������̃o�[�̌�����

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

	// �̗͂��Ȃ��Ȃ�����...
	if (nowHP <= 0) {
		nowHP = 0;
		return true;
	}

	// �̗͂���������...
	else {
		return false;
	}

}


// �f�o�b�O�p�̊֐��B��n�̗̑͂�1�ɂ���
void Base::DebugSetHP() {

	nowHP = 1;

	float barWidth = baseHP_bar.right - baseHP_bar.left;		// �o�[�̉���
	float damage = barWidth / static_cast<float>(baseHP);		// 1�_���[�W������̃o�[�̌�����

	nowHP_bar.right = nowHP_bar.left + damage;

}