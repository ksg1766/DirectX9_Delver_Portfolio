#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CFont :
    public CComponent
{
private:
    explicit    CFont();
    explicit    CFont(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit    CFont(const CComponent& rhs);
    virtual     ~CFont();

public:
    HRESULT Ready_Font();
    void    Set_pFont(ID3DXFont* _pFont) { m_pFont = _pFont; }
    void    Set_Rect(RECT _rect) { m_rect = _rect; }
    void    Set_Anchor(DWORD anchor) { m_dwAnchor = anchor; }
    void    Set_FontColor(_uint _FontColor) { m_FontColor = _FontColor;}
   
    void    DrawText(const TCHAR* outputString, LONG x = 0, LONG y = 0);
public:
    static CFont* Create(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual CComponent* Clone(void);
private:
    ID3DXFont*  m_pFont;
    DWORD       m_dwAnchor;
    RECT        m_rect;
    _uint       m_FontColor;//±âº» 0xffffffff = Èò»ö
private:
    virtual void Free();
};
END