#pragma once

#include <d2d1.h>
#include <vector>
#include "color.h"

class Brush {

private:

	/*=================
		private�ϐ�
	=================*/

	ID2D1HwndRenderTarget* pRenderTarget;		// �����_�[�^�[�Q�b�g
	std::vector<ID2D1SolidColorBrush*>	brushes;			// �u���V��z��Ƃ��ĕێ�


	/*=================
		private�֐�
	=================*/

	void Create();		// �u���V�̐���


public:

	Brush(ID2D1HwndRenderTarget*);		// �R���X�g���N�^
	~Brush();							// �f�X�g���N�^


	/*==== �A�N�Z�b�T ====*/
	ID2D1SolidColorBrush* GetBrushColor(Color);		// �u���V�̐F��ϐ��Ƃ��Ď󂯎��A���̐F�̃u���V�̃|�C���^��Ԃ��Q�b�^�[
	ID2D1SolidColorBrush* GetBrushColor(int);		// int�^�̕ϐ����󂯎��A���̗v�f�ԍ��ɂ���u���V�̃|�C���^��Ԃ��Q�b�^�[
	int GetBrushNum();								// �u���V�̐���Ԃ��Q�b�^�[

};