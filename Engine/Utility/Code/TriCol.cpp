#include "..\..\Header\TriCol.h"

CTriCol::CTriCol()
{
}

CTriCol::CTriCol(LPDIRECT3DDEVICE9 pGraphicDev)
	:  CVIBuffer(pGraphicDev)
{
}

CTriCol::CTriCol(const CTriCol & rhs)
	: CVIBuffer(rhs)
{
}


CTriCol::~CTriCol()
{
}

HRESULT CTriCol::Ready_Buffer(void)
{
	m_dwFVF = FVF_COL;
	m_dwTriCnt = 1;
	m_dwVtxCnt = 3;
	m_dwVtxSize = sizeof(VTXCOL);
		
	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	VTXCOL*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);
	// 1. 어디서부터 메모리를 잠글 것인가
	// 2. 0인 경우 전체 영역을 잠금
	// 3. 버텍스 버퍼에 저장된 정점들 중 첫 번째 주소를 반환
	// 4. 잠그는 형태, 정적 버퍼인 경우는 0을 넣어줌

	pVertex[0].vPosition = { 0.f, 1.f, 0.f };
	pVertex[0].dwColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);

	pVertex[1].vPosition = { 1.f, -1.f, 0.f };
	pVertex[1].dwColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);

	pVertex[2].vPosition = { -1.f, -1.f, 0.f };
	pVertex[2].dwColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);

	m_pVB->Unlock();


	return S_OK;
}

void CTriCol::Render_Buffer(void)
{
	CVIBuffer::Render_Buffer();
}

CTriCol * CTriCol::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTriCol *	pInstance = new CTriCol(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))
	{
		Safe_Release(pInstance);
		MSG_BOX("TriCol Create Failed");
		return nullptr;
	}

	return pInstance;
}

CComponent * CTriCol::Clone(void)
{
	return new CTriCol(*this);
}

void CTriCol::Free(void)
{
	CVIBuffer::Free();
}
