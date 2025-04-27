#pragma once
#include "CRes.h"

#include <FMOD/fmod.h>
#include <FMOD/fmod.hpp>
#include <FMOD/fmod_codec.h>

#ifdef _DEBUG
#pragma comment(lib, "FMOD/fmodL64_vc.lib")
#else
#pragma comment(lib, "FMOD/fmod64_vc.lib")
#endif

#define SOUND_MAX 1.0f
#define SOUND_MIN 0.0f
#define SOUND_DEFAULT 0.5f
#define SOUND_WEIGHT 0.1f

class CSound :
	public CRes
{
public:
	static FMOD::System* g_pFMOD; //사운드 객체를 생성할 곳인 시스템

private:
	FMOD_MODE				m_mode;//1번 Default, 반복 출력시 Loop
	FMOD::Sound*			m_pSound;//사운드 파일을 저장할 사운드 객체
	FMOD::Channel*			m_channel;
	list<FMOD::Channel*>	m_listChannel;

	float m_volume;

public:
	// 0 (무한반복) 0 ~ 1(Volume)
	int Play(int _iRoopCount, float _fVolume = 0.5f, int _iIdx = -1, bool _bOverlap = false);
	int PlayBGM(int _iRoopCount, float _fVolume = 1.f, int _iIdx = -1, bool _bOverlap = false);
	int PlaySFX(int _iRoopCount, float _fVolume = 1.f, int _iIdx = -1, bool _bOverlap = false);

	int pause();
	void Stop();
	int volumeUp();
	int volumeDown();
	float GetVolume() { return m_volume; }
	FMOD::Sound* GetSound() { return m_pSound; }
	// 0 ~ 1
	void SetVolume(float _f, int _iChannelIdx);

	bool IsChannelPlaying(FMOD::Channel* channel);

	list<FMOD::Channel*> GetChannelList() { return m_listChannel; }
private:
	void RemoveChannel(FMOD::Channel* _pTargetChannel);
	friend FMOD_RESULT CHANNEL_CALLBACK(FMOD_CHANNELCONTROL* channelcontrol, FMOD_CHANNELCONTROL_TYPE controltype
		, FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype
		, void* commanddata1, void* commanddata2);

public:
	virtual void UpdateData() {}

	// 파일로 저장
	virtual int Save(const wstring&) override { return S_OK; }

	// 파일로부터 로딩
	virtual int Load(const wstring& _strFilePath) override;
		
public:
	CSound();
	virtual ~CSound();
	friend class CSound;
};

