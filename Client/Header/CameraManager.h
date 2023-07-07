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
	void	Update_Camera();
	void	LateUpdate_Camera();

	_bool	Is_MapOn() { return m_bToggleMap; }

	void	Add_Camera(CAMERA_TYPE _eType, CGameObject* _pLight) { m_mapCameras.emplace(_eType, _pLight); }
	void	Switch_Camera(CAMERA_TYPE _eType) { m_pCurrentCam = m_mapCameras[_eType]; }

private:
	map<CAMERA_TYPE, CGameObject*>	m_mapCameras;
	CGameObject*	m_pCurrentCam;

	_bool           m_bToggleMap = false;

public:
	virtual void Free() override;
};