#include "audio_manager.h"


/*
	private
*/

void AudioManager::LoadWavs(const std::vector<std::wstring> filepath) {

	// filepathがある限り、LoadWav()を呼び出し続ける
	for (auto& path : filepath) {

		LoadWav(path);

	}

}


void AudioManager::LoadWav(std::wstring path) {

	// ファイルをバイナリ形式で開く
	std::ifstream file(path, std::ios::binary | std::ios::ate);

	if (!file.is_open()) {

		OutputDebugStringW(L"Failed to Open WAV File.\n");
		return;

	}


	// ファイルサイズを取得
	std::streampos fileSize = static_cast<std::streamoff>(file.tellg());
	file.seekg(0, std::ios::beg);


	// バッファを確保してファイル全体を読み込む
	std::vector<char> buffer(fileSize);
	if (!file.read(buffer.data(), fileSize)) {

		OutputDebugStringW(L"Failed to Read WAV File.\n");
		return;

	}


	// IXAudio2SourceVoiceの作成
	IXAudio2SourceVoice* sourceVoice = nullptr;

	// WaveFormatの設定
	WAVEFORMATEX waveFormat;
	ZeroMemory(&waveFormat, sizeof(WAVEFORMATEX));

	waveFormat.cbSize = 0;
	waveFormat.wFormatTag = WAVE_FORMAT_PCM; // または他のフォーマット
	waveFormat.nChannels = 2; // ステレオなら2
	waveFormat.nSamplesPerSec = 44100; // サンプリングレート
	waveFormat.wBitsPerSample = 16; // ビット数
	waveFormat.nBlockAlign = (waveFormat.nChannels * waveFormat.wBitsPerSample) / 8;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;


	if (FAILED(pXAudio->CreateSourceVoice(&sourceVoice, &waveFormat))) {
		DWORD error = GetLastError();
		OutputDebugStringW((L"Failed to create source voice. Error: " + std::to_wstring(error) + L"\n").c_str());
		return;
	}


	// XAUDIO2_BUFFERの設定
	XAUDIO2_BUFFER audioBuffer = {};
	audioBuffer.AudioBytes = static_cast<UINT32>(fileSize);
	audioBuffer.pAudioData = reinterpret_cast<BYTE*>(buffer.data());
	audioBuffer.Flags = XAUDIO2_END_OF_STREAM;


	// Vector配列に格納
	sourceVoises.push_back(sourceVoice);
	buffers.push_back(audioBuffer);

}


/*
	public
*/

AudioManager::AudioManager() {

	// COMライブラリの初期化
	if (FAILED(CoInitializeEx(nullptr, COINITBASE_MULTITHREADED))) {
		OutputDebugStringW(L"Failed to initilalize COM Library\n");
	}


	// XAudio2を初期化
	if (FAILED(XAudio2Create(&pXAudio, 0, XAUDIO2_DEFAULT_PROCESSOR))) {
		OutputDebugStringW(L"Failed to initialize XAudio2.\n");
	}


	// マスターボイスの生成
	if (FAILED(pXAudio->CreateMasteringVoice(&pMasterVoice))) {
		OutputDebugStringW(L"Failed to create mastervoice.\n");
	}

	// 音声ファイルへのパス
	std::vector<std::wstring> filepath;
	filepath.push_back(L"src/sounds/hit.wav");

	LoadWavs(filepath);

}


AudioManager::~AudioManager() {

	// XAudioの開放
	if (pXAudio) {
		pXAudio->Release();
		CoUninitialize();
	}


	// ソースボイスの開放
	for (auto& voice : sourceVoises) {
		if (voice) {
			voice->DestroyVoice();
		}
	}


	// バッファの開放
	for (auto& buf : buffers) {
		if (buf.pAudioData) {
			delete[] buf.pAudioData;
		}
	}

}


void AudioManager::PlayWav(Situation index) {

	// インデックスが有効範囲内か
	if (index < 0 || index >= static_cast<int>(sourceVoises.size())) {

		OutputDebugStringW(L"Invalid index for audio playback.\n");
		return;

	}


	// ソースボイスの取得
	IXAudio2SourceVoice* sv = sourceVoises[index];

	// バッファの取得
	XAUDIO2_BUFFER buf = buffers[index];


	// ソースボイスにバッファを提出
	if (FAILED(sv->SubmitSourceBuffer(&buf))) {

		OutputDebugStringW(L"Failed to submit source buffer for playback.\n");
		return;

	}


	// 音声の再生
	if (FAILED(sv->Start())) {

		OutputDebugStringW(L"Failed to start source voice for playback.\n");
		return;

	}
	else {
		OutputDebugStringW(L"Hit!\n");
	}

}