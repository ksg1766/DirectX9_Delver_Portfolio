#include "stdafx.h"
#include "..\Header\OrthoCamera.h"
#include "Export_Function.h"
#include "CameraManager.h"

COrthoCamera::COrthoCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
	::ZeroMemory(&m_backupViewPort, sizeof(D3DVIEWPORT9));
}

COrthoCamera::COrthoCamera(const COrthoCamera& rhs)
	: Engine::CGameObject(rhs)
{
	::ZeroMemory(&m_backupViewPort, sizeof(D3DVIEWPORT9));
}

COrthoCamera::~COrthoCamera()
{
	Free();
}

HRESULT COrthoCamera::Ready_Object(void)
{
	m_eObjectTag = OBJECTTAG::CAMERA;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->Translate(_vec3(0.f, 128.f, 0.f));
	m_pTransform->Rotate(ROT_X, D3DXToRadian(90.f));

	CCameraManager::GetInstance()->Add_Camera(CAMERA_TYPE::ORTHOGRAPHIC, this);
	m_pGraphicDev->GetViewport(&m_backupViewPort);

	return S_OK;
}

_int COrthoCamera::Update_Object(const _float& fTimeDelta)
{
	_int iExit = __super::Update_Object(fTimeDelta);

	return iExit;
}

void COrthoCamera::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void COrthoCamera::Render_Object(void)
{
}

HRESULT COrthoCamera::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	pComponent = m_pCamera = dynamic_cast<CCamera*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Camera"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::CAMERA, pComponent);

	for (int i = 0; i < ID_END; ++i)
		for (auto& iter : m_mapComponent[i])
			iter.second->Init_Property(this);

	return S_OK;
}

void COrthoCamera::Set_OrthoView()
{
	D3DVIEWPORT9 viewPort;
	//m_pGraphicDev->GetViewport(&m_backupViewPort);
	viewPort.Width = WINCY;
	viewPort.Height = WINCY;

	viewPort.X = (WINCX - WINCY) * 0.5f;
	viewPort.Y = 0.f;

	viewPort.MinZ = 0.f;
	viewPort.MaxZ = 1.f;

	m_pGraphicDev->SetViewport(&viewPort);
	m_pCamera->Set_ViewSpcae();
}

void COrthoCamera::ReturnViewPort()
{
	m_pGraphicDev->SetViewport(&m_backupViewPort);
	//m_pGraphicDev->GetViewport(&m_backupViewPort);
}

COrthoCamera* COrthoCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	COrthoCamera* pInstance = new COrthoCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("FlyingCamera Create Failed");
		return nullptr;
	}

	return pInstance;
}

void COrthoCamera::Free()
{
	__super::Free();
}
