#include "..\Header\CameraManager.h"
#include "stdafx.h"
#include "FlyingCamera.h"
#include "OrthoCamera.h"

IMPLEMENT_SINGLETON(CCameraManager)

CCameraManager::CCameraManager()
{

}

CCameraManager::~CCameraManager()
{
	Free();
}

void CCameraManager::Update_Camera()
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
}

void CCameraManager::Free()
{
}
