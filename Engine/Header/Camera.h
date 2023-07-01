#pragma once
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CCamera :
	public CGameObject
{
protected:
	explicit CCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCamera(const CCamera& rhs);
	virtual ~CCamera();

public:
	HRESULT			Ready_Object();
	_int			Update_Object(const _float& fTimeDelta);

protected:
	_vec3			m_vEye, m_vAt, m_vUp;
	_float			m_fNear, m_fFar, m_fAspect, m_fFov;
	_matrix			m_matView, m_matProj;

protected:
	virtual void	Free(void);
};

END
