#include "stdafx.h"
#include "SoundManager.h"

IMPLEMENT_SINGLETON(CSoundManager)

CSoundManager::CSoundManager()
{
}

CSoundManager::~CSoundManager()
{
	Free();
}

HRESULT CSoundManager::ReadySound(CSoundManager** ppSoundManager)
{
	CSoundManager* pSoundManager = CSoundManager::GetInstance();
	NULL_CHECK_RETURN(pSoundManager, E_FAIL);

	*ppSoundManager = pSoundManager;

	FMOD_System_Create(&m_pSystem);

	FMOD_System_Init(m_pSystem, 32, FMOD_INIT_NORMAL, NULL);

	return S_OK;
}
 
HRESULT CSoundManager::LoadSound(const char* pPath, const char* pKey)
{
	FMOD_SOUND* pSound = nullptr;
	FMOD_RESULT eRes = FMOD_System_CreateSound(m_pSystem, pPath, FMOD_HARDWARE, 0, &pSound);

	int iLength = strlen(pKey) + 1;
	TCHAR* pSoundKey = new TCHAR[iLength];
	ZeroMemory(pSoundKey, sizeof(TCHAR) * iLength);

	MultiByteToWideChar(CP_ACP, 0, pKey, iLength, pSoundKey, iLength);

	m_mapSound.emplace(pSoundKey, pSound);

	return S_OK;
}

void CSoundManager::PlaySound(TCHAR * pSoundKey, CHANNELID eID, float fVolume)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	// iter = find_if(m_mapSound.begin(), m_mapSound.end(), CTag_Finder(pSoundKey));
	iter = find_if(m_mapSound.begin(), m_mapSound.end(),
		[&](auto& iter)->bool
	{
		return !lstrcmp(pSoundKey, iter.first);
	});

	if (iter == m_mapSound.end())
		return;

	FMOD_BOOL bPlay = FALSE;

	if (FMOD_Channel_IsPlaying(m_pChannelArr[eID], &bPlay))
	{
		FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second, FALSE, &m_pChannelArr[eID]);
	}

	FMOD_Channel_SetVolume(m_pChannelArr[eID], fVolume);

	FMOD_System_Update(m_pSystem);
}

void CSoundManager::PlayBGM(TCHAR * pSoundKey, float fVolume)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	// iter = find_if(m_mapSound.begin(), m_mapSound.end(), CTag_Finder(pSoundKey));
	iter = find_if(m_mapSound.begin(), m_mapSound.end(), [&](auto& iter)->bool
	{
		return !lstrcmp(pSoundKey, iter.first);
	});

	if (iter == m_mapSound.end())
		return;

	FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second, FALSE, &m_pChannelArr[SOUND_BGM]);
	FMOD_Channel_SetMode(m_pChannelArr[SOUND_BGM], FMOD_LOOP_NORMAL);
	FMOD_Channel_SetVolume(m_pChannelArr[SOUND_BGM], fVolume);
	FMOD_System_Update(m_pSystem);
}

void CSoundManager::StopSound(CHANNELID eID)
{
	FMOD_Channel_Stop(m_pChannelArr[eID]);
}

void CSoundManager::StopAll()
{
	for (int i = 0; i < SOUND_MAXCHANNEL; ++i)
		FMOD_Channel_Stop(m_pChannelArr[i]);
}

void CSoundManager::SetChannelVolume(CHANNELID eID, float fVolume)
{
	FMOD_Channel_SetVolume(m_pChannelArr[eID], fVolume);

	FMOD_System_Update(m_pSystem);
}

void CSoundManager::Free()
{
	for (auto& Mypair : m_mapSound)
	{
		delete[] Mypair.first;
		FMOD_Sound_Release(Mypair.second);
	}
	m_mapSound.clear();

	FMOD_System_Release(m_pSystem);
	FMOD_System_Close(m_pSystem);
}
