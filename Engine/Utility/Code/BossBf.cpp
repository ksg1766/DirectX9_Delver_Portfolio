#include "..\..\Header\BossBf.h"

CBossBf::CBossBf()
{
}

CBossBf::CBossBf(LPDIRECT3DDEVICE9 pGraphicDev)
	:  CVIBuffer(pGraphicDev)
{
}

CBossBf::CBossBf(const CBossBf & rhs)
	: CVIBuffer(rhs)
{
}


CBossBf::~CBossBf()
{
}

HRESULT CBossBf::Ready_Buffer(void)
{
	m_dwFVF = VTXTEX::format;
	m_dwTriCnt = 2;
	m_dwVtxCnt = 4;
	m_dwVtxSize = sizeof(VTXTEX);

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	VTXTEX*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);
	
	// 오른쪽 위
	pVertex[0].vPosition = { -4.f, 5.f, 0.f };
	pVertex[0].vTexUV = { 0.f, 0.f };

	pVertex[1].vPosition = { 4.f, 5.f, 0.f };
	pVertex[1].vTexUV = { 1.f, 0.f };

	pVertex[2].vPosition = { 4.f, -0.f, 0.f };
	pVertex[2].vTexUV = { 1.f, 1.f };

	pVertex[3].vPosition = { -4.f, -0.f, 0.f };
	pVertex[3].vTexUV = { 0.f, 1.f };
	
	m_pVB->Unlock();

	INDEX32*			pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	// 오른쪽 위
	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;
	
	// 왼쪽 아래
	pIndex[1]._0 = 0;
	pIndex[1]._1 = 2;
	pIndex[1]._2 = 3;

	m_pIB->Unlock();


	return S_OK;
}

void CBossBf::Render_Buffer(void)
{
	CVIBuffer::Render_Buffer();
}

CBossBf * CBossBf::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBossBf *	pInstance = new CBossBf(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))
	{
		Safe_Release(pInstance);
		MSG_BOX("BossBf Create Failed");
		return nullptr;
	}

	return pInstance;
}

CComponent * CBossBf::Clone(void)
{
	return new CBossBf(*this);
}

void CBossBf::Free(void)
{
	CVIBuffer::Free();
}
