#include "..\..\Header\Environment.h"
#include "RcTex.h"

CEnvironment::CEnvironment(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CEnvironment::CEnvironment(const CEnvironment& rhs)
	: CGameObject(rhs), m_eEnvTag(rhs.m_eEnvTag)
{
}

CEnvironment::~CEnvironment()
{
}

HRESULT CEnvironment::Ready_Object()
{
    void* pVertices = nullptr;
    m_pRcBf->m_pVB->Lock(0, m_pRcBf->m_dwVtxCnt * m_pRcBf->m_dwVtxSize, &pVertices, 0);
    for (UINT i = 0; i < m_pRcBf->m_dwVtxCnt; ++i)
    {
        m_vecEnvVertex.push_back((((VTXCOL*)pVertices) + i)->vPosition);
    }
    m_pRcBf->m_pVB->Unlock();

    void* pIndices = nullptr;
    m_pRcBf->m_pIB->Lock(0, m_pRcBf->m_dwTriCnt * m_pRcBf->m_dwIdxSize, &pIndices, 0);
    for (UINT i = 0; i < m_pRcBf->m_dwTriCnt; ++i)
    {
        m_vecEnvIndex.push_back(*(((INDEX32*)pIndices) + i));
    }
    m_pRcBf->m_pIB->Unlock();

    return S_OK;
}

_int CEnvironment::Update_Object(const _float& fTimeDelta)
{
    _int iExit = __super::Update_Object(fTimeDelta);
	
    return iExit;
}

void CEnvironment::LateUpdate_Object()
{
    __super::LateUpdate_Object();
}

void CEnvironment::Free(void)
{
	__super::Free();
}
