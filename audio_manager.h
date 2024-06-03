#pragma once

#include <xaudio2.h>
#pragma comment(lib, "xaudio2.lib")

#include <string>
#include <vector>

#include <fstream>

// ��(��FHit = "�U����"�A Damage = "��e��"�Ȃ�)
enum Situation {
	Hit = 0,
	damage
};


class AudioManager {
private:


	IXAudio2* pXAudio;
	IXAudio2MasteringVoice* pMasterVoice;
	std::vector<IXAudio2SourceVoice*> sourceVoises;		// ��������
	std::vector<XAUDIO2_BUFFER> buffers;				// ��������


	void LoadWavs(const std::vector<std::wstring>);
	void LoadWav(std::wstring);


public:

	AudioManager();
	~AudioManager();

	void PlayWav(Situation);


};