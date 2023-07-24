#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CRcTex;
class CTexture;
class CBillBoard;
class CStateMachine;
END

class CTorch :
    public CGameObject
{
private:
    explicit CTorch(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CTorch(const CTorch& rhs);
public:
    virtual HRESULT Ready_Object(void)						override;
    virtual _int	Update_Object(const _float& fTimeDelta)	override;
    virtual void	LateUpdate_Object(void)					override;
    virtual void	Render_Object(void)						override;

private:
    CRcTex* m_pBuffer = nullptr;
    CTexture* m_pTexture = nullptr;
    CGameObject* m_pOtherObj = nullptr;
    CBillBoard* m_pBillBoard = nullptr;
    _float      m_fFrame;
private:
    HRESULT			Add_Component(void);
public:
    static CTorch* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
    virtual void Free() override;
};

