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
    m_fSpeed = 2.f;
    m_bJump = false;
  
    m_fJumpVelocity = 50.f;
    m_bJumCoolDown = false;
    m_bIsJumping = false;
    m_fJumpCoolDuration = 2.f;
    m_fJumpCoolTimer = 0.f;
    m_vLastPos = _vec3(0.f, 0.f, 0.f);
    m_vSavePos = _vec3(0.f, 0.f, 0.f);
    return S_OK;
}

STATE CJump_BossSkeleton::Update_State(const _float& fTimeDelta)
{
    m_fCool += fTimeDelta;
    if (4.f < m_fCool)
    {
        m_bJump = false;
        m_fCool = 0.f;
        return STATE::HIT;
    }
    else if ((0.5f <= m_fCool)&&(!m_bJump))
    {//Jump(fTimeDelta);
        CTransform* pPlayerTransform = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform;


        _vec3 vPlayerPos = pPlayerTransform->m_vInfo[INFO_POS];
        _vec3& vMonsterPos = m_pOwner->Get_Transform()->m_vInfo[INFO_POS];

        if (!m_bIsJumping)
        {
            m_vLastPos = vPlayerPos;
            m_bIsJumping = true;
        }

        _vec3 vDir = m_vLastPos - vMonsterPos;
        D3DXVec3Normalize(&vDir, &vDir);

        vMonsterPos.y += m_fJumpVelocity * fTimeDelta;
        vMonsterPos += vDir * 10.f * fTimeDelta;

        m_fJumpVelocity -= 0.5f * fTimeDelta * fTimeDelta * 3000.f;

        _vec3 vDistance = vPlayerPos - m_pOwner->Get_Transform()->m_vInfo[INFO_POS];

        if (vMonsterPos.y < 1.f)
        {
            vMonsterPos.y = 1.f;
            m_fJumpVelocity = 13.f;
            m_bJumCoolDown = true;
            m_bIsJumping = false;
            m_bJump = true;
        }
        return STATE::ATTACK;
    }
    else if (0.2f > m_fCool)
    {
        m_vTargetPos = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform->m_vInfo[INFO_POS];
        m_vDir = m_vTargetPos - m_pOwner->Get_Transform()->m_vInfo[INFO_POS];

        m_pOwner->Get_Transform()->Translate((m_vDir * m_fSpeed) * fTimeDelta);
    }
    else if (1.5f <= m_fCool)
    {
        m_fJumpVelocity = 13.f;
        m_bJumCoolDown = true;
        m_bIsJumping = false;
        m_bJump = true;
        return STATE::ATTACK;
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
    CTransform* pPlayerTransform = SceneManager()->Get_ObjectList(LAYERTAG::GAMELOGIC, OBJECTTAG::PLAYER).front()->m_pTransform;


    _vec3 vPlayerPos = pPlayerTransform->m_vInfo[INFO_POS];
    _vec3& vMonsterPos = m_pOwner->Get_Transform()->m_vInfo[INFO_POS];

    if (!m_bIsJumping)
    {
        m_vLastPos = vPlayerPos;
        m_bIsJumping = true;
    }

    _vec3 vDir = m_vLastPos - vMonsterPos;
    D3DXVec3Normalize(&vDir, &vDir);

    vMonsterPos.y += m_fJumpVelocity * fTimeDelta;
    vMonsterPos += vDir * 10.f * fTimeDelta;

    m_fJumpVelocity -= 0.5f * fTimeDelta * fTimeDelta * 300.f;

    _vec3 vDistance = vPlayerPos - m_pOwner->Get_Transform()->m_vInfo[INFO_POS];

    if (vMonsterPos.y < 1.f)
    {
        vMonsterPos.y = 1.f;
        m_fJumpVelocity = 13.f;
        m_bJumCoolDown = true;
        m_bIsJumping = false;
        m_bJump = true;
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
