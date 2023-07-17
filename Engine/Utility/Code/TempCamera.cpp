#include "..\..\Header\TempCamera.h"

CTempCamera::CTempCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CTempCamera::CTempCamera(const CTempCamera& rhs)
	: CGameObject(rhs)//, ~~~~
{
}

CTempCamera::~CTempCamera()
{
}

HRESULT CTempCamera::Ready_Object()
{
	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	//CPipeline::MakeViewMatrix(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);

	D3DXMatrixPerspectiveFovLH(&m_matProj, m_fFov, m_fAspect, m_fNear, m_fFar);
	//CPipeline::MakeProjMatrix(&m_matProj, m_fFov, m_fAspect, m_fNear, m_fFar);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	return S_OK;
}

_int CTempCamera::Update_Object(const _float & fTimeDelta)
{
	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);

	return 0;
}

void CTempCamera::LateUpdate_Object()
{
}

void CTempCamera::Free(void)
{
	CGameObject::Free();
}
