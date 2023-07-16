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
    ID3DXFont* Create_3DXFont(_int _Height, _uint _Width, _uint _Weight,
        _bool _Italic, const wchar_t _wchar[], ID3DXFont* _pFont);//높이(_int), 너비(_uint),굵기0~1000(_uint), 기울임(_bool), 글꼴 ""
    void    Set_pFont(ID3DXFont* _pFont) { m_pFont = _pFont; }//매개변수를 받아 컴포넌트 내부의 ID3DXFont* pFont를 설정.
    void    Set_Rect(RECT _rect) { m_rect = _rect; }//글자를 띄울 사각형의 크기 RECT
    void    Set_Anchor(DWORD anchor) { m_dwAnchor = anchor; }//글자를 띄울 사각형의 위치조정. 자세한건 NODRAWTEXT검색
    void    Set_FontColor(_uint _FontColor) { m_FontColor = _FontColor;}//글자의 색을 조정. 0xff ??(빨강) ??(초록) ??(파랑) 설정. 0xffffffff == 흰색
   
    void    DrawText(const TCHAR* outputString, LONG x = 0, LONG y = 0);//출력하고싶은 문장
public:
    static CFont* Create(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual CComponent* Clone(void);
private:
    ID3DXFont*  m_pFont;
    DWORD       m_dwAnchor;
    RECT        m_rect;
    D3DXFONT_DESC m_lf;
    _uint       m_FontColor;//기본 0xffffffff = 흰색
private:
    virtual void Free();
};
END