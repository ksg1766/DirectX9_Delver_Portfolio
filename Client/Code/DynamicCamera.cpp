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

_vec3 CDynamicCamera::Picking(HWND hWnd, const CVIBuffer* pTargetBuffer, const CTransform* pTargetTransform)
{
	//POINT		ptMouse{};
	//GetCursorPos(&ptMouse);
	//ScreenToClient(hWnd, &ptMouse);

	//_vec3		vMousePos;

	//D3DVIEWPORT9		ViewPort;
	//ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	//m_pGraphicDev->GetViewport(&ViewPort);

	////0,0      -> -1, 1
	////400, 300 ->  0, 0
	////800, 600 ->  1, -1

	//// 뷰포트 -> 투영
	//vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
	//vMousePos.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
	//vMousePos.z = 0.f;

	//// 투영 -> 뷰 스페이스
	//_matrix		matProj;
	//m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	//D3DXMatrixInverse(&matProj, 0, &matProj);
	//D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

	//_vec3	vRayPos, vRayDir;

	//vRayPos = _vec3(0.f, 0.f, 0.f);
	//vRayDir = vMousePos - vRayPos;

	//// 뷰 스페이스 -> 월드 스페이스
	//_matrix		matView;
	//m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	//D3DXMatrixInverse(&matView, 0, &matView);

	//D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
	//D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);
	//// 밑으로 수정 필요
	//const	_vec3* pTargetVtxPos = pTargetBuffer
	//const	_vec3* pTerrainVtxPos = pTargetBuffer->Get_VtxPos();

	//_ulong		dwVtxIdx[3]{};

	//_float	fU = 0.f, fV = 0.f, fDist = 0.f;

	//for (_ulong i = 0; i < VTXCNTZ - 1; ++i)
	//{
	//	for (_ulong j = 0; j < VTXCNTX - 1; ++j)
	//	{
	//		_ulong	dwIndex = i * VTXCNTX + j;

	//		// 오른쪽 위
	//		dwVtxIdx[0] = dwIndex + VTXCNTX;
	//		dwVtxIdx[1] = dwIndex + VTXCNTX + 1;
	//		dwVtxIdx[2] = dwIndex + 1;

	//		if (D3DXIntersectTri(&pTerrainVtxPos[dwVtxIdx[1]],
	//			&pTerrainVtxPos[dwVtxIdx[0]],
	//			&pTerrainVtxPos[dwVtxIdx[2]],
	//			&vRayPos, &vRayDir, &fU, &fV, &fDist))
	//		{
	//			// V1 + U(V2 - V1) + V(V3 - V1)

	//			return _vec3(pTerrainVtxPos[dwVtxIdx[1]].x + fU * (pTerrainVtxPos[dwVtxIdx[0]].x - pTerrainVtxPos[dwVtxIdx[1]].x),
	//				0.f,
	//				pTerrainVtxPos[dwVtxIdx[1]].z + fV * (pTerrainVtxPos[dwVtxIdx[2]].z - pTerrainVtxPos[dwVtxIdx[1]].z));
	//		}

	//		// 왼쪽 아래
	//		dwVtxIdx[0] = dwIndex + VTXCNTX;
	//		dwVtxIdx[1] = dwIndex + 1;
	//		dwVtxIdx[2] = dwIndex;

	//		if (D3DXIntersectTri(&pTerrainVtxPos[dwVtxIdx[1]],
	//			&pTerrainVtxPos[dwVtxIdx[0]],
	//			&pTerrainVtxPos[dwVtxIdx[2]],
	//			&vRayPos, &vRayDir, &fU, &fV, &fDist))
	//		{
	//			// V1 + U(V2 - V1) + V(V3 - V1)

	//			return _vec3(pTerrainVtxPos[dwVtxIdx[1]].x + fU * (pTerrainVtxPos[dwVtxIdx[0]].x - pTerrainVtxPos[dwVtxIdx[1]].x),
	//				0.f,
	//				pTerrainVtxPos[dwVtxIdx[1]].z + fV * (pTerrainVtxPos[dwVtxIdx[2]].z - pTerrainVtxPos[dwVtxIdx[1]].z));
	//		}
	//	}
	//}

	return _vec3(0.f, 0.f, 0.f);
}

void CDynamicCamera::Key_Input(const _float & fTimeDelta)
{


	_matrix		matCamWorld;
	D3DXMatrixInverse(&matCamWorld, 0, &m_matView);

	if (Engine::Get_DIKeyState(DIK_UP))
	{
		_vec3		vLook;
		memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));

		_vec3	vLength = *D3DXVec3Normalize(&vLook, &vLook) * m_fSpeed * fTimeDelta;

		m_vEye += vLength;
		m_vAt += vLength;

	}

	if (Engine::Get_DIKeyState(DIK_DOWN))
	{
		_vec3		vLook;
		memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));

		_vec3	vLength = *D3DXVec3Normalize(&vLook, &vLook) * m_fSpeed * fTimeDelta;

		m_vEye -= vLength;
		m_vAt  -= vLength;
	}

	if (Engine::Get_DIKeyState(DIK_LEFT))
	{
		_vec3		vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3	vLength = *D3DXVec3Normalize(&vRight, &vRight) * m_fSpeed * fTimeDelta;

		m_vEye -= vLength;
		m_vAt  -= vLength;
	}

	if (Engine::Get_DIKeyState(DIK_RIGHT))
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
