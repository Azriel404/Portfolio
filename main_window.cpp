#include "main_window.h"
#include <wchar.h>
#include <stdio.h>
#include <chrono>
#include <thread>

/*=====================
		private
=====================*/

Mouse* Window::mouse;					// �}�E�X�C���X�^���X(Static�ȃR�[���o�b�N�֐��ŗ��p���邽�߁AStatic�w�肳��Ă���)
int Window::time;						// �Q�[���̌o�ߎ���
GameState Window::currentGameState;		// ���݂̃Q�[���̏�
bool Window::isPouse;					// �|�[�Y�t���O

/*==== ���C���E�B���h�E�̏������֐� ====*/
HRESULT Window::InitWindow(int showWnd) {

	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;
	wc.lpszClassName = wcn;
	wc.lpfnWndProc = WinProc;

	if (!RegisterClassExW(&wc)) {
		return E_FAIL;
	}


	// ��ʂ̃T�C�Y���擾
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// �擾������ʂ̃T�C�Y�����ƂɁA�E�B���h�E�̍���̍��W���v�Z����
	int posX = (screenWidth - width) / 2;
	int posY = (screenHeight - height) / 2;


	// �E�B���h�E���쐬����
	hwnd = CreateWindowExW(
		NULL,
		wcn,
		L"CASTLE DEFENDER",
		WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU,
		posX, posY,
		width, height,
		NULL,
		NULL,
		hInstance,
		NULL
	);


	ShowWindow(hwnd, showWnd);
	UpdateWindow(hwnd);

	return S_OK;

}


/*==== Direct2D�̏������֐� ====*/
HRESULT Window::Init2D() {

	HRESULT hr;

	/*==== �t�@�N�g���̍쐬 ====*/
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);
	if (FAILED(hr)) {
		return E_FAIL;
	}


	/*==== �����_�[�^�[�Q�b�g�̐ݒ� ====*/
	RECT clientRect;
	GetClientRect(hwnd, &clientRect);
	UINT32 cWidth = clientRect.right - clientRect.left;
	UINT32 cHeight = clientRect.bottom - clientRect.top;
	D2D1_SIZE_U size = D2D1::SizeU(cWidth, cHeight);

	hr = pD2DFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(hwnd, size),
		&pRenderTarget
	);

	if (FAILED(hr)) {
		return E_FAIL;
	}

	return S_OK;

}


/*==== ���b�Z�[�W���[�v�֐� ====*/
int Window::MessageLoop() {

	const int targetFPS = 144;
	const std::chrono::milliseconds frameTime(1000 / targetFPS);

	auto startTime = std::chrono::high_resolution_clock::now();
	auto endTime = startTime + frameTime;


	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	bool frag = true;

	while (true) {

		if (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {

			if (msg.message == WM_QUIT) {

				break;

			}

			TranslateMessage(&msg);
			DispatchMessageW(&msg);

		}

		else {

			switch (currentGameState) {

			case Start:

				ui->ShowStartScreen();

				break;

			case InGame:

				// FPS�̐���
				std::this_thread::sleep_until(endTime);
				startTime = std::chrono::high_resolution_clock::now();
				endTime = startTime + frameTime;

				// �����_�̗̑͂�1�ȏォ
				if (game->GetBaseInstance()->DefeatCheck()) {

					currentGameState = Result;
					isDefeat = true;

				}


				/*==== �Q�[�����N���A�������H ====*/
				if (!game->ClearCheck()) {

					// game code here
					pRenderTarget->BeginDraw();
					pRenderTarget->Clear();
					game->NowTime(Window::time);
					game->Draw();
					if (!isPouse) {		// �|�[�Y�t���O��false���ǂ���
						game->Update();
					}

					pRenderTarget->EndDraw();



					// �f�o�b�O�p


					// ���U���g��ʂɈڂ�
					if (GetAsyncKeyState('P') & 0x8000) {

						currentGameState = Result;

					}


					// �����_�̗̑͂�1�ɂ���
					if (GetAsyncKeyState('O') & 0x8000) {

						game->GetBaseInstance()->DebugSetHP();

					}


				}

				else {

					if (frag) {
						ui->ShowContinueWindow();
						frag = false;
					}


					/*
						 ���݉�����Ă���L�[�𔻒�@0x8000�͍ŏ�ʃr�b�g��\��
					*/

					// Y��������Ă����...
					if (GetAsyncKeyState('Y') & 0x8000) {

						// �G���Đ���
						game->ContinueGame();
						frag = true;

					}

					// N��������Ă����...
					else if (GetAsyncKeyState('N') & 0x8000) {

						//���U���g��ʂ�
						currentGameState = Result;

					}

				}

				break;


			case Result:

				if (!isDefeat) {

					// �����_�̗̑͂�1�ȏ�̏ꍇ�́A�E�F�[�u���ɉ����ă{�[�i�X�X�R�A��^����
					ui->ShowResultScreen(game->GetFontInstance(), game->GetScore() + (game->GetWaves() * 1000));

				}

				else {

					// �����Ă��܂�����A�{�[�i�X�͂Ȃ�
					ui->ShowResultScreen(game->GetFontInstance(), game->GetScore());

				}

				break;

			}


		}

	}

	return static_cast<int>(msg.wParam);

}


LRESULT CALLBACK Window::WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {


	switch (msg) {

	case WM_CREATE:


		break;


	case WM_TIMER:

		Window::time++;

		break;


	case WM_MOUSEMOVE:

		Window::mouse->OnMouseMove(LOWORD(lParam), HIWORD(lParam));

		break;


	case WM_LBUTTONDOWN:

		Window::mouse->MouseClick_L();

		break;


	case WM_RBUTTONDOWN:

		break;


	case WM_KEYDOWN:		// �����L�[����͂��ꂽ��...

		if (wParam == VK_ESCAPE) {		// Esc�L�[�����͂��ꂽ��...

			if (MessageBoxW(NULL, L"�{���ɃE�B���h�E����܂����H", L"�m�F", MB_YESNO | MB_ICONQUESTION) == IDYES) {

				DestroyWindow(hwnd);

			}

		}

		switch (currentGameState) {

		case Start:

			if (wParam == VK_SPACE) {

				SetTimer(hwnd, 1, 1000, nullptr);
				currentGameState = InGame;
			}

			break;


		case InGame:

			if (wParam == 'F') {

				isPouse = !isPouse;

			}


			break;


		case Result:

			if (wParam == 'Q') {


				if (MessageBoxW(NULL, L"�{���ɃE�B���h�E����܂����H", L"�m�F", MB_YESNO | MB_ICONQUESTION) == IDYES) {

					DestroyWindow(hwnd);

				}

			}

			break;

		}

		break;




	case WM_CLOSE:			// �E�B���h�E�́~�{�^���������ꂽ��...

		if (MessageBoxW(NULL, L"�{���ɃE�B���h�E����܂����H", L"�m�F", MB_YESNO | MB_ICONQUESTION) == IDYES) {

			DestroyWindow(hwnd);

		}

		break;


	case WM_DESTROY:		// ���炩�̃E�B���h�E�j�����߂��󂯎������...

		PostQuitMessage(0);

		break;


	default:				// ��`����Ă��Ȃ����b�Z�[�W���󂯎������...

		return DefWindowProcW(hwnd, msg, wParam, lParam);		// �f�t�H���g�Ƃ��Đݒ肳��Ă���v���V�[�W�������s

	}

	return 0;

}



/*====================
		public
====================*/

/*==== �R���X�g���N�^ ====*/
Window::Window(HINSTANCE hInstance, int showWnd) :
	hwnd(NULL), hInstance(hInstance), windowed(true), width(1000), height(625),
	wcn(L"MainWindow"),
	pRenderTarget(nullptr)
{

	HRESULT hr;

	/*==== �������֐� ====*/
	hr = InitWindow(showWnd);
	hr = Init2D();

	time = 0;
	currentGameState = Start;
	isPouse = false;

	/*==== �C���X�^���X�̍쐬 ====*/
	image = new ImageManager(pRenderTarget);
	ui = new UIManager(pRenderTarget, image);
	audio = new AudioManager();
	game = new GameSystem(pRenderTarget, image, audio);


	mouse = new Mouse(game);

	/*==== ���b�Z�[�W���[�v�֐� ====*/
	MessageLoop();

}


/*==== �f�X�g���N�^ ====*/
Window::~Window() {
	pD2DFactory->Release();
	pRenderTarget->Release();

	delete image;
	image = nullptr;

	delete game;
	game = nullptr;

	delete ui;
	ui = nullptr;

	delete mouse;
	mouse = nullptr;

}


/*==== �E�B���h�E�n���h���̃Q�b�^�[ ====*/
HWND Window::GetHWND() {

	return hwnd;

}


/*==== �����_�[�^�[�Q�b�g�̃Q�b�^�[ ====*/
ID2D1HwndRenderTarget* Window::GetRenderTarget() {

	return pRenderTarget;

}
