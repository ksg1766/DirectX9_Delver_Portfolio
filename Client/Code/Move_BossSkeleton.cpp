#include "Move_BossSkeleton.h"
#include "Export_Function.h"
#include "Boss_Skeleton.h"

CMove_BossSkeleton::CMove_BossSkeleton()
{
}

CMove_BossSkeleton::CMove_BossSkeleton(LPDIRECT3DDEVICE9 pGraphicDev)
	:CState(pGraphicDev)
{
}

CMove_BossSkeleton::~CMove_BossSkeleton()
{
}

HRESULT CMove_BossSkeleton::Ready_State(CStateMachine* pOwner)
{
	m_pOwner = pOwner;
	m_fCool = 0.f;
	m_fSpeed = 2.f;
	return S_OK;
}

STATE CMove_BossSkeleton::Update_State(const _float& fTimeDelta)
{
    m_fCool += fTimeDelta;
    m_vTargetPos = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform->m_vInfo[INFO_POS];
    m_vDir = m_vTargetPos - m_pOwner->Get_Transform()->m_vInfo[INFO_POS];

    dynamic_cast<CBoss_Skeleton*>(SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front())->m_pTransform->Translate((m_vDir * m_fSpeed) * fTimeDelta);

    if (5.f < m_fCool)
    {
        m_fCool = 0.f;
        return STATE::ATTACK;
    }
}

void CMove_BossSkeleton::LateUpdate_State()
{
}

void CMove_BossSkeleton::Render_State()
{
}

CMove_BossSkeleton* CMove_BossSkeleton::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
    CMove_BossSkeleton* pState = new CMove_BossSkeleton(pGraphicDev);
    if (FAILED(pState->Ready_State(pOwner)))
    {
        Safe_Release<CMove_BossSkeleton*>(pState);

        MSG_BOX("Fire State Failed");
    }
    return pState;
}

void CMove_BossSkeleton::Free()
{
    __super::Free();
}
