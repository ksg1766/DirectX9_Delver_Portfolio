#include "stdafx.h"
#include "SoundManager.h"
#include "FIreWave2.h"
#include "Export_Function.h"
#include "SkeletonKing.h"
#include "BossFireWave.h"
#include "Boss_WarningEff.h"
CFIreWave2::CFIreWave2()
{
}

CFIreWave2::CFIreWave2(LPDIRECT3DDEVICE9 pGraphicDev)
	:CState(pGraphicDev)
{
}

CFIreWave2::~CFIreWave2()
{
}

HRESULT CFIreWave2::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;
	m_fDelay = 0.f;
	m_fPatternDelay = 0.f;
	m_iCount = 0;
	m_bWarning = false;
	m_bSound = false;
	return S_OK;
}

STATE CFIreWave2::Update_State(const _float& fTimeDelta)
{
	Engine::CGameObject* pGameObject = nullptr;
	m_fDelay += fTimeDelta;
	if ((1.5 > m_fDelay) && (!m_bWarning))
	{
		for (int i = 0; i < 18; ++i)
		{
			pGameObject = CBoss_WarningEff::Create(m_pGraphicDev);
			static_cast<CBoss_WarningEff*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = m_pOwner->Get_Transform()->m_vInfo[INFO_POS] + (_vec3(-18 + (i * 2), 2.f, 8.f));
			Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
		}
		for (int i = 0; i < 18; ++i)
		{
			pGameObject = CBoss_WarningEff::Create(m_pGraphicDev);
			static_cast<CBoss_WarningEff*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = m_pOwner->Get_Transform()->m_vInfo[INFO_POS] + (_vec3(-18 + (i * 2), 6.f, 8.f));
			Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
		}
		m_bWarning = true;
	}
	if ((1.5f < m_fDelay)&&(m_bWarning))
	{
		if (!m_bSound)
		{
			m_bSound = true;
			CSoundManager::GetInstance()->StopSound(CHANNELID::SOUND_KINGSPIDER);
			CSoundManager::GetInstance()->PlaySound(L"Boss_FireWave1.wav", CHANNELID::SOUND_KINGSPIDER, 1.f);
		}
		for (int i = 0; i < 18; ++i)
		{
			pGameObject = CBossFireWave::Create(m_pGraphicDev);
			static_cast<CBossFireWave*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = m_pOwner->Get_Transform()->m_vInfo[INFO_POS] + (_vec3(-18 + (i * 2), 2.f, 5.f));
			static_cast<CBossFireWave*>(pGameObject)->Set_Dir(_vec3(0.f, 0.f, 0.5f));
			Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
		}
		for (int i = 0; i < 18; ++i)
		{
			pGameObject = CBossFireWave::Create(m_pGraphicDev);
			static_cast<CBossFireWave*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = m_pOwner->Get_Transform()->m_vInfo[INFO_POS] + (_vec3(-18 + (i * 2), 6.f, 50.f));
			static_cast<CBossFireWave*>(pGameObject)->Set_Dir(_vec3(0.f, 0.f, -0.3f));
			Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
		}
		m_fDelay = 0.f;
		m_bSound = false;
		++m_iCount;
	}
	if (3 < m_iCount)
	{
		m_bWarning = false;
		m_iCount = 0;
		return STATE::BOSS_IDLE;
	}
}

void CFIreWave2::LateUpdate_State()
{
}

void CFIreWave2::Render_State()
{
}

void CFIreWave2::Create_Clone(_int _iNum)
{
}

CFIreWave2* CFIreWave2::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CFIreWave2* pState = new CFIreWave2(pGraphicDev);
	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CFIreWave2*>(pState);

		MSG_BOX("FireWave State Failed");
	}
	return pState;
}

void CFIreWave2::Free()
{
	__super::Free();
}
