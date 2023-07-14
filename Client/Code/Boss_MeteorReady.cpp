#include "Boss_MeteorReady.h"
#include "Export_Function.h"
#include "SkeletonKing.h"
#include "Boss_MeteorCube.h"

CBoss_MeteorReady::CBoss_MeteorReady()
{
}

CBoss_MeteorReady::CBoss_MeteorReady(LPDIRECT3DDEVICE9 pGraphicDev)
	:CState(pGraphicDev)
{
}

CBoss_MeteorReady::~CBoss_MeteorReady()
{
}

HRESULT CBoss_MeteorReady::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;
	m_fChannel_Count = 0.f;
	m_bSkillStart = false;
	return S_OK;
}

STATE CBoss_MeteorReady::Update_State(const _float& fTimeDelta)
{
	m_fChannel_Count += fTimeDelta;
	Engine::CGameObject* pGameObject = nullptr;
	if (!m_bSkillStart)
	{
		pGameObject = m_pGameObject = CBoss_MeteorCube::Create(m_pGraphicDev);
		Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
		dynamic_cast<CBoss_MeteorCube*>(pGameObject)->Set_Center(m_pOwner->Get_Transform()->m_vInfo[INFO_POS]);
		dynamic_cast<CBoss_MeteorCube*>(pGameObject)->Set_State(STATE::BOSS_METEORREADY);
		dynamic_cast<CBoss_MeteorCube*>(pGameObject)->Channeling_Begin();
		m_bSkillStart = true;
	}
	else if (m_bSkillStart && (10.1f < m_fChannel_Count))
	{
		m_bSkillStart = false;
		m_fChannel_Count = 0.f;
		return STATE::BOSS_IDLE;
	}
	else if(m_bSkillStart)
	{
		m_pOwner->Get_Transform()->Translate(_vec3(0.f, 0.5f * fTimeDelta, 0.f));

	}


}

void CBoss_MeteorReady::LateUpdate_State()
{
}

void CBoss_MeteorReady::Render_State()
{
}

CBoss_MeteorReady* CBoss_MeteorReady::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
	CBoss_MeteorReady* pState = new CBoss_MeteorReady(pGraphicDev);
	if (FAILED(pState->Ready_State(pOwner)))
	{
		Safe_Release<CBoss_MeteorReady*>(pState);

		MSG_BOX("Boss Meteor Ready Failed");
	}
	return pState;
}

void CBoss_MeteorReady::Free()
{
}
