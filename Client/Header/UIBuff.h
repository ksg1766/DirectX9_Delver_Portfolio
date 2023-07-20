#pragma once
#include "TempUI.h"
BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CFont;
END

class CUIBuff :
    public CTempUI
{
private:
    explicit CUIBuff(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CUIBuff();

public:
    HRESULT		Ready_Object();
    _int		Update_Object(const _float& fTimeDelta);
    void		LateUpdate_Object(void);
    void		Render_Object();

private:
    HRESULT		Add_Component(void);

    CFont* m_pFont = nullptr;
    ID3DXFont* m_pFontconfig;
    CGameObject* m_pGameObject;

    _float       m_fRemainingTime;
public:
    static CUIBuff* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
    virtual void Free();

};

