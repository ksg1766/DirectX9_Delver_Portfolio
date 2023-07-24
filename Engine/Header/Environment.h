#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CBillBoard;

class ENGINE_DLL CEnvironment :
    public CGameObject
{
protected:
    explicit            CEnvironment(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit            CEnvironment(const CEnvironment& rhs);
    virtual             ~CEnvironment();

public:
    virtual HRESULT     Ready_Object()                          override;
    virtual _int        Update_Object(const _float& fTimeDelta) override;
    virtual void        LateUpdate_Object()                     override;

public:
    ENVIRONMENTTAG      Get_EnvTag()                            { return m_eEnvTag; }
    void                Set_EnvTag(ENVIRONMENTTAG _eEnvTag)     { m_eEnvTag = _eEnvTag; }

    vector<_vec3>&      LoadEnvVertex()                         { return m_vecEnvVertex; }
    vector<INDEX32>&    LoadEnvIndex()                          { return m_vecEnvIndex; }

protected:
    ENVIRONMENTTAG      m_eEnvTag = ENVIRONMENTTAG::ENVIRONMENT_END;

    // Tool
    vector<_vec3>	    m_vecEnvVertex;
    vector<INDEX32>	    m_vecEnvIndex;

    CRcTex*             m_pRcBf         = nullptr;
    CTexture*           m_pTextureCom   = nullptr;
    CBillBoard*         m_pBillBoardCom = nullptr;

protected:
    virtual void	    Free(void);
};

END