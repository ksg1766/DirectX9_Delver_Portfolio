#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CCubeBf;
class CTexture;
class CRigidBody;

class ENGINE_DLL CFragile :
    public CGameObject
{
protected:
    explicit            CFragile(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit            CFragile(const CFragile& rhs);
    virtual             ~CFragile();

public:
    virtual HRESULT     Ready_Object()                          override;
    virtual _int        Update_Object(const _float& fTimeDelta) override;
    virtual void        LateUpdate_Object()                     override;

public:
    FRAGILETAG          Get_FragileTag() { return m_eFragileTag; }
    void                Set_FragileTag(FRAGILETAG _eFragileTag) { m_eFragileTag = _eFragileTag; }

    vector<_vec3>&      LoadFragileVertex() { return m_vecFragileVertex; }
    vector<INDEX32>&    LoadFragileIndex() { return m_vecFragileIndex; }

protected:
    FRAGILETAG          m_eFragileTag = FRAGILETAG::FRAGILE_END;

    // Tool
    CCubeBf*            m_pCubeBf = nullptr;
    CTexture*           m_pTexture = nullptr;
    CRigidBody*         m_pRigidBody = nullptr;
    CGameObject*        m_pOtherObj;

    _float			    m_fHitCool;
    _float			    m_fShakeDelay;
    _bool			    m_bHit;
    _bool			    m_bShake;
    _int			    m_iHP;

    _vec3			    m_vOriginUp;
    _vec3			    m_vOriginLook;
    _vec3			    m_vOriginRight;
    _float			    m_fLerpTime;



    vector<_vec3>	    m_vecFragileVertex;
    vector<INDEX32>	    m_vecFragileIndex;

protected:
    virtual void	    Free(void);
};

END