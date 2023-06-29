#include "..\..\Header\RcCol.h"

CRcCol::CRcCol()
{
}

CRcCol::CRcCol(LPDIRECT3DDEVICE9 pGraphicDev)
	:  CVIBuffer(pGraphicDev)
{
}

CRcCol::CRcCol(const CRcCol & rhs)
	: CVIBuffer(rhs)
{
}


CRcCol::~CRcCol()
{
}

HRESULT CRcCol::Ready_Buffer(void)
{
	m_dwFVF = FVF_COL;
	m_dwTriCnt = 2;
	m_dwVtxCnt = 6;
	m_dwVtxSize = sizeof(VTXCOL);

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	VTXCOL*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);
	
	// 오른쪽 위
	pVertex[0].vPosition = { -1.f, 1.f, 0.f };
	pVertex[0].dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

	pVertex[1].vPosition = { 1.f, 1.f, 0.f };
	pVertex[1].dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

	pVertex[2].vPosition = { 1.f, -1.f, 0.f };
	pVertex[2].dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

	// 왼쪽 아래
	pVertex[3].vPosition = { -1.f, 1.f, 0.f };
	pVertex[3].dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

	pVertex[4].vPosition = { 1.f, -1.f, 0.f };
	pVertex[4].dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

	pVertex[5].vPosition = { -1.f, -1.f, 0.f };
	pVertex[5].dwColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);
	
	m_pVB->Unlock();

	return S_OK;
}

void CRcCol::Render_Buffer(void)
{
	CVIBuffer::Render_Buffer();
}

CRcCol * CRcCol::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRcCol *	pInstance = new CRcCol(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))
	{
		Safe_Release(pInstance);
		MSG_BOX("RcCol Create Failed");
		return nullptr;
	}

	return pInstance;
}

CComponent * CRcCol::Clone(void)
{
	return new CRcCol(*this);
}

void CRcCol::Free(void)
{
	CVIBuffer::Free();
}
