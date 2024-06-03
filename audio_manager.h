#pragma once

#include <xaudio2.h>
#pragma comment(lib, "xaudio2.lib")

#include <string>
#include <vector>

#include <fstream>

// èÛãµ(ó·ÅFHit = "çUåÇâπ"ÅA Damage = "îÌíeâπ"Ç»Ç«)
enum Situation {
	Hit = 0,
	damage
};


class AudioManager {
private:


	IXAudio2* pXAudio;
	IXAudio2MasteringVoice* pMasterVoice;
	std::vector<IXAudio2SourceVoice*> sourceVoises;		// âπê∫êßå‰
	std::vector<XAUDIO2_BUFFER> buffers;				// ó¨Ç∑âπê∫


	void LoadWavs(const std::vector<std::wstring>);
	void LoadWav(std::wstring);


public:

	AudioManager();
	~AudioManager();

	void PlayWav(Situation);


};