#include "Boss_Meteor1Ph.h"
#include "Export_Function.h"
#include "SkeletonKing.h"
#include "Boss_MeteorCube.h"
#include "Boss_LostSoul.h"

CBoss_MeteorPh1::CBoss_MeteorPh1()
{
}

CBoss_MeteorPh1::CBoss_MeteorPh1(LPDIRECT3DDEVICE9 pGraphicDev)
	:CState(pGraphicDev)
{
}

CBoss_MeteorPh1::~CBoss_MeteorPh1()
{
}

HRESULT CBoss_MeteorPh1::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;
	m_fChannel_Count = 0.f;
	m_fDelay = 0.f;
	m_bSkillStart = false;
	return S_OK;
}

STATE CBoss_MeteorPh1::Update_State(const _float& fTimeDelta)
{
	m_fChannel_Count += fTimeDelta;
	m_fDelay += fTimeDelta;
	Engine::CGameObject* pGameObject = nullptr;
	if (!m_bSkillStart)
	{
		pGameObject = m_pGameObject = CBoss_MeteorCube::Create(m_pGraphicDev);
		Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
		static_cast<CBoss_MeteorCube*>(pGameObject)->Set_Center(m_pOwner->Get_Transform()->m_vInfo[INFO_POS]);
		static_cast<CBoss_MeteorCube*>(pGameObject)->Set_State(STATE::BOSS_PH1SKILL5);
		static_cast<CBoss_MeteorCube*>(pGameObject)->Channeling_Begin();
		m_bSkillStart = true;
	}
	else if (m_bSkillStart && (12.1f < m_fChannel_Count))
	{
		m_bSkillStart = false;
		m_fChannel_Count = 0.f;
		return STATE::BOSS_ATTACK1;
	}
	else if(m_bSkillStart)
	{
		m_pOwner->Get_Transform()->Translate(_vec3(0.f, 0.5f * fTimeDelta, 0.f));

	}
	if (2.f < m_fDelay)
	{
		pGameObject = CBossLostSoul::Create(m_pGraphicDev);
		static_cast<CBossLostSoul*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = m_pOwner->Get_Transform()->m_vInfo[INFO_POS];
		static_cast<CBossLostSoul*>(pGameObject)->Set_Target(m_pOwner->Get_Transform()->m_vInfo[INFO_POS]);
		Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
		m_fDelay = 0.f;
	}
}

void CBoss_MeteorPh1::LateUpdate_State()
{
}

void CBoss_MeteorPh1::Render_State()
{
}

CBoss_MeteorPh1* CBoss_MeteorPh1::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CBoss_MeteorPh1* pState = new CBoss_MeteorPh1(pGraphicDev);
	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CBoss_MeteorPh1*>(pState);

		MSG_BOX("Boss Meteor Ready Failed");
	}
	return pState;
}

void CBoss_MeteorPh1::Free()
{
	__super::Free();
}
