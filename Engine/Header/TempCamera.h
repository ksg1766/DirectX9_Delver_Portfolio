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
	_bool			Get_CameraMode() { return m_bCameraCheck; }	// false 일 때 1인칭인듯...?

	_matrix&		Get_ViewMat() { return m_matView; }
	_matrix&		Get_ProjMat() { return m_matProj; }

protected:
	_vec3			m_vEye, m_vAt, m_vUp;
	_float			m_fNear, m_fFar, m_fAspect, m_fFov;
	_matrix			m_matView, m_matProj;

	_bool			m_bCameraCheck;

protected:
	virtual void	Free(void);
};

END
