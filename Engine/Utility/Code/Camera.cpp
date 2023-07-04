#include "..\..\Header\Camera.h"

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

void CCamera::SetViewSpcae()
{


	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
}

void CCamera::SetProjection()
{


	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);
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
}
