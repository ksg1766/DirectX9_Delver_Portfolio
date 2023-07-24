#include "LostSoulPattern2.h"
#include "Export_Function.h"
#include "SkeletonKing.h"
#include "Boss_LostSoul.h"
#include "Player.h"
CLostSoulPattern2::CLostSoulPattern2()
{
}

CLostSoulPattern2::CLostSoulPattern2(LPDIRECT3DDEVICE9 pGraphicDev)
	:CState(pGraphicDev)
{
}

CLostSoulPattern2::~CLostSoulPattern2()
{
}

HRESULT CLostSoulPattern2::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;
	m_fDelay = 0.f;
	m_fPatternDelay = 0.f;
	m_iSkillCount = 0;
	m_vLaunchPos[0] = _vec3(16.f, 0.f, 0.f);
	m_vLaunchPos[1] = _vec3(10.f, 5.f, 0.f);
	m_vLaunchPos[2] = _vec3(0.f, 10.f, 0.f);
	m_vLaunchPos[3] = _vec3(-10.f, 5.f, 0.f);
	m_vLaunchPos[4] = _vec3(-16.f, 0.f, 0.f);

	return S_OK;
}

STATE CLostSoulPattern2::Update_State(const _float& fTimeDelta)
{
	Engine::CGameObject* pGameObject = nullptr;
	m_fDelay += fTimeDelta;
	if (1.f < m_fDelay)
	{
		pGameObject = CBossLostSoul::Create(m_pGraphicDev);
		dynamic_cast<CBossLostSoul*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = m_pOwner->Get_Transform()->m_vInfo[INFO_POS];
		dynamic_cast<CBossLostSoul*>(pGameObject)->Set_Target(m_pOwner->Get_Transform()->m_vInfo[INFO_POS]+ m_vLaunchPos[m_iSkillCount]);
		Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
	
		m_fDelay = 0.f;
		++m_iSkillCount;
	}

	if (5 <= m_iSkillCount)
	{
		m_iSkillCount = 0.f;
		return STATE::BOSS_IDLE;
	}

}

void CLostSoulPattern2::LateUpdate_State()
{
}

void CLostSoulPattern2::Render_State()
{
}

CLostSoulPattern2* CLostSoulPattern2::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CLostSoulPattern2* pState = new CLostSoulPattern2(pGraphicDev);
	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CLostSoulPattern2*>(pState);

		MSG_BOX("LostSoul State Failed");
	}
	return pState;
}

void CLostSoulPattern2::Free()
{
	__super::Free();
}
