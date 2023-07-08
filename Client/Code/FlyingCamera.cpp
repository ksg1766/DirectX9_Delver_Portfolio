#include "stdafx.h"
#include "..\Header\FlyingCamera.h"
#include "Export_Function.h"
#include "CameraManager.h"

CFlyingCamera::CFlyingCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CFlyingCamera::CFlyingCamera(const CFlyingCamera& rhs)
	: Engine::CGameObject(rhs)
{
}

CFlyingCamera::~CFlyingCamera()
{
	Free();
}

HRESULT CFlyingCamera::Ready_Object(void)
{
	m_eObjectTag = OBJECTTAG::CAMERA;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->Translate(_vec3(0.f, 10.f, 0.f));

	CCameraManager::GetInstance()->Add_Camera(CAMERA_TYPE::PERSPECTIVE, this);

	return S_OK;
}

_int CFlyingCamera::Update_Object(const _float& fTimeDelta)
{
	Key_Input(fTimeDelta);
	_int iExit = __super::Update_Object(fTimeDelta);

	return iExit;
}

void CFlyingCamera::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CFlyingCamera::Render_Object(void)
{
}

HRESULT CFlyingCamera::Add_Component(void)
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

void CFlyingCamera::Key_Input(const _float& fTimeDelta)
{
	if (Engine::InputDev()->Key_Pressing(DIK_W))
		m_pTransform->Translate(m_fSpeed * fTimeDelta * m_pTransform->m_vInfo[INFO_LOOK]);

	if (Engine::InputDev()->Key_Pressing(DIK_S))
		m_pTransform->Translate(m_fSpeed * fTimeDelta * -m_pTransform->m_vInfo[INFO_LOOK]);

	if (Engine::InputDev()->Key_Pressing(DIK_A))
		m_pTransform->Translate(m_fSpeed * fTimeDelta * -m_pTransform->m_vInfo[INFO_RIGHT]);

	if (Engine::InputDev()->Key_Pressing(DIK_D))
		m_pTransform->Translate(m_fSpeed * fTimeDelta * m_pTransform->m_vInfo[INFO_RIGHT]);

	_long		dwMouseMove = 0;

	if (dwMouseMove = Engine::InputDev()->Get_DIMouseMove(DIMS_X))
		m_pTransform->Rotate(ROT_Y, D3DXToRadian(dwMouseMove) * fTimeDelta * 2.f);

	if (dwMouseMove = Engine::InputDev()->Get_DIMouseMove(DIMS_Y))
		m_pTransform->Rotate(ROT_X, D3DXToRadian(dwMouseMove) * fTimeDelta * 2.f);

	POINT		pt{ WINCX >> 1, WINCY >> 1 };
	ClientToScreen(g_hWnd, &pt);
	SetCursorPos(pt.x, pt.y);
}

CFlyingCamera* CFlyingCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CFlyingCamera* pInstance = new CFlyingCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("FlyingCamera Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CFlyingCamera::Free()
{
	__super::Free();
}
