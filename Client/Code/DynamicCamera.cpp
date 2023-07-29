#include "stdafx.h"
#include "..\Header\DynamicCamera.h"
#include "Player.h"

CDynamicCamera::CDynamicCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CTempCamera(pGraphicDev)
{
}


CDynamicCamera::~CDynamicCamera()
{
}

HRESULT CDynamicCamera::Ready_Object(const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const float& fFov, const float& fAspect, const float& fNear, const float& fFar)
{
	m_vEye = *pEye;
	m_vAt = *pAt;
	m_vUp = *pUp;

	m_fFar = fFar;
	m_fNear = fNear;
	m_fAspect = fAspect;
	m_fFov = fFov;

	// Camera Shake
	m_bCameraCheck = false;
	m_bShaking = false; // Èçµé¸² »óÅÂ
	m_fShakeElipsedTime = 0.f; // ÁßÃ¸½Ã°£

	m_fAmplitude = 0.005f; // ÁøÆø
	m_fDuration = 0.1f;  // Èçµå´Â ½Ã°£
	m_fFrequency = 1.f; // Èçµå´Â ¼Óµµ
	m_vOriginPos = _vec3(0.f, 0.f, 0.f);

	FAILED_CHECK_RETURN(CTempCamera::Ready_Object(), E_FAIL);
	m_eObjectTag = OBJECTTAG::CAMERA;
	m_eCamera_Mode = CAMERA_MODE::CAMERA_FIRST;

	return S_OK;
}

_int CDynamicCamera::Update_Object(const _float& fTimeDelta)
{
	Key_Input(fTimeDelta);


	if (false == m_bFix)
	{
		if (m_eCamera_Mode == CAMERA_MODE::CAMERA_THIRD)
			Mouse_Move();
		
		Mouse_Fix();
	}

	switch (m_eCamera_Mode)
	{
	case CAMERA_MODE::CAMERA_FIRST:
		First_Camera();
		break;
	case CAMERA_MODE::CAMERA_THIRD:
		//Third_Camera();
		break;
	}

	if (m_bShaking)
	{
		m_fShakeElipsedTime += fTimeDelta;

		if (m_fShakeElipsedTime < m_fDuration)
		{
			_float X = m_fAmplitude * cosf(m_fShakeElipsedTime * m_fFrequency + (((_float)rand() / (_float)RAND_MAX) * D3DX_PI));
			_float Y = m_fAmplitude * -sinf(m_fShakeElipsedTime * m_fFrequency + (((_float)rand() / (_float)RAND_MAX) * D3DX_PI));
			m_vEye += _vec3(X, Y, 0);
		}
		else
		{
			m_vEye = m_vOriginPos;
			m_bShaking = false;
		}
	}

	_int iExit = CTempCamera::Update_Object(fTimeDelta);

	return iExit;
}

void CDynamicCamera::LateUpdate_Object()
{
	CTempCamera::LateUpdate_Object();
}

HRESULT CDynamicCamera::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::PrototypeManager()->Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENTTAG::TRANSFORM, pComponent);

	return S_OK;
}

void CDynamicCamera::Key_Input(const _float& fTimeDelta)
{
	_matrix		matCamWorld;
	D3DXMatrixInverse(&matCamWorld, 0, &m_matView);

	if (Engine::InputDev()->Key_Pressing(DIK_UP))
	{
		_vec3		vLook;
		memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));

		_vec3	vLength = *D3DXVec3Normalize(&vLook, &vLook) * m_fSpeed * fTimeDelta;

		m_vEye += vLength;
		m_vAt += vLength;
	}

	if (Engine::InputDev()->Key_Pressing(DIK_DOWN))
	{
		_vec3		vLook;
		memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));

		_vec3	vLength = *D3DXVec3Normalize(&vLook, &vLook) * m_fSpeed * fTimeDelta;

		m_vEye -= vLength;
		m_vAt -= vLength;
	}

	if (Engine::InputDev()->Key_Pressing(DIK_LEFT))
	{
		_vec3		vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3	vLength = *D3DXVec3Normalize(&vRight, &vRight) * m_fSpeed * fTimeDelta;

		m_vEye -= vLength;
		m_vAt -= vLength;
	}

	if (Engine::InputDev()->Key_Pressing(DIK_RIGHT))
	{
		_vec3		vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3	vLength = *D3DXVec3Normalize(&vRight, &vRight) * m_fSpeed * fTimeDelta;

		m_vEye += vLength;
		m_vAt += vLength;
	}

	if (Engine::InputDev()->Key_Pressing(DIK_TAB))
	{
		if (m_bCheck)
			return;

		m_bCheck = true;

		if (m_bFix)
			m_bFix = false;
		else
			m_bFix = true;
	}
	else
		m_bCheck = false;

	if (Engine::InputDev()->Key_Pressing(DIK_F5))
	{
		Shake_Camera();
	}

	if (Engine::InputDev()->Key_Pressing(DIK_F2))
	{
		m_eCamera_Mode = CAMERA_MODE::CAMERA_FIRST;
		m_bCameraCheck = false;
	}

	if (Engine::InputDev()->Key_Pressing(DIK_F3))
	{
		m_eCamera_Mode = CAMERA_MODE::CAMERA_THIRD;
		m_bCameraCheck = true;
		Third_Camera();
	}

	if (false == m_bFix)
		return;
}

void CDynamicCamera::Mouse_Move()
{
	_matrix		matCamWorld;
	D3DXMatrixInverse(&matCamWorld, 0, &m_matView);

	_long		dwMouseMove = 0;

	if (dwMouseMove = Engine::InputDev()->Get_DIMouseMove(DIMS_Y))
	{
		_vec3	vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3	vLook = m_vAt - m_vEye;
		_matrix		matRot;

		D3DXMatrixRotationAxis(&matRot, &vRight, D3DXToRadian(dwMouseMove / 10.f));
		D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

		m_vAt = m_vEye + vLook;
	}

	if (dwMouseMove = Engine::InputDev()->Get_DIMouseMove(DIMS_X))
	{
		_vec3	vUp{ 0.f, 1.f, 0.f };

		_vec3	vLook = m_vAt - m_vEye;
		_matrix		matRot;

		D3DXMatrixRotationAxis(&matRot, &vUp, D3DXToRadian(dwMouseMove / 10.f));
		D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

		m_vAt = m_vEye + vLook;
	}
}

void CDynamicCamera::Mouse_Fix()
{
	POINT		pt{ WINCX >> 1, WINCY >> 1 };

	ClientToScreen(g_hWnd, &pt);
	SetCursorPos(pt.x, pt.y);
}

void CDynamicCamera::First_Camera()
{
	CComponent* pComponent = SceneManager()->Get_Scene()->Get_MainPlayer()->m_pTransform;

	const _matrix& matPlayerWorld = dynamic_cast<CTransform*>(pComponent)->WorldMatrix();

	_vec3&	vPlayerLook = dynamic_cast<CTransform*>(pComponent)->m_vInfo[INFO_LOOK];
	_vec3&	vPlayerUp = dynamic_cast<CTransform*>(pComponent)->m_vInfo[INFO_UP];

	//_vec3 vCameraOffset = vPlayerUp * 1.0f + vPlayerLook * 1.0f;
	//_vec3 vCameraPosition = dynamic_cast<CTransform*>(pComponent)->m_vInfo[INFO_POS] + vCameraOffset;

	m_vAt = { matPlayerWorld._41 + vPlayerLook.x, matPlayerWorld._42 + vPlayerLook.y , matPlayerWorld._43 + vPlayerLook.z };
	m_vEye = { matPlayerWorld._41, matPlayerWorld._42, matPlayerWorld._43 };
	//m_vEye += ((vPlayerUp * 0.5f) + (vPlayerLook * 0.5f));
	//m_vAt += ((vPlayerUp * 0.5f) + (vPlayerLook * 0.5f));

	m_vUp = vPlayerUp;
}

void CDynamicCamera::Third_Camera()
{
	if (m_bCameraCheck)
	{
		m_vEye = _vec3(0.f, 10.f, -10.f);
		m_vAt = _vec3(0.f, 0.f, 1.f);
		m_vUp = _vec3(0.f, 1.f, 0.f);

		m_fFar = 1000.f;
		m_fNear = 0.1f;
		m_fAspect = (_float)WINCX / WINCY;
		m_fFov = D3DXToRadian(60.f);
	}
}

void CDynamicCamera::Set_ShakeForce(_float _ShakeTime, _float _Amplitude, _float _Duration, _float _Frequency)
{
	m_fShakeElipsedTime = _ShakeTime;
	m_fAmplitude = _Amplitude;
	m_fDuration = _Duration;
	m_fFrequency = _Frequency;
}

void CDynamicCamera::Reset_ShakeForce()
{
	m_fShakeElipsedTime = 0.f; // ÁßÃ¸½Ã°£

	m_fAmplitude = 0.f; // ÁøÆø
	m_fDuration = 0.f;  // Èçµå´Â ½Ã°£
	m_fFrequency = 0.f; // Èçµå´Â ¼Óµµ
}

void CDynamicCamera::Shake_Camera()
{
	CComponent* pComponent = SceneManager()->Get_Scene()->Get_MainPlayer()->m_pTransform;

	_vec3	vPlayerPos = dynamic_cast<CTransform*>(pComponent)->m_vInfo[INFO_POS];

	m_fShakeElipsedTime = 0.f;
	m_vOriginPos = vPlayerPos;
	m_bShaking = true;
}

void CDynamicCamera::Drunk_Camera()
{
	CComponent* pComponent = SceneManager()->Get_Scene()->Get_MainPlayer()->m_pTransform;

	_vec3	vPlayerPos = dynamic_cast<CTransform*>(pComponent)->m_vInfo[INFO_POS];

	m_fShakeElipsedTime = 0.f;
	m_vOriginPos = vPlayerPos;
}

CDynamicCamera* CDynamicCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev,
	const _vec3* pEye, const _vec3* pAt, const _vec3* pUp,
	const float& fFov, const float& fAspect, const float& fNear, const float& fFar)
{
	CDynamicCamera* pInstance = new CDynamicCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_Object(pEye, pAt, pUp, fFov, fAspect, fNear, fFar)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Dynamic Camera Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CDynamicCamera::Free()
{
	CTempCamera::Free();
}
