#include "Boss_Dead.h"
#include "Export_Function.h"
#include "SkeletonKing.h"
#include "DimensionGate.h"

CBoss_Dead::CBoss_Dead()
{
}

CBoss_Dead::CBoss_Dead(LPDIRECT3DDEVICE9 pGraphicDev)
    :CState(pGraphicDev)
{
}

CBoss_Dead::~CBoss_Dead()
{
}

HRESULT CBoss_Dead::Ready_State(CStateMachine* pOwner)
{
    m_pOwner = pOwner;
    m_bSpawnGate = false;
    return S_OK;
}

STATE CBoss_Dead::Update_State(const _float& fTimeDelta)
{
    if (!m_bSpawnGate)
    {
        m_bSpawnGate = true;
        CGameObject* pGameObject = nullptr;
        pGameObject = CDimensionGate::Create(m_pGraphicDev);
        dynamic_cast<CDimensionGate*>(pGameObject)->m_pTransform->m_vInfo[INFO_POS] = _vec3(-95.f, 35.f, 0.f);
        Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
    }

    return STATE::BOSS_DEAD;

}

void CBoss_Dead::LateUpdate_State()
{
}

void CBoss_Dead::Render_State()
{
}

CBoss_Dead* CBoss_Dead::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
    CBoss_Dead* pState = new CBoss_Dead(pGraphicDev);
    if (FAILED(pState->Ready_State(pOwner)))
    {
        Safe_Release<CBoss_Dead*>(pState);

        MSG_BOX("Boss Dead State Failed");
    }
    return pState;
}

void CBoss_Dead::Free()
{
    __super::Free();
}
