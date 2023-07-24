#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CCubeBf;
class ENGINE_DLL CTrap :
    public CGameObject
{
protected:
    explicit            CTrap(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit            CTrap(const CTrap& rhs);
    virtual             ~CTrap();

public:
    virtual HRESULT     Ready_Object()      override;
    virtual _int        Update_Object(const _float& fTimeDelta) override;
    virtual void        LateUpdate_Object() override;

public:
    TRAPTAG             Get_TrapTag()                   { return m_eTrapTag; }
    void                Set_TrapTag(TRAPTAG _eTrapTag)  { m_eTrapTag = _eTrapTag; }

    vector<_vec3>&      LoadTrapVertex()                { return m_vecTrapVertex; }
    vector<INDEX32>&    LoadTrapIndex()                 { return m_vecTrapIndex; }

protected:
    TRAPTAG             m_eTrapTag = TRAPTAG::TRAP_END;

    // Tool
    CCubeBf*            m_pCubeBf = nullptr;

    vector<_vec3>	    m_vecTrapVertex;
    vector<INDEX32>	    m_vecTrapIndex;

protected:
    virtual void	    Free(void);
};

END