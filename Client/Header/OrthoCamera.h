#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CTransform;
class CCamera;

END

class COrthoCamera :
    public CGameObject
{
private:
    explicit COrthoCamera(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit COrthoCamera(const COrthoCamera& rhs);
    virtual ~COrthoCamera();

public:
    virtual HRESULT Ready_Object(void)                      override;
    virtual _int Update_Object(const _float& fTimeDelta)    override;
    virtual void LateUpdate_Object(void)                    override;
    virtual void Render_Object(void)                        override;

public:
    CCamera*    Get_Camera() { return m_pCamera; }

    void        Set_OrthoView();

    void        ReturnViewPort();// { m_pGraphicDev->SetViewport(&m_backupViewPort); }
private:
    HRESULT		Add_Component(void);


    //  CTransform* m_pTransform;	// Transform Attached to this GameObject
    CCamera*    m_pCamera = nullptr;

public:
    static COrthoCamera* Create(LPDIRECT3DDEVICE9 pGraphicDev);
    D3DVIEWPORT9	m_backupViewPort;

private:
    virtual void Free() override;
};

