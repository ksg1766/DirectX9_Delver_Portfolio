#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CTransform;
class CCamera;

END

class CFlyingCamera :
    public CGameObject
{
private:
    explicit        CFlyingCamera(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit        CFlyingCamera(const CFlyingCamera& rhs);
    virtual         ~CFlyingCamera();

public:
    virtual HRESULT Ready_Object(void)                      override;
    virtual _int    Update_Object(const _float& fTimeDelta)    override;
    virtual void    LateUpdate_Object(void)                    override;
    virtual void    Render_Object(void)                        override;

public:
    CCamera*        Get_Camera() { return m_pCamera; }

private:
    HRESULT		    Add_Component(void);
    void		    Key_Input(const _float& fTimeDelta);

private:
    _float			m_fSpeed = 30.f;

//  CTransform*     m_pTransform;	// Transform Attached to this GameObject
    CCamera*        m_pCamera = nullptr;

public:
    static CFlyingCamera* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
    virtual void Free() override;
};

