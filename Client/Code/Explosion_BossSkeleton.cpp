#include "Explosion_BossSkeleton.h"
#include "Export_Function.h"
#include "BossExplosion.h"

CExplosion_BossSkeleton::CExplosion_BossSkeleton()
{
}

CExplosion_BossSkeleton::CExplosion_BossSkeleton(LPDIRECT3DDEVICE9 pGraphicDev)
	:CState(pGraphicDev)
{
}

CExplosion_BossSkeleton::~CExplosion_BossSkeleton()
{
}

HRESULT CExplosion_BossSkeleton::Ready_State(CStateMachine* pOwner)
{
    m_pOwner = pOwner;
	m_bExplosion = false;
 false;
    return S_OK;
}

STATE CExplosion_BossSkeleton::Update_State(const _float& fTimeDelta)
{
    Engine::CGameObject* pGameObject = nullptr;
    pGameObject = CBossExplosion::Create(m_pGraphicDev);
    static_cast<CBossExplosion*>(pGameObject)->Set_StartPos(m_pOwner->Get_Transform()->m_vInfo[INFO_POS]);
    Engine::EventManager()->CreateObject(pGameObject, LAYERTAG::GAMELOGIC);
	    return STATE::DEAD;
}

void CExplosion_BossSkeleton::LateUpdate_State()
{
}

void CExplosion_BossSkeleton::Render_State()
{
}

CExplosion_BossSkeleton* CExplosion_BossSkeleton::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
    CExplosion_BossSkeleton* pState = new CExplosion_BossSkeleton(pGraphicDev);
    if (FAILED(pState->Ready_State(pOwner)))
    {
        Safe_Release<CExplosion_BossSkeleton*>(pState);

        MSG_BOX("BossSkeleton Explosion State Failed");
    }
    return pState;
}

void CExplosion_BossSkeleton::Free()
{
    __super::Free();
}
