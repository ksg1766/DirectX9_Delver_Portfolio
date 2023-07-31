#include "stdafx.h"
#include "..\Header\FlyingCamera.h"
#include "Export_Function.h"

#include "SceneManager.h"
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

	CCameraManager::GetInstance()->Add_Camera(CAMERA_TYPE::PERSPECTIVE, this);
	CCameraManager::GetInstance()->Set_CurrentCam(this);

	return S_OK;
}

_int CFlyingCamera::Update_Object(const _float& fTimeDelta)
{
	Key_Input(fTimeDelta);

	if (SCENETAG::EDITOR != SceneManager()->Get_Scene()->Get_SceneTag())
	{
		if (false == m_isMouseFree)
		{
			Mouse_Fix();
		}
	}

	if (m_tCameraShaking.m_bShaking)
	{
		m_tCameraShaking.m_fShakeElipsedTime += fTimeDelta;

		if (m_tCameraShaking.m_fShakeElipsedTime < m_tCameraShaking.m_fDuration)
		{
			m_pTransform->m_vInfo[INFO_POS] = m_pTransform->m_pParent->m_vInfo[INFO_POS];

			_float X = m_tCameraShaking.m_fAmplitude * cosf(m_tCameraShaking.m_fShakeElipsedTime * m_tCameraShaking.m_fFrequency + (((_float)rand() / (_float)RAND_MAX) * D3DX_PI));
			_float Y = m_tCameraShaking.m_fAmplitude * -sinf(m_tCameraShaking.m_fShakeElipsedTime * m_tCameraShaking.m_fFrequency + (((_float)rand() / (_float)RAND_MAX) * D3DX_PI));
			m_pTransform->Translate(_vec3(X, Y, 0));
		}
		else
		{
			m_pTransform->Copy_RUL_AddPos(m_pTransform->m_pParent->m_vInfo);
			m_tCameraShaking.m_bShaking = false;
		}
	}

	_int iExit = __super::Update_Object(fTimeDelta);

	return iExit;
}

void CFlyingCamera::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CFlyingCamera::Shake_Camera()
{
	m_tCameraShaking.m_fShakeElipsedTime = 0.f;
	m_tCameraShaking.m_vOriginPos = m_pTransform->m_vInfo[INFO_POS];
	m_tCameraShaking.m_bShaking = true;
}

void CFlyingCamera::Drunk_Camera()
{
	m_tCameraShaking.m_fShakeElipsedTime = 0.f;
	m_tCameraShaking.m_vOriginPos = m_pTransform->m_vInfo[INFO_POS];
}

void CFlyingCamera::Set_ShakeForce(_float _ShakeTime, _float _Amplitude, _float _Duration, _float _Frequency)
{
	m_tCameraShaking.m_fShakeElipsedTime = _ShakeTime;
	m_tCameraShaking.m_fAmplitude = _Amplitude;
	m_tCameraShaking.m_fDuration = _Duration;
	m_tCameraShaking.m_fFrequency = _Frequency;
}

void CFlyingCamera::Reset_ShakeForce()
{
	ZeroMemory(&m_tCameraShaking, sizeof(CameraShaking));
}

void CFlyingCamera::Change_Mode()
{
	if(m_eCameraMode)
		m_pTransform->Copy_RUL_AddPos(m_pTransform->m_pParent->m_vInfo);
	m_eCameraMode = (CAMERA_MODE)((m_eCameraMode + 1) % 2);
	/*// 위와 같은 결과
	if (FIRST_PERSON == m_eCameraMode)
	{
		m_eCameraMode = THIRD_PERSON;
	}
	else
	{
		m_eCameraMode = FIRST_PERSON;
		m_pTransform->Copy_RUL_AddPos(m_pTransform->m_pParent->m_vInfo);
	}*/
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
	if (m_eCameraMode == CAMERA_MODE::THIRD_PERSON || SCENETAG::EDITOR == SceneManager()->Get_Scene()->Get_SceneTag())
	{
		if (Engine::InputDev()->Key_Pressing(DIK_W))
			m_pTransform->Translate(m_fSpeed * fTimeDelta * m_pTransform->m_vInfo[INFO_LOOK]);

		if (Engine::InputDev()->Key_Pressing(DIK_S))
			m_pTransform->Translate(m_fSpeed * fTimeDelta * -m_pTransform->m_vInfo[INFO_LOOK]);

		if (Engine::InputDev()->Key_Pressing(DIK_A))
			m_pTransform->Translate(m_fSpeed * fTimeDelta * -m_pTransform->m_vInfo[INFO_RIGHT]);

		if (Engine::InputDev()->Key_Pressing(DIK_D))
			m_pTransform->Translate(m_fSpeed * fTimeDelta * m_pTransform->m_vInfo[INFO_RIGHT]);

		if (Engine::InputDev()->Key_Pressing(DIK_Q))
			m_pTransform->Translate(m_fSpeed * fTimeDelta * m_pTransform->m_vInfo[INFO_UP]);

		if (Engine::InputDev()->Key_Pressing(DIK_E))
			m_pTransform->Translate(m_fSpeed * fTimeDelta * -m_pTransform->m_vInfo[INFO_UP]);

		_long		dwMouseMove = 0;

		if (dwMouseMove = Engine::InputDev()->Get_DIMouseMove(DIMS_X))
		{
			if (Engine::InputDev()->Mouse_Pressing(DIM_LB))
				m_pTransform->Rotate(ROT_Y, -D3DXToRadian(dwMouseMove) * fTimeDelta * 4.f);
		}

		if (dwMouseMove = Engine::InputDev()->Get_DIMouseMove(DIMS_Y))
		{
			if (Engine::InputDev()->Mouse_Pressing(DIM_LB))
				m_pTransform->Rotate(ROT_X, -D3DXToRadian(dwMouseMove) * fTimeDelta * 4.f);
		}

		if (dwMouseMove = Engine::InputDev()->Get_DIMouseMove(DIMS_X))
		{
			if (Engine::InputDev()->Mouse_Pressing(DIM_RB))
				m_pTransform->Translate(dwMouseMove * fTimeDelta * -m_pTransform->m_vInfo[INFO_RIGHT] * 2.f);
		}

		if (dwMouseMove = Engine::InputDev()->Get_DIMouseMove(DIMS_Y))
		{
			if (Engine::InputDev()->Mouse_Pressing(DIM_RB))
				m_pTransform->Translate(dwMouseMove * fTimeDelta * m_pTransform->m_vInfo[INFO_UP] * 2.f);
		}
	}
	

	if (Engine::InputDev()->Key_Down(DIK_F2) && SCENETAG::EDITOR != SceneManager()->Get_Scene()->Get_SceneTag())
	{
		Change_Mode();
	}

	if (Engine::InputDev()->Key_Down(DIK_TAB))
	{
		if (m_bCheck)
			return;

		m_bCheck = true;

		m_isMouseFree = !m_isMouseFree;
	}
	else
		m_bCheck = false;

	if (Engine::InputDev()->Key_Pressing(DIK_F5))
	{
		Set_ShakeForce(0.f, 0.01f, 3, 2.f);
		Shake_Camera();
	}
}

void CFlyingCamera::Mouse_Fix()
{
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
