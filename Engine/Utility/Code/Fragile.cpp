#include "..\..\Header\Fragile.h"
#include "CubeBf.h"

CFragile::CFragile(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev), m_eFragileTag(FRAGILETAG::FRAGILE_END)
{
}

CFragile::CFragile(const CFragile& rhs)
	: CGameObject(rhs), m_eFragileTag(rhs.m_eFragileTag)
{
    
}

CFragile::~CFragile()
{
}

HRESULT CFragile::Ready_Object()
{
    void* pVertices = nullptr;
    m_pCubeBf->m_pVB->Lock(0, m_pCubeBf->m_dwVtxCnt * m_pCubeBf->m_dwVtxSize, &pVertices, 0);
    for (UINT i = 0; i < m_pCubeBf->m_dwVtxCnt; ++i)
    {
        m_vecFragileVertex.push_back((((VTXCOL*)pVertices) + i)->vPosition);
    }
    m_pCubeBf->m_pVB->Unlock();

    void* pIndices = nullptr;
    m_pCubeBf->m_pIB->Lock(0, m_pCubeBf->m_dwTriCnt * m_pCubeBf->m_dwIdxSize, &pIndices, 0);
    for (UINT i = 0; i < m_pCubeBf->m_dwTriCnt; ++i)
    {
        m_vecFragileIndex.push_back(*(((INDEX32*)pIndices) + i));
    }
    m_pCubeBf->m_pIB->Unlock();

	return S_OK;
}

_int CFragile::Update_Object(const _float& fTimeDelta)
{
    _int iExit = __super::Update_Object(fTimeDelta);

    return iExit;
}

void CFragile::LateUpdate_Object()
{
    __super::LateUpdate_Object();
}

void CFragile::Free(void)
{
    __super::Free();
}
