#include "..\..\Header\Collider.h"
#include "GameObject.h"
#include "Transform.h"

UINT CCollider::g_iNextID = 0;

CCollider::CCollider()
{
	::ZeroMemory(&m_vAxisDir, sizeof(_vec3));
	::ZeroMemory(&m_fAxisLen, sizeof(_float));
}

CCollider::CCollider(LPDIRECT3DDEVICE9 pGraphicDev) :CComponent(pGraphicDev), m_iCol(0)
{
	::ZeroMemory(&m_vAxisDir, sizeof(_vec3));
	::ZeroMemory(&m_fAxisLen, sizeof(_float));
}

CCollider::CCollider(const CCollider & rhs) : CComponent(rhs), m_iCol(0)
{
	::ZeroMemory(&m_vAxisDir, sizeof(_vec3));
	::ZeroMemory(&m_fAxisLen, sizeof(_float));
}

CCollider::~CCollider()
{

}

HRESULT CCollider::Ready_Collider()
{
	m_iID = g_iNextID++;
	ZeroMemory(m_vCenterPos, sizeof(_vec3));
	ZeroMemory(m_vAxisDir, 3 * sizeof(_vec3));
	ZeroMemory(m_fAxisLen, 3 * sizeof(float));

	return S_OK;
}

void CCollider::LateUpdate_Component()
{
	CTransform* pTransform = m_pHost->m_pTransform;
	m_vCenterPos = pTransform->m_vInfo[INFO_POS];

	//for (UINT i = 0; i < 3; ++i)
	//	::CopyMemory(&m_vAxisDir[i], &pTransform->m_vInfo[i], sizeof(_vec3));
}

void CCollider::Render_Collider()
{
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXCOL));

	m_pGraphicDev->SetFVF(VTXCOL::format);
	m_pGraphicDev->SetIndices(m_pIB);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	_matrix matCollider;
	D3DXMatrixIdentity(&matCollider);

	// Fixing Bounding Box
	for (_int i = 0; i < 3; ++i)
		::CopyMemory(&matCollider.m[i], &m_vAxisDir[i], sizeof(_vec3));
	::CopyMemory(&matCollider.m[3], &m_vCenterPos, sizeof(_vec3));

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matCollider);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);

	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
}

HRESULT CCollider::InitOBB(_vec3 & _vCenter, _vec3 * _vAxisDir, float * _fAxisLen)
{
	m_vCenterPos = _vCenter;

	_vec3 vAxisDir[3];

	for (int i = 0; i < INFO_POS; ++i)
	{
		::CopyMemory(&vAxisDir[i], _vAxisDir[i], sizeof(_vec3));
		D3DXVec3Normalize(&vAxisDir[i], &vAxisDir[i]);
		m_vAxisDir[i] = vAxisDir[i];
		m_fAxisLen[i] = _fAxisLen[i];
	}

#if _DEBUG

	FAILED_CHECK_RETURN(m_pGraphicDev->CreateVertexBuffer(8 * sizeof(VTXCOL), // 생성할 버퍼의 크기
		0, // 생성하고자 버텍스 버퍼의 종류(0인 경우 정적버퍼, D3DUSAGE_DYNAMIC 지정 시 동적 버퍼)
		VTXCOL::format,		// 버텍스의 속성 옵션
		D3DPOOL_MANAGED,	// 메모리 저장 방식(정적 버퍼인 경우 MANAGED)
		&m_pVB,				// 결과 값 컴객체
		NULL),
		E_FAIL);

	FAILED_CHECK_RETURN(m_pGraphicDev->CreateIndexBuffer(12 * sizeof(INDEX32), // 생성할 버퍼의 크기
		0, // 생성하고자 버텍스 버퍼의 종류(0인 경우 정적버퍼, D3DUSAGE_DYNAMIC 지정 시 동적 버퍼)
		D3DFMT_INDEX32,		// 버텍스의 속성 옵션
		D3DPOOL_MANAGED, // 메모리 저장 방식(정적 버퍼인 경우 MANAGED)
		&m_pIB,			// 결과 값 컴객체
		NULL),
		E_FAIL);

	m_pVB->AddRef();
	m_pIB->AddRef();

	VTXCOL* pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	// 전면
	pVertex[0].vPosition = { m_vCenterPos.x - m_fAxisLen[0], m_vCenterPos.y + m_fAxisLen[1], m_vCenterPos.z - m_fAxisLen[2] };
	pVertex[0].dwColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);

	pVertex[1].vPosition = { m_vCenterPos.x + m_fAxisLen[0], m_vCenterPos.y + m_fAxisLen[1], m_vCenterPos.z - m_fAxisLen[2] };
	pVertex[1].dwColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);

	pVertex[2].vPosition = { m_vCenterPos.x + m_fAxisLen[0], m_vCenterPos.y - m_fAxisLen[1], m_vCenterPos.z - m_fAxisLen[2] };
	pVertex[2].dwColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);

	pVertex[3].vPosition = { m_vCenterPos.x - m_fAxisLen[0], m_vCenterPos.y - m_fAxisLen[1], m_vCenterPos.z - m_fAxisLen[2] };
	pVertex[3].dwColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);

	// 후면
	pVertex[4].vPosition = { m_vCenterPos.x - m_fAxisLen[0], m_vCenterPos.y + m_fAxisLen[1], m_vCenterPos.z + m_fAxisLen[2] };
	pVertex[4].dwColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);

	pVertex[5].vPosition = { m_vCenterPos.x + m_fAxisLen[0], m_vCenterPos.y + m_fAxisLen[1], m_vCenterPos.z + m_fAxisLen[2] };
	pVertex[5].dwColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);

	pVertex[6].vPosition = { m_vCenterPos.x + m_fAxisLen[0], m_vCenterPos.y - m_fAxisLen[1], m_vCenterPos.z + m_fAxisLen[2] };
	pVertex[6].dwColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);

	pVertex[7].vPosition = { m_vCenterPos.x - m_fAxisLen[0], m_vCenterPos.y - m_fAxisLen[1], m_vCenterPos.z + m_fAxisLen[2] };
	pVertex[7].dwColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);

	m_pVB->Unlock();

	INDEX32* pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

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
#endif
}

void CCollider::OnCollisionEnter(CCollider * _pOther)
{
	++m_iCol;
	m_pHost->OnCollisionEnter(_pOther);
}

void CCollider::OnCollisionStay(CCollider * _pOther)
{
	m_pHost->OnCollisionStay(_pOther);
}

void CCollider::OnCollisionExit(CCollider * _pOther)
{
	--m_iCol;
	m_pHost->OnCollisionExit(_pOther);
}

CCollider * CCollider::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCollider*		pInstance = new CCollider(pGraphicDev);

	if (FAILED(pInstance->Ready_Collider()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Collider Create Failed");
		return nullptr;
	}

	return pInstance;
}

CComponent * CCollider::Clone(void)
{
	return new CCollider(*this);
}

void CCollider::Free()
{
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);

	CComponent::Free();
}
