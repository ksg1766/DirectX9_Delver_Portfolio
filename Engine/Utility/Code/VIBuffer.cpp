#include "..\..\Header\VIBuffer.h"

CVIBuffer::CVIBuffer()
	: m_pVB(nullptr), m_pIB(nullptr)
{
}

CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev), m_pVB(nullptr), m_pIB(nullptr)
	, m_dwVtxCnt(0), m_dwVtxSize(0), m_dwTriCnt(0), m_dwFVF(0), m_dwIdxSize(0), m_IdxFmt(D3DFMT_UNKNOWN)
{

}

CVIBuffer::CVIBuffer(const CVIBuffer & rhs)
	: CComponent(rhs)
	, m_pVB(rhs.m_pVB)
	, m_pIB(rhs.m_pIB)
	, m_dwVtxCnt(rhs.m_dwVtxCnt)
	, m_dwVtxSize(rhs.m_dwVtxSize)
	, m_dwTriCnt(rhs.m_dwTriCnt)
	, m_dwFVF(rhs.m_dwFVF)
	, m_dwIdxSize(rhs.m_dwIdxSize)
	, m_IdxFmt(rhs.m_IdxFmt)
{
	m_pVB->AddRef();
	m_pIB->AddRef();
}

CVIBuffer::~CVIBuffer()
{
}

HRESULT CVIBuffer::Ready_Buffer(void)
{
	FAILED_CHECK_RETURN(m_pGraphicDev->CreateVertexBuffer(m_dwVtxCnt * m_dwVtxSize, // 생성할 버퍼의 크기
															0, // 생성하고자 버텍스 버퍼의 종류(0인 경우 정적버퍼, D3DUSAGE_DYNAMIC 지정 시 동적 버퍼)
															m_dwFVF,		// 버텍스의 속성 옵션
															D3DPOOL_MANAGED, // 메모리 저장 방식(정적 버퍼인 경우 MANAGED)
															&m_pVB,			// 결과 값 컴객체
															NULL), 
															E_FAIL);

	FAILED_CHECK_RETURN(m_pGraphicDev->CreateIndexBuffer(m_dwTriCnt * m_dwIdxSize, // 생성할 버퍼의 크기
															0, // 생성하고자 버텍스 버퍼의 종류(0인 경우 정적버퍼, D3DUSAGE_DYNAMIC 지정 시 동적 버퍼)
															m_IdxFmt,		// 버텍스의 속성 옵션
															D3DPOOL_MANAGED, // 메모리 저장 방식(정적 버퍼인 경우 MANAGED)
															&m_pIB,			// 결과 값 컴객체
															NULL),
															E_FAIL);

	return S_OK;
}

void CVIBuffer::Render_Buffer(void)
{
	// 디바이스 장치에게 현재 그리려는 메모리 버퍼를 링크 시키는 함수
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);

	// 1. 몇 번 스트림에 버퍼를 전송할 것인가
	// 2. 어떤 대상을 장치에 연결할 것인가
	// 3. 어디서부터 전달할 것인가
	// 4. 어떤 크기 단위로 전달할 것인가

	m_pGraphicDev->SetFVF(m_dwFVF);
	m_pGraphicDev->SetIndices(m_pIB);


	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0,m_dwTriCnt);
	// 1. 삼각형을 그리는 방식
	// 2. 버텍스의 인덱스 값(몇 번 인덱스부터 그릴 것인가, 넣은 값에 따라 기존에 저장된 인덱스 값에 더해서 그려줌)
	// 3. 버텍스 버퍼에 들어가 있는 인덱스 중 몇 번째부터 그릴 것인가
	// 4. 내가 사용하고자 하는 버텍스의 개수
	// 5. 인덱브 버퍼에 들어가 있는 인덱스 중 몇 번째부터 그릴 것인가
	// 6. 그리려는 삼각형 개수


	//m_pGraphicDev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_dwTriCnt);
	// 1. 삼각형을 그리는 방식
	// 2. 몇 번째 버텍스부터 그릴 것인가
	// 3. 그리려는 삼각형 개수


}

void CVIBuffer::Free(void)
{
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);

	CComponent::Free();

}


/*
// D3DXMatrixLookAtLH()	// 뷰스페이스 변환
// D3DXMatrixPerspectiveFovLH() // 원근 투영 행렬 함수*/

