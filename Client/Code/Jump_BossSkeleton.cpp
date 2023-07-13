#include "Jump_BossSkeleton.h"
#include "Export_Function.h"
#include "Boss_Skeleton.h"

CJump_BossSkeleton::CJump_BossSkeleton()
{
}

CJump_BossSkeleton::CJump_BossSkeleton(LPDIRECT3DDEVICE9 pGraphicDev)
    :CState(pGraphicDev)
{
}

CJump_BossSkeleton::~CJump_BossSkeleton()
{
}

HRESULT CJump_BossSkeleton::Ready_State(CStateMachine* pOwner)
{
    m_pOwner = pOwner;
    m_fCool = 0.f;
    m_fJumpVelocity = 13.f;
    m_fSpeed = 2.f;
    m_bJump = false;
    return S_OK;
}

STATE CJump_BossSkeleton::Update_State(const _float& fTimeDelta)
{
    m_fCool += fTimeDelta;
    if (!m_bJump)
    {
        m_vTargetPos = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform->m_vInfo[INFO_POS];
        m_vDir = m_vTargetPos - m_pOwner->Get_Transform()->m_vInfo[INFO_POS];

        dynamic_cast<CBoss_Skeleton*>(SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::BOSS).front())->m_pTransform->Translate((m_vDir * m_fSpeed) * fTimeDelta);
    }
    if (5.f < m_fCool)
    {
        m_fCool = 0.f;
        return STATE::DEAD;
    }
}

void CJump_BossSkeleton::LateUpdate_State()
{
}

void CJump_BossSkeleton::Render_State()
{
}

void CJump_BossSkeleton::Jump(const _float& fTimeDelta)
{
    if (!m_bJump)
        return;
    m_pOwner->Get_Transform()->m_vInfo[INFO_POS].y += m_fJumpVelocity * fTimeDelta;
    m_pOwner->Get_Transform()->m_vInfo[INFO_POS] += m_vDir * 10.f * fTimeDelta;
    m_fJumpVelocity -= 0.5f * fTimeDelta * fTimeDelta * 3000.f;
    if (0.f > m_pOwner->Get_Transform()->m_vInfo[INFO_POS].y)
    {
        m_pOwner->Get_Transform()->m_vInfo[INFO_POS].y = 0.f;
    }

}

CJump_BossSkeleton* CJump_BossSkeleton::Create(LPDIRECT3DDEVICE9 pGraphicDev, CStateMachine* pOwner)
{
    CJump_BossSkeleton* pState = new CJump_BossSkeleton(pGraphicDev);
    if (FAILED(pState->Ready_State(pOwner)))
    {
        Safe_Release<CJump_BossSkeleton*>(pState);

        MSG_BOX("Fire State Failed");
    }
    return pState;
}

void CJump_BossSkeleton::Free()
{
    __super::Free();
}
