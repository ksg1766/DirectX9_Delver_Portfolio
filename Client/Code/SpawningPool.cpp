#include "..\Header\SpawningPool.h"
#include "Export_Function.h"
#include "PoolManager.h"
#include "Player.h"
#include "Monstergroup.h"

CSpawningPool::CSpawningPool(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CSpawningPool::CSpawningPool(const CSpawningPool& rhs)
	: CGameObject(rhs)
{
}

CSpawningPool::~CSpawningPool()
{
}

HRESULT CSpawningPool::Ready_Object()
{
    m_eObjectTag = OBJECTTAG::SPAWNINGPOOL;
    m_eMonsterTag = MONSTERTAG::MONSTER_END;
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    void* pVertices = nullptr;
    m_pBuffer->m_pVB->Lock(0, m_pBuffer->m_dwVtxCnt * m_pBuffer->m_dwVtxSize, &pVertices, 0);
    for (UINT i = 0; i < m_pBuffer->m_dwVtxCnt; ++i)
    {
        m_vecSpawnerVertex.push_back((((VTXCOL*)pVertices) + i)->vPosition);
    }
    m_pBuffer->m_pVB->Unlock();

    void* pIndices = nullptr;
    m_pBuffer->m_pIB->Lock(0, m_pBuffer->m_dwTriCnt * m_pBuffer->m_dwIdxSize, &pIndices, 0);
    for (UINT i = 0; i < m_pBuffer->m_dwTriCnt; ++i)
    {
        m_vecSpawnerIndex.push_back(*(((INDEX32*)pIndices) + i));
    }
    m_pBuffer->m_pIB->Unlock();

    m_pTransform->Scale(_vec3(m_fSpawnRadius, 1.f, m_fSpawnRadius));
   
    return S_OK;
}

_int CSpawningPool::Update_Object(const _float& fTimeDelta)
{
    if (SCENETAG::EDITOR == SceneManager()->Get_Scene()->Get_SceneTag())
    {
        Engine::Renderer()->Add_RenderGroup(RENDER_NONALPHA, this);
        return 0;
    }

    if (SceneManager()->Get_GameStop()) { return 0; } // ! Esc 및 M키 누를 시 업데이트 멈추게 하는 용도 입니다.

    _vec3 vPlayerPos = SceneManager()->Get_Scene()->Get_MainPlayer()->m_pTransform->m_vInfo[INFO_POS];

    if (D3DXVec3Length(&(m_pTransform->m_vInfo[INFO_POS] - vPlayerPos)) > 80.f)
        return 0;

    _int iExit = __super::Update_Object(fTimeDelta);

    if (m_iLifeCount <= 0)
    {
        EventManager()->DeleteObject(this);
    }

    if (!m_MonsterList.empty())
    {
        for (auto& iter = m_MonsterList.begin(); iter != m_MonsterList.end();)
        {
            if ((*iter)->IsDead())
                iter = m_MonsterList.erase(iter);
            else
                ++iter;
        }
    }

    if (m_dqReserved.size() + m_MonsterList.size() < m_iPoolCapacity)
    {
        m_dqReserved.push_back(fTimeDelta);
    }

    for(int i = 0; i < m_dqReserved.size(); ++i)
    {
        m_dqReserved[i] += fTimeDelta;
        if (m_dqReserved[i] > m_fSpawnTime)
        {
            m_dqReserved.pop_front();
            ReserveSpawn();
        }
    }

    return iExit;
}

HRESULT CSpawningPool::InitSpawnerBox()
{   

    return S_OK;
}

void CSpawningPool::ReserveSpawn()
{
    mt19937 engine((_uint)time(NULL));           // MT19937 난수 엔진
    uniform_int_distribution<__int64> distribution(-m_fSpawnRadius, m_fSpawnRadius); // 생성 범위
    auto generator = bind(distribution, engine);

    CMonster* pMonster = nullptr;

    _float fX = generator();
    _float fZ = generator();
    /*pMonster->m_pTransform->Translate(m_pTransform->m_vInfo[INFO_POS] + _vec3(fX, -2.f, fZ));
    EventManager()->CreateObject(pMonster, LAYERTAG::GAMELOGIC);*/
    //pMonster = CPoolManager::GetInstance()->Create_Monster(m_eMonsterTag, m_pTransform->m_vInfo[INFO_POS] + _vec3(fX, 10.f, fZ));
    pMonster = CPoolManager::GetInstance()->Create_Monster(m_eMonsterTag, m_pTransform->m_vInfo[INFO_POS] + _vec3(fX, 0.f, fZ));
    pMonster->Set_CenterPos(m_pTransform->m_vInfo[INFO_POS]);
    pMonster->Set_MoveRange(abs(m_fSpawnRadius));
    pMonster->Set_RandomMoveRange(abs(m_fSpawnRadius + 0.2f));

    m_MonsterList.push_back(pMonster);
    --m_iLifeCount;
}

void CSpawningPool::Set_SpawnRadius(_float _fRadius)
{
    m_fSpawnRadius = _fRadius; m_pTransform->Scale(_vec3(m_fSpawnRadius, 1.f, m_fSpawnRadius));
}

HRESULT CSpawningPool::Add_Component()
{
    CComponent* pComponent = nullptr;

    //if(SCENETAG::EDITOR == SceneManager()->Get_Scene()->Get_SceneTag())
    pComponent = m_pBuffer = static_cast<CCubeColBf*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_CubeColBf"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_STATIC].emplace(COMPONENTTAG::BUFFER, pComponent);

    pComponent = m_pTransform = static_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

    return S_OK;
}

CSpawningPool* CSpawningPool::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CSpawningPool* pInstance = new CSpawningPool(pGraphicDev);

    if (FAILED(pInstance->Ready_Object()))
    {
        Safe_Release<CSpawningPool*>(pInstance);

        MSG_BOX("SpawningPool Create Failed");
        return nullptr;
    }

    return pInstance;
}

void CSpawningPool::Free(void)
{
    //for_each(m_MonsterList.begin(), m_MonsterList.end(), CDeleteObj());
    m_MonsterList.clear();

    __super::Free();
}

void CSpawningPool::Render_Object(void)
{
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    //m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
    //m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

    m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransform->WorldMatrix());
    m_pBuffer->Render_Buffer();

    m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    //m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    //m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}