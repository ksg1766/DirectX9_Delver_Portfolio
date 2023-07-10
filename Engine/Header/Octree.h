#pragma once
#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL COctree :
    public CBase
{
    DECLARE_SINGLETON(COctree)

private:
    explicit COctree();
    virtual ~COctree();

public:
    virtual void Free();
};

END