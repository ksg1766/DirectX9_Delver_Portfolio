#include "..\..\Header\DynamicBf.h"

CDynamicBf::CDynamicBf()
{
}

CDynamicBf::CDynamicBf(LPDIRECT3DDEVICE9 pGraphicDev)
	:CVIBuffer(pGraphicDev)
{
}

CDynamicBf::CDynamicBf(const CDynamicBf& rhs)
	:CVIBuffer(rhs)
{
}

CDynamicBf::~CDynamicBf()
{
}

HRESULT CDynamicBf::Ready_Buffer(void)
{
	//m_dwFVF = VTXCUBE::format;
	m_dwFVF = VTXNTX::format;
	m_dwTriCnt = 12;
	m_dwVtxCnt = 8;
	//m_dwVtxSize = sizeof(VTXCUBE);
	m_dwVtxSize = sizeof(VTXNTX);
	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	FAILED_CHECK_RETURN(m_pGraphicDev->CreateVertexBuffer(m_dwVtxCnt * m_dwVtxSize, // 생성할 버퍼의 크기
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, // 생성하고자 버텍스 버퍼의 종류(0인 경우 정적버퍼, D3DUSAGE_DYNAMIC 지정 시 동적 버퍼)
		m_dwFVF,		// 버텍스의 속성 옵션
		D3DPOOL_DEFAULT, // 메모리 저장 방식(정적 버퍼인 경우 MANAGED)
		&m_pVB,			// 결과 값 컴객체
		NULL),
		E_FAIL);

	FAILED_CHECK_RETURN(m_pGraphicDev->CreateIndexBuffer(m_dwTriCnt * m_dwIdxSize, // 생성할 버퍼의 크기
		D3DUSAGE_WRITEONLY, // 생성하고자 버텍스 버퍼의 종류(0인 경우 정적버퍼, D3DUSAGE_DYNAMIC 지정 시 동적 버퍼)
		m_IdxFmt,		// 버텍스의 속성 옵션
		D3DPOOL_DEFAULT, // 메모리 저장 방식(정적 버퍼인 경우 MANAGED)
		&m_pIB,			// 결과 값 컴객체
		NULL),
		E_FAIL);

	//VTXCUBE* pVertex = nullptr;
	VTXNTX* pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, D3DLOCK_DISCARD);

	// 전면
	pVertex[0].vPosition = { -1.f, 1.f, -1.f };
	pVertex[0].vNormal = { -0.577350259f, 0.577350259f, 0.577350259f };
	pVertex[0].vTexUV = pVertex[0].vPosition;

	pVertex[1].vPosition = { 1.f, 1.f, -1.f };
	pVertex[0].vNormal = { 0.577350259f, 0.577350259f, 0.577350259f };
	pVertex[1].vTexUV = pVertex[1].vPosition;

	pVertex[2].vPosition = { 1.f, -1.f, -1.f };
	pVertex[0].vNormal = { 0.577350259f, 0.577350259f, -0.577350259f };
	pVertex[2].vTexUV = pVertex[2].vPosition;

	pVertex[3].vPosition = { -1.f, -1.f, -1.f };
	pVertex[0].vNormal = { -0.577350259f, 0.577350259f, -0.577350259f };
	pVertex[3].vTexUV = pVertex[3].vPosition;

	// 후면
	pVertex[4].vPosition = { -1.f, 1.f, 1.f };
	pVertex[0].vNormal = { -0.577350259f, -0.577350259f, 0.577350259f };
	pVertex[4].vTexUV = pVertex[4].vPosition;

	pVertex[5].vPosition = { 1.f, 1.f, 1.f };
	pVertex[0].vNormal = { 0.577350259f, -0.577350259f, 0.577350259f };
	pVertex[5].vTexUV = pVertex[5].vPosition;

	pVertex[6].vPosition = { 1.f, -1.f, 1.f };
	pVertex[0].vNormal = { 0.577350259f, -0.577350259f, -0.577350259f };
	pVertex[6].vTexUV = pVertex[6].vPosition;

	pVertex[7].vPosition = { -1.f, -1.f, 1.f };
	pVertex[0].vNormal = { -0.577350259f, -0.577350259f, -0.577350259f };
	pVertex[7].vTexUV = pVertex[7].vPosition;

	m_pVB->Unlock();

	INDEX32* pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, D3DLOCK_DISCARD);

	// X+
	pIndex[0]._0 = 1;
	pIndex[0]._1 = 5;
	pIndex[0]._2 = 6;

	pIndex[1]._0 = 1;
	pIndex[1]._1 = 6;
	pIndex[1]._2 = 2;

	// X-
	pIndex[2]._0 = 4;
	pIndex[2]._1 = 0;
	pIndex[2]._2 = 3;

	pIndex[3]._0 = 4;
	pIndex[3]._1 = 3;
	pIndex[3]._2 = 7;

	// Y+
	pIndex[4]._0 = 4;
	pIndex[4]._1 = 5;
	pIndex[4]._2 = 1;

	pIndex[5]._0 = 4;
	pIndex[5]._1 = 1;
	pIndex[5]._2 = 0;

	// Y-
	pIndex[6]._0 = 3;
	pIndex[6]._1 = 2;
	pIndex[6]._2 = 6;

	pIndex[7]._0 = 3;
	pIndex[7]._1 = 6;
	pIndex[7]._2 = 7;

	// Z+
	pIndex[8]._0 = 7;
	pIndex[8]._1 = 6;
	pIndex[8]._2 = 5;

	pIndex[9]._0 = 7;
	pIndex[9]._1 = 5;
	pIndex[9]._2 = 4;

	// Z-
	pIndex[10]._0 = 0;
	pIndex[10]._1 = 1;
	pIndex[10]._2 = 2;

	pIndex[11]._0 = 0;
	pIndex[11]._1 = 2;
	pIndex[11]._2 = 3;

	m_pIB->Unlock();

	return S_OK;
}

void CDynamicBf::Render_Buffer(void)
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

CDynamicBf* CDynamicBf::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDynamicBf* pInstance = new CDynamicBf(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))
	{
		Safe_Release(pInstance);
		MSG_BOX("DynamicBf Create Failed");
		return nullptr;
	}

	return pInstance;
}

CComponent* CDynamicBf::Clone(void)
{
	return new CDynamicBf(*this);
}

void CDynamicBf::Free(void)
{
	CComponent::Free();
}
