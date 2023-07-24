#include "..\..\Header\Trap.h"
#include "CubeBf.h"

CTrap::CTrap(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CTrap::CTrap(const CTrap& rhs)
	: CGameObject(rhs), m_eTrapTag(rhs.m_eTrapTag)
{
}

CTrap::~CTrap()
{
}

HRESULT CTrap::Ready_Object()
{
    void* pVertices = nullptr;
    m_pCubeBf->m_pVB->Lock(0, m_pCubeBf->m_dwVtxCnt * m_pCubeBf->m_dwVtxSize, &pVertices, 0);
    for (UINT i = 0; i < m_pCubeBf->m_dwVtxCnt; ++i)
    {
        m_vecTrapVertex.push_back((((VTXCOL*)pVertices) + i)->vPosition);
    }
    m_pCubeBf->m_pVB->Unlock();

    void* pIndices = nullptr;
    m_pCubeBf->m_pIB->Lock(0, m_pCubeBf->m_dwTriCnt * m_pCubeBf->m_dwIdxSize, &pIndices, 0);
    for (UINT i = 0; i < m_pCubeBf->m_dwTriCnt; ++i)
    {
        m_vecTrapIndex.push_back(*(((INDEX32*)pIndices) + i));
    }
    m_pCubeBf->m_pIB->Unlock();

	return S_OK;
}

_int CTrap::Update_Object(const _float& fTimeDelta)
{
	_int iExit = __super::Update_Object(fTimeDelta);

	return iExit;
}

void CTrap::LateUpdate_Object()
{
    __super::LateUpdate_Object();
}

void CTrap::Free(void)
{
	__super::Free();
}