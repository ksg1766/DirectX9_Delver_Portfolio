#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CCubeBf;
class CTexture;
class CState;
END
class CBoss_MeteorCube :
    public CGameObject
{
private:
    explicit CBoss_MeteorCube(LPDIRECT3DDEVICE9 pGraphicDev );
    explicit	CBoss_MeteorCube(const CBoss_MeteorCube& rhs);
    virtual ~CBoss_MeteorCube();

public:
    virtual HRESULT Ready_Object() override;
    virtual _int Update_Object(const _float& fTimeDelta) override;
    virtual void LateUpdate_Object(void) override;
    virtual void Render_Object(void) override;

public:
    virtual void    Set_Center(_vec3    _CenterPos);
   
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

    STATE   m_eState;

    _float   m_fScale;
    _float   m_bMaxHeight;

    _bool   m_bChanneling_End;

    _vec3 m_vCenter;
public:
    HRESULT		Add_Component();
public:
    static CBoss_MeteorCube* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
    virtual void Free();
};

