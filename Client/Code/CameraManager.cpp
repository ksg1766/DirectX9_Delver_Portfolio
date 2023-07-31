#include "..\Header\CameraManager.h"
#include "stdafx.h"
#include "FlyingCamera.h"
#include "OrthoCamera.h"
#include "Octree.h"
#include "Frustum.h"

IMPLEMENT_SINGLETON(CCameraManager)

CCameraManager::CCameraManager()
{

}

CCameraManager::~CCameraManager()
{
	Free();
}

_int CCameraManager::Update_Camera(const _float& fTimeDelta)
{
	if (SCENETAG::EDITOR == SceneManager()->Get_Scene()->Get_SceneTag())
	{
		if (Engine::InputDev()->Key_Down(DIK_M))
		{
			if (!m_bToggleMap)
				m_bToggleMap = true;
			else
			{
				m_bToggleMap = false;
				dynamic_cast<COrthoCamera*>(m_mapCameras[CAMERA_TYPE::ORTHOGRAPHIC])->ReturnViewPort();
			}
		}
	}

	return _int();
}

void CCameraManager::LateUpdate_Camera()
{
	CCamera* pCamera;
	if (!m_bToggleMap)
	{
		pCamera = dynamic_cast<CFlyingCamera*>(m_mapCameras[CAMERA_TYPE::PERSPECTIVE])->Get_Camera();
		pCamera->Set_ViewSpcae();
		pCamera->Set_Projection();
	}
	else
	{
		pCamera = dynamic_cast<COrthoCamera*>(m_mapCameras[CAMERA_TYPE::ORTHOGRAPHIC])->Get_Camera();
		dynamic_cast<COrthoCamera*>(m_mapCameras[CAMERA_TYPE::ORTHOGRAPHIC])->Set_OrthoView();
		pCamera->Set_Projection(CAMERA_TYPE::ORTHOGRAPHIC);
	}
	
	if (SCENETAG::EDITOR != SceneManager()->Get_Scene()->Get_SceneTag())
	{
		_matrix _matViewProj = pCamera->Get_ViewMatrix() * pCamera->Get_ProjMatrix();
		Octree()->GetFrustum()->MakeFrustum(&_matViewProj);
	}
}

void CCameraManager::ZoomInTarget(_vec3& vTargetPoint, const _float& fTimeDelta, _float _fMagnific)
{// 잘 안되면 _fMag는 빼자
	_vec3 vStopPoint = (_fMagnific - 1.f) * ((vTargetPoint - m_pCurrentCam->m_pTransform->m_vInfo[INFO_POS]) / _fMagnific);

	D3DXVec3Lerp(&m_pCurrentCam->m_pTransform->m_vInfo[INFO_POS], &m_pCurrentCam->m_pTransform->m_vInfo[INFO_POS], &vStopPoint, fTimeDelta);
}

void CCameraManager::ZoomOutToTrans(CTransform* pTransform, const _float& fTimeDelta)
{
	D3DXVec3Lerp(&m_pCurrentCam->m_pTransform->m_vInfo[INFO_POS], &m_pCurrentCam->m_pTransform->m_vInfo[INFO_POS], &pTransform->m_vInfo[INFO_POS], fTimeDelta);
}

void CCameraManager::LookAtTarget(_vec3& vTargetPoint, const _float& fTimeDelta)
{
	_vec3 vToTarget = vTargetPoint - m_pCurrentCam->m_pTransform->m_vInfo[INFO_POS];

	D3DXVec3Lerp(&m_pCurrentCam->m_pTransform->m_vInfo[INFO_LOOK], &m_pCurrentCam->m_pTransform->m_vInfo[INFO_LOOK], &vToTarget, fTimeDelta);
}

void CCameraManager::Free()
{
}
