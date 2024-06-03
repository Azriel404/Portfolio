#pragma once

#include <Windows.h>
#include "mouse.h"
#include <d2d1.h>
#include "game_system.h"
#include "ui_manager.h"
#include "audio_manager.h"

#pragma comment (lib, "d2d1.lib")

enum GameState {
	Start,
	InGame,
	Result
};

class Window {

private:

	/*=================
		private�ϐ�
	=================*/

	HWND		hwnd;			// �E�B���h�n���h��
	HINSTANCE	hInstance;		// �C���X�^���X�n���h��
	bool		windowed;		// �S��ʕ\�����邩(true:�E�B���h�E�\��  false:�t���X�N���[���\��)
	LPCTSTR		wcn;			// �E�B���h�E�N���X�̖��O


	float		width;			// �E�B���h�E�T�C�Y(��)	
	float		height;			// �E�B���h�E�T�C�Y(����)


	ID2D1HwndRenderTarget* pRenderTarget;		// �����_�[�^�[�Q�b�g�ւ̃|�C���^
	ID2D1Factory* pD2DFactory;		// Direct2D Factory�ւ̃|�C���^

	GameSystem* game;
	ImageManager* image;
	UIManager* ui;
	AudioManager* audio;

	bool isDefeat;


	static Mouse* mouse;
	static int time;
	static GameState currentGameState;
	static bool isPouse;

	/*=================
		private�֐�
	=================*/

	/*==== �������֐� ====*/
	HRESULT InitWindow(int);	// ���C���E�B���h�E�̏������֐�
	HRESULT Init2D();			// Direct2D�̏������֐�


	/*==== ���̑��̏����֐� ====*/
	static LRESULT CALLBACK WinProc(HWND, UINT, WPARAM, LPARAM);		// �R�[���o�b�N�֐�
	int MessageLoop();			// ���b�Z�[�W���[�v�֐�


public:

	Window(HINSTANCE, int);		// �R���X�g���N�^
	~Window();					// �f�X�g���N�^

	/*==== �A�N�Z�b�T ====*/
	HWND GetHWND();
	ID2D1HwndRenderTarget* GetRenderTarget();

};