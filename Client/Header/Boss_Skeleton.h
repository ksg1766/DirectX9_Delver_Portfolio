#pragma once
#include "Monster.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CBillBoard;
class CAnimator;
END
class CTerrain;
class CBoss_Skeleton :
    public CMonster
{
private:
    explicit CBoss_Skeleton(LPDIRECT3DDEVICE9 pGrapicDev);
    explicit CBoss_Skeleton(const CBoss_Skeleton& rhs);
    virtual ~CBoss_Skeleton();

public:
    virtual HRESULT    Ready_Object()                                override;
    virtual _int    Update_Object(const _float& fTimeDelta)        override;
    virtual void    LateUpdate_Object()                            override;
    virtual void    Render_Object()                                override;

public:
    void        Set_Terrain(CTerrain* _pCurrentTerrain) { m_pTerrain = _pCurrentTerrain; }

    virtual void        OnCollisionEnter(CCollider* _pOther);
    virtual void        OnCollisionStay(CCollider* _pOther);
    virtual void        OnCollisionExit(CCollider* _pOther);

private:
    HRESULT        Add_Component();
    void        ForceHeight(_vec3 _vPos);

private:
    CRcTex* m_pBuffer = nullptr;
    CTexture* m_pTexture[static_cast<_uint>(STATE::STATE_END)] = {};
    CTerrain* m_pTerrain = nullptr;
    CAnimator* m_pAnimator = nullptr;


    _bool        m_bAttackTick;
    _float        m_fFrame;

    STATE        m_eState;

public:
    static CBoss_Skeleton* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
    virtual void Free() override;



};

