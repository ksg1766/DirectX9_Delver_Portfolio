#include "WakeUp_BossSkeleton.h"
#include "Export_Function.h"
#include "Boss_Skeleton.h"

CWakeUp_BossSkeleton::CWakeUp_BossSkeleton()
{
}

CWakeUp_BossSkeleton::CWakeUp_BossSkeleton(LPDIRECT3DDEVICE9 pGraphicDev)
	:CState(pGraphicDev)
{
}

CWakeUp_BossSkeleton::~CWakeUp_BossSkeleton()
{
}

HRESULT CWakeUp_BossSkeleton::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;
    m_fSpeed = 4.f;
	return S_OK;
}

STATE CWakeUp_BossSkeleton::Update_State(const _float& fTimeDelta)
{
    if (0.f <= m_pOwner->Get_Transform()->m_vInfo[INFO_POS].y) { return STATE::ROMIMG; }

    m_pOwner->Get_Transform()->m_vInfo[INFO_POS].y += (m_fSpeed *fTimeDelta);
}

void CWakeUp_BossSkeleton::LateUpdate_State()
{
}

void CWakeUp_BossSkeleton::Render_State()
{
}

CWakeUp_BossSkeleton* CWakeUp_BossSkeleton::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
    CWakeUp_BossSkeleton* pState = new CWakeUp_BossSkeleton(pGraphicDev);
    if (FAILED(pState->Ready_State(pOwner)))
    {
        Safe_Release<CWakeUp_BossSkeleton*>(pState);

        MSG_BOX("Fire State Failed");
    }
    return pState;
}

void CWakeUp_BossSkeleton::Free()
{
    __super::Free();
}
