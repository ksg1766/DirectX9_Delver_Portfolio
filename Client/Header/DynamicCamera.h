#pragma once

#include "Export_Function.h"
#include "Camera.h"

class CDynamicCamera : public CCamera
{
private:
	explicit CDynamicCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CDynamicCamera();

public:
	HRESULT		Ready_Object(const _vec3* pEye,
							const _vec3* pAt,
							const _vec3* pUp,
							const float& fFov,
							const float& fAspect,
							const float& fNear,
							const float& fFar);
	_int		Update_Object(const _float& fTimeDelta);
	void		Render_Object() {}

public:
	_vec3		Picking(HWND hWnd,
		const CVIBuffer* pTargetBuffer,
		const CTransform* pTargetTransform);

private:
	void		Key_Input(const _float& fTimeDelta);
	void		Mouse_Move();
	void		Mouse_Fix();

private:
	_float		m_fSpeed = 10.f;
	_bool		m_bFix   = false;
	_bool		m_bCheck = false;

public:
	static CDynamicCamera*		Create(LPDIRECT3DDEVICE9 pGraphicDev,
										const _vec3* pEye,
										const _vec3* pAt,
										const _vec3* pUp,
										const float& fFov,
										const float& fAspect,
										const float& fNear,
										const float& fFar);
	
private:
	virtual void Free();

};

