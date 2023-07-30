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
	
	_matrix _matViewProj = pCamera->Get_ViewMatrix() * pCamera->Get_ProjMatrix();
	Octree()->GetFrustum()->MakeFrustum(&_matViewProj);
}

void CCameraManager::Free()
{
}
