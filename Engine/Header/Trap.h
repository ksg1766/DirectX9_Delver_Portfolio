#pragma once
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CTrap :
    public CGameObject
{
protected:
    explicit CTrap(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CTrap(const CTrap& rhs);
    virtual ~CTrap();

public:
    HRESULT Ready_Object();
    _int    Update_Object(const _float& fTimeDelta);
    void    LateUpdate_Object();

public:
    TRAPTAG Get_TrapTag()                   { return m_eTrapTag; }
    void    Set_TrapTag(TRAPTAG _eTrapTag)  { m_eTrapTag = _eTrapTag; }

protected:
    TRAPTAG m_eTrapTag = TRAPTAG::TRAP_END;

protected:
    virtual void	Free(void);
};

END