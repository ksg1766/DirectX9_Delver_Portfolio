#pragma once
#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class CLight;
class ENGINE_DLL CLightManager :
    public CBase
{
	DECLARE_SINGLETON(CLightManager)

private:
	explicit CLightManager();
	virtual ~CLightManager();

public:
	void	Add_Light(LIGHT_TYPE _eType, CLight* _pLight) { m_mapLights[_eType].push_back(_pLight); }
	void	Swtich_All();

private:
	map<LIGHT_TYPE, vector<CLight*>>	m_mapLights;
	
public:
    virtual void Free() override;
};

END