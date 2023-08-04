#include "Boss_Skeleton.h"
#include "Export_Function.h"
#include "Move_BossSkeleton.h"
#include "Jump_BossSkeleton.h"
#include "Explosion_BossSkeleton.h"
#include "Dead_BossSkeleton.h"
#include "WakeUp_BossSkeleton.h"

CBoss_Skeleton::CBoss_Skeleton(LPDIRECT3DDEVICE9 pGrapicDev)
    : Engine::CMonster(pGrapicDev)
{
}

CBoss_Skeleton::CBoss_Skeleton(const CBoss_Skeleton& rhs)
    : Engine::CMonster(rhs)
{
}

CBoss_Skeleton::~CBoss_Skeleton()
{
    Free();
}

HRESULT CBoss_Skeleton::Ready_Object()
{
    Set_ObjectTag(OBJECTTAG::MONSTER);
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
    m_pBasicStat->Get_Stat()->fHealth = 3.f;

    CState*  pState = CDead_BossSkeleton::Create(m_pGraphicDev, m_pStateMachine);
    m_pStateMachine->Add_State(STATE::DEAD, pState);

    pState = CMove_BossSkeleton::Create(m_pGraphicDev, m_pStateMachine);
    m_pStateMachine->Add_State(STATE::ROMIMG, pState);

    pState = CJump_BossSkeleton::Create(m_pGraphicDev, m_pStateMachine);
    m_pStateMachine->Add_State(STATE::ATTACK, pState);

    pState = CExplosion_BossSkeleton::Create(m_pGraphicDev, m_pStateMachine);
    m_pStateMachine->Add_State(STATE::HIT, pState);

#pragma region 애니메이션
    CAnimation* pAnimation = CAnimation::Create(m_pGraphicDev,
        m_pTexture[(_uint)STATE::DEAD], STATE::DEAD, 8.f, FALSE);
    m_pAnimator->Add_Animation(STATE::DEAD, pAnimation);

    pAnimation = CAnimation::Create(m_pGraphicDev,
        m_pTexture[(_uint)STATE::ROMIMG], STATE::ROMIMG, 8.f, TRUE);
    m_pAnimator->Add_Animation(STATE::ROMIMG, pAnimation);

    pAnimation = CAnimation::Create(m_pGraphicDev,
        m_pTexture[(_uint)STATE::ATTACK], STATE::ATTACK, 8.f, FALSE);
    m_pAnimator->Add_Animation(STATE::ATTACK, pAnimation);

    pAnimation = CAnimation::Create(m_pGraphicDev,
        m_pTexture[(_uint)STATE::HIT], STATE::HIT, 8.f, FALSE);
    m_pAnimator->Add_Animation(STATE::HIT, pAnimation);
#pragma endregion 애니메이션

    m_pStateMachine->Set_Animator(m_pAnimator);
    m_pStateMachine->Set_State(STATE::ROMIMG);

    m_pTransform->Translate(_vec3(0.f, -3.f, 0.2f));
    return S_OK;
}

_int CBoss_Skeleton::Update_Object(const _float& fTimeDelta)
{
    Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this); 

    if (SceneManager()->Get_GameStop()) { return 0; } 

    _int iExit = __super::Update_Object(fTimeDelta);

    m_pStateMachine->Update_StateMachine(fTimeDelta);
    return iExit;
}

void CBoss_Skeleton::LateUpdate_Object()
{
    if (SceneManager()->Get_GameStop()) { return; }
    m_pBillBoard->LateUpdate_Component();
    __super::LateUpdate_Object();
    __super::Compute_ViewZ(&m_pTransform->m_vInfo[INFO_POS]);
}

void CBoss_Skeleton::Render_Object()
{
    m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());

    m_pStateMachine->Render_StateMachine();
    m_pBuffer->Render_Buffer();
}

void CBoss_Skeleton::Init_Stat()
{
}

void CBoss_Skeleton::OnCollisionEnter(CCollider* _pOther)
{
    if (SceneManager()->Get_GameStop()) { return; }

}

void CBoss_Skeleton::OnCollisionStay(CCollider* _pOther)
{
    if (SceneManager()->Get_GameStop()) { return; }
}

void CBoss_Skeleton::OnCollisionExit(CCollider* _pOther)
{
}

HRESULT CBoss_Skeleton::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pBuffer = static_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

    pComponent = m_pTransform = static_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

    pComponent = m_pCollider = static_cast<CCollider*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Collider"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);

    pComponent = m_pBillBoard = static_cast<CBillBoard*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BillBoard"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BILLBOARD, pComponent);

    pComponent = m_pStateMachine = static_cast<CStateMachine*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_State"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::STATEMACHINE, pComponent);

    pComponent = m_pAnimator = static_cast<CAnimator*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Animator"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::ANIMATOR, pComponent);

    pComponent = m_pBasicStat = static_cast<CBasicStat*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BasicStat"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BASICSTAT, pComponent);

#pragma region 텍스쳐 컴포넌트
    pComponent = m_pTexture[(_uint)STATE::ROMIMG] = static_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_Skeleton"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

    pComponent = m_pTexture[(_uint)STATE::ATTACK] = static_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_SkeletonAttack"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

    pComponent = m_pTexture[(_uint)STATE::HIT] = static_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_SkeletonHit"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

    pComponent = m_pTexture[(_uint)STATE::DEAD] = static_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_SkeletonDead"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

    /*pComponent = m_pTexture[(_uint)STATE::IDLE] = static_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_SkeletonReverse"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);*/
#pragma endregion 텍스쳐 컴포넌트

    for (_uint i = 0; i < ID_END; ++i)
        for (auto& iter : m_mapComponent[i])
            iter.second->Init_Property(this);

    return S_OK;
}

CBoss_Skeleton* CBoss_Skeleton::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{

    CBoss_Skeleton* pInstance = new CBoss_Skeleton(pGraphicDev);

    if (FAILED(pInstance->Ready_Object()))
    {
        Safe_Release(pInstance);

        MSG_BOX("Boss_Skeleton Create Failed");
        return nullptr;
    }

    return pInstance;
}

void CBoss_Skeleton::Free()
{
    __super::Free();
}
