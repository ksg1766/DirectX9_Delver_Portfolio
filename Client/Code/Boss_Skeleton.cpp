#include "Boss_Skeleton.h"
#include "Export_Function.h"
#include "Terrain.h"
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
    m_pCollider->InitOBB(m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT], m_pTransform->LocalScale());
  /*  CState* pState = CWakeUp_BossSkeleton::Create(m_pGraphicDev, m_pStateMachine);
    m_pStateMachine->Add_State(STATE::IDLE, pState);*/

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

  /*  pAnimation = CAnimation::Create(m_pGraphicDev,
        m_pTexture[(_uint)STATE::IDLE], STATE::IDLE, 8.f, FALSE);
    m_pAnimator->Add_Animation(STATE::IDLE, pAnimation);*/

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

    ForceHeight(m_pTransform->m_vInfo[INFO_POS]);
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
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);


    m_pStateMachine->Render_StateMachine();
    m_pBuffer->Render_Buffer();

#if _DEBUG
    m_pCollider->Render_Collider();
#endif

    m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CBoss_Skeleton::OnCollisionEnter(CCollider* _pOther)
{
    if (SceneManager()->Get_GameStop()) { return; }

    _vec3	vOtherPos = _pOther->GetCenterPos();
    _float* fOtherAxis = _pOther->GetAxisLen();

    _vec3	vThisPos = m_pCollider->GetCenterPos();
    _float* fThisAxis = m_pCollider->GetAxisLen();

    // OBJECTTAG에 따른 예외 처리 가능성
    _float fWidth = fabs(vOtherPos.x - vThisPos.x);
    _float fHeight = fabs(vOtherPos.y - vThisPos.y);
    _float fDepth = fabs(vOtherPos.z - vThisPos.z);

    _float fRadiusX = (fOtherAxis[0] + fThisAxis[0]) - fWidth;
    _float fRadiusY = (fOtherAxis[1] + fThisAxis[1]) - fHeight;
    _float fRadiusZ = (fOtherAxis[2] + fThisAxis[2]) - fDepth;

    _float fMinAxis = min(min(fRadiusX, fRadiusY), fRadiusZ);	// 가장 작은 값이 가장 얕게 충돌한 축. 이 축을 밀어내야 함.

    if (fRadiusX == fMinAxis)
    {
        if (vOtherPos.x < vThisPos.x)
            m_pTransform->Translate(_vec3(fRadiusX, 0.f, 0.f));
        else
            m_pTransform->Translate(_vec3(-fRadiusX, 0.f, 0.f));
    }
    else if (fRadiusZ == fMinAxis)
    {
        if (vOtherPos.z < vThisPos.z)
            m_pTransform->Translate(_vec3(0.f, 0.f, fRadiusZ));
        else
            m_pTransform->Translate(_vec3(0.f, 0.f, -fRadiusZ));
    }
    else //(fRadiusY == fMinAxis)
    {
        if (vOtherPos.y < vThisPos.y)
            m_pTransform->Translate(_vec3(0.f, fRadiusY, 0.f));
        else
            m_pTransform->Translate(_vec3(0.f, -fRadiusY, 0.f));
    }
}

void CBoss_Skeleton::OnCollisionStay(CCollider* _pOther)
{
    if (SceneManager()->Get_GameStop()) { return; }

#pragma region 밀어내기
    _vec3	vOtherPos = _pOther->GetCenterPos();
    _float* fOtherAxis = _pOther->GetAxisLen();

    _vec3	vThisPos = m_pCollider->GetCenterPos();
    _float* fThisAxis = m_pCollider->GetAxisLen();

    // OBJECTTAG에 따른 예외 처리 가능성
    _float fWidth = fabs(vOtherPos.x - vThisPos.x);
    _float fHeight = fabs(vOtherPos.y - vThisPos.y);
    _float fDepth = fabs(vOtherPos.z - vThisPos.z);

    _float fRadiusX = (fOtherAxis[0] + fThisAxis[0]) - fWidth;
    _float fRadiusY = (fOtherAxis[1] + fThisAxis[1]) - fHeight;
    _float fRadiusZ = (fOtherAxis[2] + fThisAxis[2]) - fDepth;

    _float fMinAxis = min(min(fRadiusX, fRadiusY), fRadiusZ);	// 가장 작은 값이 가장 얕게 충돌한 축. 이 축을 밀어내야 함.

    if (fRadiusX == fMinAxis)
    {
        if (vOtherPos.x < vThisPos.x)
            m_pTransform->Translate(_vec3(fRadiusX, 0.f, 0.f));
        else
            m_pTransform->Translate(_vec3(-fRadiusX, 0.f, 0.f));
    }
    else if (fRadiusZ == fMinAxis)
    {
        if (vOtherPos.z < vThisPos.z)
            m_pTransform->Translate(_vec3(0.f, 0.f, fRadiusZ));
        else
            m_pTransform->Translate(_vec3(0.f, 0.f, -fRadiusZ));
    }
    else //(fRadiusY == fMinAxis)
    {
        if (vOtherPos.y < vThisPos.y)
            m_pTransform->Translate(_vec3(0.f, fRadiusY, 0.f));
        else
            m_pTransform->Translate(_vec3(0.f, -fRadiusY, 0.f));
    }
#pragma endregion 밀어내기
}

void CBoss_Skeleton::OnCollisionExit(CCollider* _pOther)
{
}

HRESULT CBoss_Skeleton::Add_Component()
{
    CComponent* pComponent = nullptr;

    pComponent = m_pBuffer = dynamic_cast<CRcTex*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_RcTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

    pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

    pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Collider"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::COLLIDER, pComponent);

    pComponent = m_pBillBoard = dynamic_cast<CBillBoard*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BillBoard"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BILLBOARD, pComponent);

    pComponent = m_pStateMachine = dynamic_cast<CStateMachine*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_State"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::STATEMACHINE, pComponent);

    pComponent = m_pAnimator = dynamic_cast<CAnimator*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Animator"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::ANIMATOR, pComponent);

    pComponent = m_pBasicStat = dynamic_cast<CBasicStat*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BasicStat"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BASICSTAT, pComponent);

#pragma region 텍스쳐 컴포넌트
    pComponent = m_pTexture[(_uint)STATE::ROMIMG] = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_Skeleton"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

    pComponent = m_pTexture[(_uint)STATE::ATTACK] = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_SkeletonAttack"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

    pComponent = m_pTexture[(_uint)STATE::HIT] = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_SkeletonHit"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

    pComponent = m_pTexture[(_uint)STATE::DEAD] = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_SkeletonDead"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);

    /*pComponent = m_pTexture[(_uint)STATE::IDLE] = dynamic_cast<CTexture*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Texture_SkeletonReverse"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::TEXTURE0, pComponent);*/
#pragma endregion 텍스쳐 컴포넌트

    for (_uint i = 0; i < ID_END; ++i)
        for (auto& iter : m_mapComponent[i])
            iter.second->Init_Property(this);

    return S_OK;
}

void CBoss_Skeleton::ForceHeight(_vec3 _vPos)
{
    if (STATE::BOSS_METEORREADY == m_pStateMachine->Get_State())
        return;
    _float x = (VTXCNTX * VTXITV / 2.f) + _vPos.x;
    _float z = (VTXCNTZ * VTXITV / 2.f) + _vPos.z;

    x /= (_float)VTXITV;
    z /= (_float)VTXITV;

    _int col = ::floorf(x);
    _int row = ::floorf(z);

    _vec3 A = m_pTerrain->LoadTerrainVertex()[row * VTXCNTX + col];
    _vec3 B = m_pTerrain->LoadTerrainVertex()[row * VTXCNTX + col + 1];
    _vec3 C = m_pTerrain->LoadTerrainVertex()[(row + 1) * VTXCNTX + col];
    _vec3 D = m_pTerrain->LoadTerrainVertex()[(row + 1) * VTXCNTX + col + 1];

    _float dx = x - col;
    _float dz = z - row;

    _float height;

    if (dz < 1.0f - dx)
    {

        _vec3 uy = B - A;
        _vec3 vy = C - A;

        height = A.y + (uy.y * dx) + (vy.y * dz) + 1.f;
        m_pTransform->m_vInfo[INFO_POS].y = height;
    }
    else
    {
        _vec3 uy = C - D;
        _vec3 vy = B - D;

        height = D.y + (uy.y * (1.f - dx)) + (vy.y * (1.f - dz)) + 1.f;
        m_pTransform->m_vInfo[INFO_POS].y = height;
    }
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
