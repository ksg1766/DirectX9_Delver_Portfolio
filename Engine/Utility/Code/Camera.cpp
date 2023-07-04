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
}

void CCamera::SetProjection()
{
}

CCamera* CCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	return nullptr;
}

CComponent* CCamera::Clone(void)
{
	return nullptr;
}

void CCamera::Free()
{
}
