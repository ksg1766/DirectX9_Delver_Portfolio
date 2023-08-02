#pragma once
#include "Monster.h"

BEGIN(Engine)
class CCubeBf;
class CTexture;
class CState;
END
class CBoss_MeteorCube3Ph :
    public CMonster
{
private:
    explicit CBoss_MeteorCube3Ph(LPDIRECT3DDEVICE9 pGraphicDev );
    explicit	CBoss_MeteorCube3Ph(const CBoss_MeteorCube3Ph& rhs);
    virtual ~CBoss_MeteorCube3Ph();

public:
    virtual HRESULT Ready_Object()                          override;
    virtual _int Update_Object(const _float& fTimeDelta)    override;
    virtual void LateUpdate_Object(void)                    override;
    virtual void Render_Object(void)                        override;

public:
    virtual void    Init_Stat()                             override;

    virtual void    Set_Center(_vec3    _CenterPos);
    virtual void    Set_PlayerPos();
    virtual void    Set_ChannelingEnd() { m_bChanneling_End = true; }

    virtual void    Channeling_Begin();
    virtual void     Channeling_Now(const _float& fTimeDelta);
    virtual void     Channeling_End(const _float& fTimeDelta);

    virtual void		OnCollisionEnter(CCollider* _pOther);
    virtual void		OnCollisionStay(CCollider* _pOther);
    virtual void		OnCollisionExit(CCollider* _pOther);

private:
    //함수
private:
    //변수
    CCubeBf* m_pCubeBf;
    CTexture* m_pTexture;

    _vec3 m_vDir;
    _vec3   m_vTargetPos;

    _float  m_fMeteorExplosionTime;
    _float   m_fEndTime;
    _float   m_fScale;
    _float   m_bMaxHeight;

    _bool   m_bChanneling_Start;
    _bool   m_bChanneling_End;
    _bool   m_bTargetSet;
    _bool   m_bHit;
    _bool   m_bSound;
    _vec3    m_vCenter;
    _float m_fAttack;
    _float  m_fDistance;

public:
    HRESULT		Add_Component();
public:
    static CBoss_MeteorCube3Ph* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
    virtual void Free();
};

