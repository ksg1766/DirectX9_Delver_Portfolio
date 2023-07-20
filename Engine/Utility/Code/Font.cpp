#include "Font.h"

CFont::CFont()
{
}

CFont::CFont(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev)
{
}

CFont::CFont(const CComponent& rhs)
	: CComponent(rhs)
{
}

CFont::~CFont()
{

}


HRESULT CFont::Ready_Font()
{
	m_pFont = 0;
	m_dwAnchor = 0;
	m_rect = {0,0, 0, 0};
	m_FontColor = 0xffffffff;
	return S_OK;
}

ID3DXFont* CFont::Create_3DXFont(_int _Height, _uint _Width, _uint _Weight, _bool _Italic, WCHAR _wchar[], ID3DXFont* _pFont)
{
	D3DXFONT_DESC	lf;
	ZeroMemory(&lf, sizeof(D3DXFONT_DESC));
	lf.Height = _Height;//높이 논리적 단위
	lf.Width = _Width;//너비 논리적 단위
	lf.Weight = _Weight;//굵기의 단위	0~1000
	lf.Italic = _Italic;//기울임꼴
	lf.MipLevels = 1;//요청된 밉수준. 값이 1일 때 텍스처 공간이 화면 공간에 동일하게 맵핑
	lf.CharSet = DEFAULT_CHARSET;//문자집합
	lf.OutputPrecision = OUT_DEFAULT_PRECIS;//출력 정밀도
	lf.Quality = DEFAULT_QUALITY;//품질
	lf.PitchAndFamily = DEFAULT_PITCH;//피치 및 패밀리
	lf.FaceName, _wchar;//문자열의 길이가 32를 초과하면 안됨. 글꼴설정.

	if (FAILED(D3DXCreateFontIndirect(m_pGraphicDev, &lf, &_pFont)))
	{
		MessageBox(0, TEXT("D3DXCreateFontIndirect() - FAILED"), 0, 0);
	}
	return _pFont;
}



void CFont::DrawText(const TCHAR* outputString, LONG x, LONG y)
{
	m_rect;
#ifdef UNICODE
	m_pFont->DrawTextW(0, outputString, -1, &m_rect, m_dwAnchor, m_FontColor);//출력문자열, null문자열, 텍스트 사각형, 사각형위치, 글꼴색
#else
	m_pFont->DrawTextA(0, outputString, -1, &m_rect, m_dwAnchor, m_FontColor);
#endif
}

CFont* CFont::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CFont* pInstance = new CFont(pGraphicDev);

	if (FAILED(pInstance->Ready_Font()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Font_Component Create Failed");
		return nullptr;
	}

	return pInstance;
}

CComponent* CFont::Clone(void)
{
	return new CFont(*this);
}

void CFont::Free()
{
	CComponent::Free();
}
