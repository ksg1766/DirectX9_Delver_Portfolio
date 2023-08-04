#pragma once
#include "Export_Function.h"
#include "Base.h"

BEGIN(Engine)

END

class CCameraManager :
    public CBase
{
	DECLARE_SINGLETON(CCameraManager)

private:
	explicit CCameraManager();
	virtual ~CCameraManager();

public:
	_int			Update_Camera(const _float& fTimeDelta);
	void			LateUpdate_Camera();

	_bool			Is_MapOn() { return m_bToggleMap; }

	void			Add_Camera(CAMERA_TYPE _eType, CGameObject* _pLight){ m_mapCameras.emplace(_eType, _pLight); }
	void			Switch_Camera(CAMERA_TYPE _eType)					{ m_pCurrentCam = m_mapCameras[_eType]; }

	CGameObject*	Get_CurrentCam()									{ return m_pCurrentCam; }
	void			Set_CurrentCam(CGameObject* _pCurrentCam)			{ m_pCurrentCam = _pCurrentCam; }

public:
	void			ZoomInTarget(_vec3& vTargetPoint, const _float& fTimeDelta, _float _fMagnific = 1.5f);
	void			ZoomOutToTrans(CTransform* pTransform, const _float& fTimeDelta);
	void			LookAtTarget(_vec3& vTargetPoint, const _float& fTimeDelta);
	//void			FixOnTarget(_vec3& vTargetPoint, const _float& fTimeDelta);

private:
	map<CAMERA_TYPE, CGameObject*>	m_mapCameras;
	CGameObject*	m_pCurrentCam;

	_bool           m_bToggleMap = false;
	_bool			m_bMakeFrustum = true;

public:
	virtual void Free() override;
};