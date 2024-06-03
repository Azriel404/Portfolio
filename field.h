#pragma once

#include <array>
#include <d2d1.h>

#pragma comment(lib, "d2d1.lib")


struct Area {

	D2D1_RECT_F areaRect;
	float crWidth;
	float crHeight;

};


struct Grid {

	float cellWidth;
	float cellHeight;

};

struct CanDrop {

	D2D1_ELLIPSE area;
	bool frag;

};


class Field {

private:

	/*=================
		private�ϐ�
	=================*/

	Area			area;		// �Q�[���G���A��� 
	D2D1_RECT_F		myArea;		// ���L������ݒu�ł���G���A���
	Grid			grid;		// �O���b�h�����


	float	edge;		// �E�B���h�E�̉E�[

	int		row;		// ���w�̍s��
	int		column;		// ���w�̗�


	ID2D1HwndRenderTarget* pRenderTarget;		// �����_�[�^�[�Q�b�g

	std::array<std::array<D2D1_POINT_2F, 3>, 5> centerPos;	// ���w�̊e�Z���̒��S���W��ێ�����z��
	std::array<std::array<CanDrop, 3>, 5> canDropArea;	// ���L������ݒu�ł���G���A


	/*=================
		private�֐�
	=================*/

	/*===== �������֐� ====*/
	HRESULT InitArea();					// �Q�[���G���A�̏������֐�
	HRESULT InitGrids(int, int);		// �O���b�h���̏������֐�
	HRESULT InitCenterPositions();		// �Z���̒��S���W�̏������֐�
	HRESULT InitCanDropArea();


	/*==== �`��֐� ====*/
	void DrawArea(ID2D1SolidColorBrush*);				// �Q�[���G���A�̕`��֐�
	void DrawGrids(ID2D1SolidColorBrush*);				// �O���b�h���̕`��֐�
	void DrawCenterPositions(ID2D1SolidColorBrush*);	// �Z���̒��S�_�̕`��֐�



public:

	Field(ID2D1HwndRenderTarget*);		// �R���X�g���N�^
	~Field();							// �f�X�g���N�^


	/*==== �`��֐� ====*/
	void DrawField(ID2D1SolidColorBrush*, ID2D1SolidColorBrush*, ID2D1SolidColorBrush*);		// �Q�[���G���A�A�O���b�h���A�Z���̒��S�_��`�悷��֐�
	void DrawCanDropArea(ID2D1SolidColorBrush*, ID2D1SolidColorBrush*);

	/*==== �A�N�Z�b�T ====*/
	D2D1_POINT_2F GetCenterPosition(int, int);		// �Z���̒��S�_��Ԃ��Q�b�^�[
	int GetRow();									// �s����Ԃ��Q�b�^�[
	D2D1_RECT_F GetGameArea();						// �Q�[���G���A��Ԃ��Q�b�^�[
	std::array<std::array<CanDrop, 3>, 5>& GetCanDropArea();

};