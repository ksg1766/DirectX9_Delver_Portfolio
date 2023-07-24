#include "FIreWave2.h"
#include "Export_Function.h"
#include "SkeletonKing.h"
#include "BossFireWave.h"

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
	return S_OK;
}

STATE CFIreWave2::Update_State(const _float& fTimeDelta)
{
	Engine::CGameObject* pGameObject = nullptr;
	m_fDelay += fTimeDelta;
	if (1.5f < m_fDelay)
	{
		for (int i = 0; i < 18; ++i)
		{
			pGameObject = CBossFireWave::Create(m_pGraphicDev);
			dynamic_cast<CBossFireWave*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = m_pOwner->Get_Transform()->m_vInfo[INFO_POS] + (_vec3(-18 + (i * 2), 2.f, 5.f));
			dynamic_cast<CBossFireWave*>(pGameObject)->Set_Dir(_vec3(0.f, 0.f, 0.5f));
			Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
		}
		for (int i = 0; i < 18; ++i)
		{
			pGameObject = CBossFireWave::Create(m_pGraphicDev);
			dynamic_cast<CBossFireWave*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = m_pOwner->Get_Transform()->m_vInfo[INFO_POS] + (_vec3(-18 + (i * 2), 6.f, 50.f));
			dynamic_cast<CBossFireWave*>(pGameObject)->Set_Dir(_vec3(0.f, 0.f, -0.3f));
			Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
		}
		m_fDelay = 0.f;
		++m_iCount;
	}
	if (3 < m_iCount)
	{
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
