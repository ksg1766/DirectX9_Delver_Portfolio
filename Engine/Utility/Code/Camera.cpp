#include "..\..\Header\Camera.h"
#include "Transform.h"

CCamera::CCamera()
{
}

CCamera::CCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev)
{
}

CCamera::CCamera(const CCamera& rhs)
	: CComponent(rhs)
{
}

CCamera::~CCamera()
{
}

HRESULT CCamera::Ready_Camera()
{
	return S_OK;
}

_int CCamera::Update_Component(const _float& fTimeDelta)
{
	return _int();
}

void CCamera::LateUpdate_Component()
{
	//Set_ViewSpcae();
	//Set_Projection();
}

void CCamera::Set_ViewSpcae()
{
	D3DXMatrixLookAtLH(&m_matView,
		&m_pTransform->m_vInfo[INFO_POS],
		&(m_pTransform->m_vInfo[INFO_POS] + m_pTransform->m_vInfo[INFO_LOOK]),
		&m_pTransform->m_vInfo[INFO_UP]);

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
}

void CCamera::Set_Projection(const CAMERA_TYPE eMode)
{
	switch (eMode)
	{
	case CAMERA_TYPE::PERSPECTIVE:
		D3DXMatrixPerspectiveFovLH(&m_matProj, D3DX_PI / 4.f, WINCX / (float)WINCY, 0.1f, 1000.0f);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);
		break;
	case CAMERA_TYPE::ORTHOGRAPHIC:	// custom
		D3DXMatrixOrthoLH(&m_matProj, WINCY, WINCY, 1.0f, 1000.0f);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);
		break;
	}
}

CCamera* CCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCamera* pInstance = new CCamera(pGraphicDev);
	// TODO : 매개변수 추가
	if (FAILED(pInstance->Ready_Camera()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Camera Create Failed");
		return nullptr;
	}

	return pInstance;
}

CComponent* CCamera::Clone(void)
{
	return new CCamera(*this);
}

void CCamera::Free()
{
	CComponent::Free();
}
