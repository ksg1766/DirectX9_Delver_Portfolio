#pragma once
#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CLightManager :
    public CBase
{
	DECLARE_SINGLETON(CLightManager)

private:
	explicit CLightManager();
	virtual ~CLightManager();

public:
	
	
public:
    virtual void Free() override;
};

END