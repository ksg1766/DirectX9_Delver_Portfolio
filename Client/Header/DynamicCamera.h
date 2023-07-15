#pragma once

#include "Export_Function.h"
#include "TempCamera.h"

class CDynamicCamera : public CTempCamera
{
	enum CAMERA_MODE { CAMERA_FIRST, CAMERA_THIRD, CAMERA_END };

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
	HRESULT		Add_Component();
	void		Render_Object() {}

private:
	void		Key_Input(const _float& fTimeDelta);
	void		Mouse_Move();
	void		Mouse_Fix();

private:
	void		First_Camera();
	void		Third_Camera();
	void		Shake_Camera();

public:
	//_bool		Get_CameraMode() { return m_bCameraCheck; }
	_bool		Get_MouseFix()	 { return m_bFix; }

public:
	void		Set_Fix(_bool _Fix) { m_bFix = _Fix; }

private:
	_float		m_fSpeed = 40.f;
	_bool		m_bFix = false;
	_bool		m_bCheck = false;

	// Camera Shake
	_bool		m_bShaking; // 흔들림 상태
	_float		m_fShakeElipsedTime; // 중첩시간

	_float		m_fAmplitude; // 진폭
	_float		m_fDuration;  // 흔드는 시간
	_float		m_fFrequency; // 흔드는 속도

	_vec3		m_vOriginPos; // 원래 있던 위치
	CAMERA_MODE m_eCamera_Mode; // 카메라 모드 상태

	//_bool		m_bCameraCheck;
public:
	static CDynamicCamera* Create(LPDIRECT3DDEVICE9 pGraphicDev,
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

