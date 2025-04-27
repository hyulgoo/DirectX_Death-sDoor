#include "pch.h"
#include "CSound.h"

#include "CResMgr.h"

FMOD_RESULT CHANNEL_CALLBACK(FMOD_CHANNELCONTROL* channelcontrol, FMOD_CHANNELCONTROL_TYPE controltype
	, FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype
	, void* commanddata1, void* commanddata2);

FMOD::System* CSound::g_pFMOD = nullptr;

CSound::CSound()
	: CRes(RES_TYPE::SOUND)
	, m_pSound(nullptr)
	, m_volume(0.f)
{
}

CSound::~CSound()
{
	if (nullptr != m_pSound)
	{
		m_pSound->release();
		m_pSound = nullptr;
	}
}

int CSound::Play(int _iRoopCount, float _fVolume, int _iIdx, bool _bOverlap)
{
	if (_iRoopCount <= -1)
	{
		assert(nullptr);
	}

	// 재생되고 있는 채널이 있는데, 중복재생을 허용하지 않았다 -> 재생 안함
	if (!_bOverlap && !m_listChannel.empty())
	{
		return -1;
	}

	_iRoopCount -= 1;

	FMOD::Channel* pChannel = nullptr;
	g_pFMOD->playSound(m_pSound, nullptr, false, &pChannel);

	pChannel->setVolume(_fVolume);
	m_volume = _fVolume;
	pChannel->setCallback(CHANNEL_CALLBACK);
	pChannel->setUserData(this);

	pChannel->setMode(FMOD_LOOP_NORMAL);
	pChannel->setLoopCount(_iRoopCount);

	m_listChannel.push_back(pChannel);

	return _iIdx;
}

int CSound::PlayBGM(int _iRoopCount, float _fVolume, int _iIdx, bool _bOverlap )
{
	if (_iRoopCount <= -1)
	{
		assert(nullptr);
	}

	// 재생되고 있는 채널이 있는데, 중복재생을 허용하지 않았다 -> 재생 안함
	if (!_bOverlap && !m_listChannel.empty())
	{
		return -1;
	}

	_iRoopCount -= 1;
	
	FMOD::Channel* pChannel = nullptr;
	g_pFMOD->playSound(m_pSound, nullptr, false, &pChannel);

	pChannel->setVolume(_fVolume);
	m_volume = _fVolume;
	pChannel->setCallback(CHANNEL_CALLBACK);
	pChannel->setUserData(this);

	pChannel->setMode(FMOD_LOOP_NORMAL);
	pChannel->setLoopCount(_iRoopCount);

	m_listChannel.push_back(pChannel);

	FMOD::ChannelGroup* channelBGM = nullptr;
	g_pFMOD->createChannelGroup("BGM", &channelBGM);
	pChannel->setChannelGroup(channelBGM);
	pChannel->getIndex(&_iIdx);

	return _iIdx;
}
int CSound::PlaySFX(int _iRoopCount, float _fVolume, int _iIdx, bool _bOverlap)
{
	if (_iRoopCount <= -1)
	{
		assert(nullptr);
	}

	// 재생되고 있는 채널이 있는데, 중복재생을 허용하지 않았다 -> 재생 안함
	if (!_bOverlap && !m_listChannel.empty())
	{
		return -1;
	}

	_iRoopCount -= 1;

	FMOD::Channel* pChannel = nullptr;
	g_pFMOD->playSound(m_pSound, nullptr, false, &pChannel);

	pChannel->setVolume(_fVolume);
	m_volume = _fVolume;
	pChannel->setCallback(CHANNEL_CALLBACK);
	pChannel->setUserData(this);

	pChannel->setMode(FMOD_LOOP_NORMAL);
	pChannel->setLoopCount(_iRoopCount);

	m_listChannel.push_back(pChannel);

	FMOD::ChannelGroup* channelSFX = nullptr;
	g_pFMOD->createChannelGroup("SFX", &channelSFX);
	pChannel->setChannelGroup(channelSFX);
	pChannel->getIndex(&_iIdx);

	return _iIdx;
}
int CSound::pause()
{
	m_channel[0].setPaused(true);

	return 0;
}

void CSound::Stop()
{
	list<FMOD::Channel*>::iterator iter;

	while (!m_listChannel.empty())
	{
		iter = m_listChannel.begin();
		(*iter)->stop();
	}
}

int CSound::volumeUp()
{
	if (m_volume < SOUND_MAX) {
		m_volume += SOUND_WEIGHT;
	}
	int a = 1;
	//m_channel = g_pFMOD->getChannelsPlaying(&a);
	m_channel[0].setVolume(m_volume);

	return 0;
}

int CSound::volumeDown()
{
	if (m_volume > SOUND_MIN) {
		m_volume -= SOUND_WEIGHT;
	}

	m_channel[0].setVolume(m_volume);

	return 0;
}

void CSound::SetVolume(float _f, int _iChannelIdx)
{
	list<FMOD::Channel*>::iterator iter = m_listChannel.begin();

	int iIdx = -1;
	for (; iter != m_listChannel.end(); ++iter)
	{
		(*iter)->getIndex(&iIdx);
		if (_iChannelIdx == iIdx)
		{
			(*iter)->setVolume(_f);
			return;
		}
	}
}

bool CSound::IsChannelPlaying(FMOD::Channel* channel)
{
	bool isPlaying = false;
	if (channel) {
		channel->isPlaying(&isPlaying);
	}
	return isPlaying;
}

void CSound::RemoveChannel(FMOD::Channel* _pTargetChannel)
{
	list<FMOD::Channel*>::iterator iter = m_listChannel.begin();
	for (; iter != m_listChannel.end(); ++iter)
	{
		if (*iter == _pTargetChannel)
		{
			m_listChannel.erase(iter);
			return;
		}
	}
}

int CSound::Load(const wstring& _strFilePath)
{
	string path(_strFilePath.begin(), _strFilePath.end());

	if (FMOD_OK != g_pFMOD->createSound(path.c_str(), FMOD_DEFAULT, nullptr, &m_pSound))
	{
		assert(nullptr);
	}

	return S_OK;
}



// =========
// Call Back
// =========
FMOD_RESULT CHANNEL_CALLBACK(FMOD_CHANNELCONTROL* channelcontrol, FMOD_CHANNELCONTROL_TYPE controltype
	, FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype
	, void* commanddata1, void* commanddata2)
{
	FMOD::Channel* cppchannel = (FMOD::Channel*)channelcontrol;
	CSound* pSound = nullptr;

	switch (controltype)
	{
	case FMOD_CHANNELCONTROL_CALLBACK_END:
	{
		cppchannel->getUserData((void**)&pSound);
		pSound->RemoveChannel(cppchannel);
	}
	break;
	}

	return FMOD_OK;
}