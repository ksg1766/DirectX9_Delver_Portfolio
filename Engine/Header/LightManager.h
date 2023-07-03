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
	void	Swtich_All();

private:
	vector<CLight*>	m_LightList;
	
public:
    virtual void Free() override;
};

END