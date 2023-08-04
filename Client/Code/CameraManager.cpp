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
				static_cast<COrthoCamera*>(m_mapCameras[CAMERA_TYPE::ORTHOGRAPHIC])->ReturnViewPort();
			}
		}
	}

	if (Engine::InputDev()->Key_Pressing(DIK_LCONTROL) && Engine::InputDev()->Key_Down(DIK_F))
	{
		m_bMakeFrustum = !m_bMakeFrustum;
	}

	return _int();
}

void CCameraManager::LateUpdate_Camera()
{
	CCamera* pCamera;
	if (!m_bToggleMap)
	{
		pCamera = static_cast<CFlyingCamera*>(m_mapCameras[CAMERA_TYPE::PERSPECTIVE])->Get_Camera();
		pCamera->Set_ViewSpcae();
		pCamera->Set_Projection();
	}
	else
	{
		pCamera = static_cast<COrthoCamera*>(m_mapCameras[CAMERA_TYPE::ORTHOGRAPHIC])->Get_Camera();
		static_cast<COrthoCamera*>(m_mapCameras[CAMERA_TYPE::ORTHOGRAPHIC])->Set_OrthoView();
		pCamera->Set_Projection(CAMERA_TYPE::ORTHOGRAPHIC);
	}
	
	if (SCENETAG::EDITOR != SceneManager()->Get_Scene()->Get_SceneTag())
	{
		_matrix _matViewProj = pCamera->Get_ViewMatrix() * pCamera->Get_ProjMatrix();

		if(m_bMakeFrustum)
			Octree()->GetFrustum()->MakeFrustum(&_matViewProj);
	}
}

void CCameraManager::ZoomInTarget(_vec3& vTargetPoint, const _float& fTimeDelta, _float _fMagnific)
{// 잘 안되면 _fMag는 빼자
	_vec3 vStopPoint = (_fMagnific - 1.f) * ((vTargetPoint - m_pCurrentCam->m_pTransform->m_vInfo[INFO_POS]) / _fMagnific);

	D3DXVec3Lerp(&m_pCurrentCam->m_pTransform->m_vInfo[INFO_POS], &m_pCurrentCam->m_pTransform->m_vInfo[INFO_POS], &(m_pCurrentCam->m_pTransform->m_vInfo[INFO_POS] + vStopPoint), 3.f * fTimeDelta);
}

void CCameraManager::ZoomOutToTrans(CTransform* pTransform, const _float& fTimeDelta)
{
	D3DXVec3Lerp(&m_pCurrentCam->m_pTransform->m_vInfo[INFO_POS], &m_pCurrentCam->m_pTransform->m_vInfo[INFO_POS], &pTransform->m_vInfo[INFO_POS], 8.f * fTimeDelta);
	D3DXVec3Lerp(&m_pCurrentCam->m_pTransform->m_vInfo[INFO_LOOK], &m_pCurrentCam->m_pTransform->m_vInfo[INFO_LOOK], &pTransform->m_vInfo[INFO_LOOK], 8.f * fTimeDelta);
	D3DXVec3Cross(&m_pCurrentCam->m_pTransform->m_vInfo[INFO_RIGHT], &_vec3(0.f, 1.f, 0.f), &m_pCurrentCam->m_pTransform->m_vInfo[INFO_LOOK]);
	D3DXVec3Cross(&m_pCurrentCam->m_pTransform->m_vInfo[INFO_UP], &m_pCurrentCam->m_pTransform->m_vInfo[INFO_LOOK], &m_pCurrentCam->m_pTransform->m_vInfo[INFO_RIGHT]);
}

void CCameraManager::LookAtTarget(_vec3& vTargetPoint, const _float& fTimeDelta)
{
	_vec3 vToTarget = vTargetPoint - m_pCurrentCam->m_pTransform->m_vInfo[INFO_POS];

	D3DXVec3Lerp(&m_pCurrentCam->m_pTransform->m_vInfo[INFO_LOOK], &m_pCurrentCam->m_pTransform->m_vInfo[INFO_LOOK], &vToTarget, fTimeDelta);
	D3DXVec3Cross(&m_pCurrentCam->m_pTransform->m_vInfo[INFO_RIGHT], &_vec3(0.f, 1.f, 0.f), &m_pCurrentCam->m_pTransform->m_vInfo[INFO_LOOK]);
	D3DXVec3Cross(&m_pCurrentCam->m_pTransform->m_vInfo[INFO_UP], &m_pCurrentCam->m_pTransform->m_vInfo[INFO_LOOK], &m_pCurrentCam->m_pTransform->m_vInfo[INFO_RIGHT]);
}

void CCameraManager::Free()
{
}
