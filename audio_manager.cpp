#include "audio_manager.h"


/*
	private
*/

void AudioManager::LoadWavs(const std::vector<std::wstring> filepath) {

	// filepath���������ALoadWav()���Ăяo��������
	for (auto& path : filepath) {

		LoadWav(path);

	}

}


void AudioManager::LoadWav(std::wstring path) {

	// �t�@�C�����o�C�i���`���ŊJ��
	std::ifstream file(path, std::ios::binary | std::ios::ate);

	if (!file.is_open()) {

		OutputDebugStringW(L"Failed to Open WAV File.\n");
		return;

	}


	// �t�@�C���T�C�Y���擾
	std::streampos fileSize = static_cast<std::streamoff>(file.tellg());
	file.seekg(0, std::ios::beg);


	// �o�b�t�@���m�ۂ��ăt�@�C���S�̂�ǂݍ���
	std::vector<char> buffer(fileSize);
	if (!file.read(buffer.data(), fileSize)) {

		OutputDebugStringW(L"Failed to Read WAV File.\n");
		return;

	}


	// IXAudio2SourceVoice�̍쐬
	IXAudio2SourceVoice* sourceVoice = nullptr;

	// WaveFormat�̐ݒ�
	WAVEFORMATEX waveFormat;
	ZeroMemory(&waveFormat, sizeof(WAVEFORMATEX));

	waveFormat.cbSize = 0;
	waveFormat.wFormatTag = WAVE_FORMAT_PCM; // �܂��͑��̃t�H�[�}�b�g
	waveFormat.nChannels = 2; // �X�e���I�Ȃ�2
	waveFormat.nSamplesPerSec = 44100; // �T���v�����O���[�g
	waveFormat.wBitsPerSample = 16; // �r�b�g��
	waveFormat.nBlockAlign = (waveFormat.nChannels * waveFormat.wBitsPerSample) / 8;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;


	if (FAILED(pXAudio->CreateSourceVoice(&sourceVoice, &waveFormat))) {
		DWORD error = GetLastError();
		OutputDebugStringW((L"Failed to create source voice. Error: " + std::to_wstring(error) + L"\n").c_str());
		return;
	}


	// XAUDIO2_BUFFER�̐ݒ�
	XAUDIO2_BUFFER audioBuffer = {};
	audioBuffer.AudioBytes = static_cast<UINT32>(fileSize);
	audioBuffer.pAudioData = reinterpret_cast<BYTE*>(buffer.data());
	audioBuffer.Flags = XAUDIO2_END_OF_STREAM;


	// Vector�z��Ɋi�[
	sourceVoises.push_back(sourceVoice);
	buffers.push_back(audioBuffer);

}


/*
	public
*/

AudioManager::AudioManager() {

	// COM���C�u�����̏�����
	if (FAILED(CoInitializeEx(nullptr, COINITBASE_MULTITHREADED))) {
		OutputDebugStringW(L"Failed to initilalize COM Library\n");
	}


	// XAudio2��������
	if (FAILED(XAudio2Create(&pXAudio, 0, XAUDIO2_DEFAULT_PROCESSOR))) {
		OutputDebugStringW(L"Failed to initialize XAudio2.\n");
	}


	// �}�X�^�[�{�C�X�̐���
	if (FAILED(pXAudio->CreateMasteringVoice(&pMasterVoice))) {
		OutputDebugStringW(L"Failed to create mastervoice.\n");
	}

	// �����t�@�C���ւ̃p�X
	std::vector<std::wstring> filepath;
	filepath.push_back(L"src/sounds/hit.wav");

	LoadWavs(filepath);

}


AudioManager::~AudioManager() {

	// XAudio�̊J��
	if (pXAudio) {
		pXAudio->Release();
		CoUninitialize();
	}


	// �\�[�X�{�C�X�̊J��
	for (auto& voice : sourceVoises) {
		if (voice) {
			voice->DestroyVoice();
		}
	}


	// �o�b�t�@�̊J��
	for (auto& buf : buffers) {
		if (buf.pAudioData) {
			delete[] buf.pAudioData;
		}
	}

}


void AudioManager::PlayWav(Situation index) {

	// �C���f�b�N�X���L���͈͓���
	if (index < 0 || index >= static_cast<int>(sourceVoises.size())) {

		OutputDebugStringW(L"Invalid index for audio playback.\n");
		return;

	}


	// �\�[�X�{�C�X�̎擾
	IXAudio2SourceVoice* sv = sourceVoises[index];

	// �o�b�t�@�̎擾
	XAUDIO2_BUFFER buf = buffers[index];


	// �\�[�X�{�C�X�Ƀo�b�t�@���o
	if (FAILED(sv->SubmitSourceBuffer(&buf))) {

		OutputDebugStringW(L"Failed to submit source buffer for playback.\n");
		return;

	}


	// �����̍Đ�
	if (FAILED(sv->Start())) {

		OutputDebugStringW(L"Failed to start source voice for playback.\n");
		return;

	}
	else {
		OutputDebugStringW(L"Hit!\n");
	}

}