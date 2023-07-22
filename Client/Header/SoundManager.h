#pragma once
#include "Engine_Define.h"
#include "Base.h"
#include "../fmode/inc/fmod.h"

class CSoundManager : public CBase
{
	DECLARE_SINGLETON(CSoundManager)

private:
	explicit CSoundManager();
	virtual ~CSoundManager();

public:
	HRESULT ReadySound(CSoundManager** ppSceneManager);
	HRESULT LoadSound(const char* pPath, const char* pKey);

public:
	void PlaySound(TCHAR* pSoundKey, CHANNELID eID, float fVolume);
	void PlayBGM(TCHAR* pSoundKey, float fVolume);
	void StopSound(CHANNELID eID);
	void StopAll();
	void SetChannelVolume(CHANNELID eID, float fVolume);

private:
	FMOD_SYSTEM*  m_pSystem = nullptr;
	FMOD_CHANNEL* m_pChannelArr[CHANNELID::MAXCHANNEL];
	map<TCHAR*, FMOD_SOUND*> m_mapSound;

private:
	virtual void Free() override;

};

