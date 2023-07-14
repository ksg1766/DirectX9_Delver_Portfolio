#include "Dead_BossSkeleton.h"
#include "Export_Function.h"
#include "BossExplosion.h"

CDead_BossSkeleton::CDead_BossSkeleton()
{
}

CDead_BossSkeleton::CDead_BossSkeleton(LPDIRECT3DDEVICE9 pGraphicDev)
	:CState(pGraphicDev)
{
}

CDead_BossSkeleton::~CDead_BossSkeleton()
{
}

HRESULT CDead_BossSkeleton::Ready_State(CStateMachine* pOwner)
{
    m_pOwner = pOwner;
    
 false;
    return S_OK;
}

STATE CDead_BossSkeleton::Update_State(const _float& fTimeDelta)
{
 
	    return STATE::DEAD;
}

void CDead_BossSkeleton::LateUpdate_State()
{
}

void CDead_BossSkeleton::Render_State()
{
}

CDead_BossSkeleton* CDead_BossSkeleton::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
    CDead_BossSkeleton* pState = new CDead_BossSkeleton(pGraphicDev);
    if (FAILED(pState->Ready_State(pOwner)))
    {
        Safe_Release<CDead_BossSkeleton*>(pState);

        MSG_BOX("BossSkeleton Explosion State Failed");
    }
    return pState;
}

void CDead_BossSkeleton::Free()
{
    __super::Free();
}
