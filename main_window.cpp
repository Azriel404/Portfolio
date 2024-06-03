#include "main_window.h"
#include <wchar.h>
#include <stdio.h>
#include <chrono>
#include <thread>

/*=====================
		private
=====================*/

Mouse* Window::mouse;					// マウスインスタンス(Staticなコールバック関数で利用するため、Static指定されている)
int Window::time;						// ゲームの経過時間
GameState Window::currentGameState;		// 現在のゲームの状況
bool Window::isPouse;					// ポーズフラグ

/*==== メインウィンドウの初期化関数 ====*/
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


	// 画面のサイズを取得
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// 取得した画面のサイズをもとに、ウィンドウの左上の座標を計算する
	int posX = (screenWidth - width) / 2;
	int posY = (screenHeight - height) / 2;


	// ウィンドウを作成する
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


/*==== Direct2Dの初期化関数 ====*/
HRESULT Window::Init2D() {

	HRESULT hr;

	/*==== ファクトリの作成 ====*/
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);
	if (FAILED(hr)) {
		return E_FAIL;
	}


	/*==== レンダーターゲットの設定 ====*/
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


/*==== メッセージループ関数 ====*/
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

				// FPSの制御
				std::this_thread::sleep_until(endTime);
				startTime = std::chrono::high_resolution_clock::now();
				endTime = startTime + frameTime;

				// 自拠点の体力が1以上か
				if (game->GetBaseInstance()->DefeatCheck()) {

					currentGameState = Result;
					isDefeat = true;

				}


				/*==== ゲームをクリアしたか？ ====*/
				if (!game->ClearCheck()) {

					// game code here
					pRenderTarget->BeginDraw();
					pRenderTarget->Clear();
					game->NowTime(Window::time);
					game->Draw();
					if (!isPouse) {		// ポーズフラグがfalseかどうか
						game->Update();
					}

					pRenderTarget->EndDraw();



					// デバッグ用


					// リザルト画面に移る
					if (GetAsyncKeyState('P') & 0x8000) {

						currentGameState = Result;

					}


					// 自拠点の体力を1にする
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
						 現在押されているキーを判定　0x8000は最上位ビットを表す
					*/

					// Yが押されていれば...
					if (GetAsyncKeyState('Y') & 0x8000) {

						// 敵を再生成
						game->ContinueGame();
						frag = true;

					}

					// Nが押されていれば...
					else if (GetAsyncKeyState('N') & 0x8000) {

						//リザルト画面へ
						currentGameState = Result;

					}

				}

				break;


			case Result:

				if (!isDefeat) {

					// 自拠点の体力が1以上の場合は、ウェーブ数に応じてボーナススコアを与える
					ui->ShowResultScreen(game->GetFontInstance(), game->GetScore() + (game->GetWaves() * 1000));

				}

				else {

					// 負けてしまったら、ボーナスはなし
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


	case WM_KEYDOWN:		// 何かキーを入力されたら...

		if (wParam == VK_ESCAPE) {		// Escキーが入力されたら...

			if (MessageBoxW(NULL, L"本当にウィンドウを閉じますか？", L"確認", MB_YESNO | MB_ICONQUESTION) == IDYES) {

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


				if (MessageBoxW(NULL, L"本当にウィンドウを閉じますか？", L"確認", MB_YESNO | MB_ICONQUESTION) == IDYES) {

					DestroyWindow(hwnd);

				}

			}

			break;

		}

		break;




	case WM_CLOSE:			// ウィンドウの×ボタンが押されたら...

		if (MessageBoxW(NULL, L"本当にウィンドウを閉じますか？", L"確認", MB_YESNO | MB_ICONQUESTION) == IDYES) {

			DestroyWindow(hwnd);

		}

		break;


	case WM_DESTROY:		// 何らかのウィンドウ破棄命令を受け取ったら...

		PostQuitMessage(0);

		break;


	default:				// 定義されていないメッセージを受け取ったら...

		return DefWindowProcW(hwnd, msg, wParam, lParam);		// デフォルトとして設定されているプロシージャを実行

	}

	return 0;

}



/*====================
		public
====================*/

/*==== コンストラクタ ====*/
Window::Window(HINSTANCE hInstance, int showWnd) :
	hwnd(NULL), hInstance(hInstance), windowed(true), width(1000), height(625),
	wcn(L"MainWindow"),
	pRenderTarget(nullptr)
{

	HRESULT hr;

	/*==== 初期化関数 ====*/
	hr = InitWindow(showWnd);
	hr = Init2D();

	time = 0;
	currentGameState = Start;
	isPouse = false;

	/*==== インスタンスの作成 ====*/
	image = new ImageManager(pRenderTarget);
	ui = new UIManager(pRenderTarget, image);
	audio = new AudioManager();
	game = new GameSystem(pRenderTarget, image, audio);


	mouse = new Mouse(game);

	/*==== メッセージループ関数 ====*/
	MessageLoop();

}


/*==== デストラクタ ====*/
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


/*==== ウィンドウハンドルのゲッター ====*/
HWND Window::GetHWND() {

	return hwnd;

}


/*==== レンダーターゲットのゲッター ====*/
ID2D1HwndRenderTarget* Window::GetRenderTarget() {

	return pRenderTarget;

}
