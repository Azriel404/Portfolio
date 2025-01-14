#pragma once

#include <xaudio2.h>
#pragma comment(lib, "xaudio2.lib")

#include <string>
#include <vector>

#include <fstream>

// 状況(例：Hit = "攻撃音"、 Damage = "被弾音"など)
enum Situation {
	Hit = 0,
	damage
};


class AudioManager {
private:


	IXAudio2* pXAudio;
	IXAudio2MasteringVoice* pMasterVoice;
	std::vector<IXAudio2SourceVoice*> sourceVoises;		// 音声制御
	std::vector<XAUDIO2_BUFFER> buffers;				// 流す音声


	void LoadWavs(const std::vector<std::wstring>);
	void LoadWav(std::wstring);


public:

	AudioManager();
	~AudioManager();

	void PlayWav(Situation);


};