#include "..\..\Header\PlayerBf.h"

CPlayerBf::CPlayerBf()
{
}

CPlayerBf::CPlayerBf(LPDIRECT3DDEVICE9 pGraphicDev)
	:  CVIBuffer(pGraphicDev)
{
}

CPlayerBf::CPlayerBf(const CPlayerBf & rhs)
	: CVIBuffer(rhs)
{
}


CPlayerBf::~CPlayerBf()
{
}

HRESULT CPlayerBf::Ready_Buffer(void)
{
	m_dwFVF = VTXCOL::format;
	m_dwTriCnt = 2;
	m_dwVtxCnt = 4;
	m_dwVtxSize = sizeof(VTXCOL);
	m_IdxFmt = D3DFMT_INDEX32;
	m_dwIdxSize = sizeof(INDEX32);
	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	VTXCOL*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);
	
	pVertex[0].vPosition = { -1.f, 1.f, 0.f };
	pVertex[0].dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

	pVertex[1].vPosition = { 1.f, 1.f, 0.f };
	pVertex[1].dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

	pVertex[2].vPosition = { 1.f, -1.f, 0.f };
	pVertex[2].dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

	pVertex[3].vPosition = { -1.f, -1.f, 0.f };
	pVertex[3].dwColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);
	
	m_pVB->Unlock();

	INDEX32* pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;

	pIndex[1]._0 = 0;
	pIndex[1]._1 = 2;
	pIndex[1]._2 = 3;

	m_pIB->Unlock();


	return S_OK;
}

void CPlayerBf::Render_Buffer(void)
{
	CVIBuffer::Render_Buffer();
}

CPlayerBf * CPlayerBf::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayerBf *	pInstance = new CPlayerBf(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))
	{
		Safe_Release(pInstance);
		MSG_BOX("PlayerBf Create Failed");
		return nullptr;
	}

	return pInstance;
}

CComponent * CPlayerBf::Clone(void)
{
	return new CPlayerBf(*this);
}

void CPlayerBf::Free(void)
{
	CVIBuffer::Free();
}
