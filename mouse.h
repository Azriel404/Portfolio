#pragma once

#include <d2d1.h>
#include "game_system.h"

#pragma comment(lib, "d2d1.lib")



class Mouse {

private:

	D2D1_POINT_2F mousePos;
	float mouseXpos;
	float mouseYpos;

	GameSystem* game;

	Brush* brush;
	Field* field;
	Command* command;
	Character* character;


public:

	Mouse(GameSystem*);		// �R���X�g���N�^
	~Mouse();				// �f�X�g���N�^


	/*==== �}�E�X����֐� ====*/
	void OnMouseMove(float, float);		// �}�E�X�𓮂����ƍ��W���X�V����
	void MouseClick_L();				// �E�N���b�N
	void MouseClick_R();				// ���N���b�N


	/*==== �A�N�Z�b�T ====*/
	D2D1_POINT_2F GetMousePos();
	float GetMouseXPos();
	float GetMouseYpos();


};

