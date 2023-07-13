#include "Boss_Skeleton.h"
#include "Export_Function.h"
#include "Terrain.h"

#include "Player.h"

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
    Set_ObjectTag(OBJECTTAG::MONSTERBULLET);
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
    m_pBasicStat->Get_Stat()->fHealth = 3.f;

    m_pCollider->InitOBB(m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_RIGHT], m_pTransform->LocalScale());

    //CState* pState = CBoss_Idle::Create(m_pGraphicDev, m_pStateMachine);
    //m_pStateMachine->Add_State(STATE::BOSS_IDLE, pState);



    return S_OK;
}

_int CBoss_Skeleton::Update_Object(const _float& fTimeDelta)
{
    Engine::Renderer()->Add_RenderGroup(RENDER_ALPHA, this); 

    if (SceneManager()->Get_GameStop()) { return 0; } 

    _int iExit = __super::Update_Object(fTimeDelta);


    return iExit;
}

void CBoss_Skeleton::LateUpdate_Object()
{
    if (SceneManager()->Get_GameStop()) { return; }
    __super::LateUpdate_Object();

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
}

void CBoss_Skeleton::OnCollisionStay(CCollider* _pOther)
{
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

    pComponent = dynamic_cast<CBillBoard*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_BillBoard"));
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
