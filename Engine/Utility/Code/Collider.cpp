#include "..\..\Header\Collider.h"
#include "GameObject.h"
#include "Transform.h"

UINT CCollider::g_iNextID = 0;

CCollider::CCollider()
{
}

CCollider::CCollider(LPDIRECT3DDEVICE9 pGraphicDev) :CComponent(pGraphicDev), m_iCol(0)
{
}

CCollider::CCollider(const CCollider & rhs) : CComponent(rhs), m_iCol(0)
{
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
	//m_vCenterPos = dynamic_cast<CTransform*>(m_pHost->Get_Component(L"Transform", ID_DYNAMIC))->Position();
	//_matrix matWorld = dynamic_cast<CTransform*>(m_pHost->Get_Component(L"Transform", ID_DYNAMIC))->WorldMatrix();

	//for (UINT i = 0; i < 3; ++i)
		//CopyMemory()
	//	memcpy(&m_vAxisDir[i], &matWorld.m[i][0], 3 * sizeof(float));
}

void CCollider::Render_Collider()
{

}

void CCollider::InitOBB(_vec3 & _vCenter, _vec3 * _vAxisDir, float * _fAxisLen)
{
	m_vCenterPos = _vCenter;

	m_vAxisDir[0] = _vAxisDir[0];
	m_vAxisDir[1] = _vAxisDir[1];
	m_vAxisDir[2] = _vAxisDir[2];

	m_fAxisLen[0] = _fAxisLen[0];
	m_fAxisLen[1] = _fAxisLen[1];
	m_fAxisLen[2] = _fAxisLen[2];

	//

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
		MSG_BOX("Transform Create Failed");
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
	CComponent::Free();
}
