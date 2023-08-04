#include "Boss_CloneMeteorState.h"
#include "Export_Function.h"
#include "SkeletonKing_CloneMeteor.h"
#include "BossProjectile.h"
#include "SkeletonKing.h"
#include "Player.h"
CBoss_CloneMeteorState::CBoss_CloneMeteorState()
{
}

CBoss_CloneMeteorState::CBoss_CloneMeteorState(LPDIRECT3DDEVICE9 pGraphicDev)
	:CState(pGraphicDev)
{
}

CBoss_CloneMeteorState::~CBoss_CloneMeteorState()
{
}

HRESULT CBoss_CloneMeteorState::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;
    m_fDelay = 0.f;
	return S_OK;
}

STATE CBoss_CloneMeteorState::Update_State(const _float& fTimeDelta)
{
    if (STATE::BOSS_PH2SKILL5 != static_cast<CSkeletonKing*>(SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front())->Get_StateMachine()->Get_State())
        return STATE::DEAD;
     if (STATE::BOSS_IDLE == static_cast<CSkeletonKing*>(SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front())->Get_StateMachine()->Get_State())
        return STATE::DEAD;

    m_fDelay += fTimeDelta;
    if ((3.f < m_fDelay)&&(static_cast<CSkeletonKing_CloneMeteor*>(m_pOwner->Get_Host())->Get_bMove()))
    {
        Engine::CGameObject* pGameObject = nullptr;
        pGameObject = CBossProjectile::Create(m_pGraphicDev);
        static_cast<CBossProjectile*>(pGameObject)->Set_Target(m_pOwner->Get_Transform()->m_vInfo[INFO_POS]);
        Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
        m_fDelay = 0.f;
	    return STATE::IDLE;
    }
}

void CBoss_CloneMeteorState::LateUpdate_State()
{
}

void CBoss_CloneMeteorState::Render_State()
{
}

CBoss_CloneMeteorState* CBoss_CloneMeteorState::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
    CBoss_CloneMeteorState* pState = new CBoss_CloneMeteorState(pGraphicDev);
    if (FAILED(pState->Ready_State(pOwner)))
    {
        Safe_Release<CBoss_CloneMeteorState*>(pState);

        MSG_BOX("Boss Attack State Failed");
    }
    return pState;
}

void CBoss_CloneMeteorState::Free()
{
    __super::Free();
}
