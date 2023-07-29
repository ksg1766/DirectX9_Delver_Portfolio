#pragma once
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CTempCamera :
	public CGameObject
{
protected:
	explicit CTempCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTempCamera(const CTempCamera& rhs);
	virtual ~CTempCamera();

public:
	HRESULT			Ready_Object();
	virtual _int	Update_Object(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Object()						override;
	_bool			Get_CameraMode() { return m_bCameraCheck; }

	_matrix& Get_ViewMat() { return m_matView; }
	_matrix& Get_ProjMat() { return m_matProj; }

public:
	CAMERA_TYPE	Get_CameraTag()				     { return m_eCameraType; }
	void		Set_CameraTag(CAMERA_TYPE _eTag) { m_eCameraType = _eTag; }

protected:
	_vec3			m_vEye, m_vAt, m_vUp;
	_float			m_fNear, m_fFar, m_fAspect, m_fFov;
	_matrix			m_matView, m_matProj;

	_bool			m_bCameraCheck;
	CAMERA_TYPE		m_eCameraType;

protected:
	virtual void	Free(void);
};

END
