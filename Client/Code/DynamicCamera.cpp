#include "stdafx.h"
#include "..\Header\DynamicCamera.h"

CDynamicCamera::CDynamicCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCamera(pGraphicDev)
{
}


CDynamicCamera::~CDynamicCamera()
{
}

HRESULT CDynamicCamera::Ready_Object(const _vec3 * pEye, const _vec3 * pAt, const _vec3 * pUp, const float & fFov, const float & fAspect, const float & fNear, const float & fFar)
{
	m_vEye = *pEye;
	m_vAt = *pAt;
	m_vUp = *pUp;

	m_fFar = fFar;
	m_fNear = fNear;
	m_fAspect = fAspect;
	m_fFov = fFov;

	FAILED_CHECK_RETURN(CCamera::Ready_Object(), E_FAIL);
	m_eObjectTag = OBJECTTAG::CAMERA;

	return S_OK;
}

_int CDynamicCamera::Update_Object(const _float & fTimeDelta)
{
	Key_Input(fTimeDelta);

	if (false == m_bFix)
	{
		Mouse_Move();
		Mouse_Fix();
	}	

	_int iExit = CCamera::Update_Object(fTimeDelta);

	return iExit;
}

void CDynamicCamera::Key_Input(const _float & fTimeDelta)
{


	_matrix		matCamWorld;
	D3DXMatrixInverse(&matCamWorld, 0, &m_matView);

	if (Engine::Get_DIKeyState(DIK_W))
	{
		_vec3		vLook;
		memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));

		_vec3	vLength = *D3DXVec3Normalize(&vLook, &vLook) * m_fSpeed * fTimeDelta;

		m_vEye += vLength;
		m_vAt += vLength;

	}

	if (Engine::Get_DIKeyState(DIK_S))
	{
		_vec3		vLook;
		memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));

		_vec3	vLength = *D3DXVec3Normalize(&vLook, &vLook) * m_fSpeed * fTimeDelta;

		m_vEye -= vLength;
		m_vAt  -= vLength;
	}

	if (Engine::Get_DIKeyState(DIK_A))
	{
		_vec3		vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3	vLength = *D3DXVec3Normalize(&vRight, &vRight) * m_fSpeed * fTimeDelta;

		m_vEye -= vLength;
		m_vAt  -= vLength;
	}

	if (Engine::Get_DIKeyState(DIK_D))
	{
		_vec3		vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3	vLength = *D3DXVec3Normalize(&vRight, &vRight) * m_fSpeed * fTimeDelta;

		m_vEye += vLength;
		m_vAt  += vLength;
	}

	if (Engine::Get_DIKeyState(DIK_TAB))
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
	
	if (false == m_bFix)
		return;
}

void CDynamicCamera::Mouse_Move()
{
	_matrix		matCamWorld;
	D3DXMatrixInverse(&matCamWorld, 0, &m_matView);

	_long		dwMouseMove = 0;

	if (dwMouseMove = Engine::Get_DIMouseMove(DIMS_Y))
	{
		_vec3	vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3	vLook = m_vAt - m_vEye;
		_matrix		matRot;

		D3DXMatrixRotationAxis(&matRot, &vRight, D3DXToRadian(dwMouseMove / 10.f));
		D3DXVec3TransformNormal(&vLook, &vLook, &matRot);
		
		m_vAt = m_vEye + vLook;
	}

	if (dwMouseMove = Engine::Get_DIMouseMove(DIMS_X))
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


CDynamicCamera * CDynamicCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev,
	const _vec3 * pEye, const _vec3 * pAt, const _vec3 * pUp,
	const float& fFov, const float & fAspect, const float & fNear, const float & fFar)
{
	CDynamicCamera *	pInstance = new CDynamicCamera(pGraphicDev);

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
	CCamera::Free();
}
